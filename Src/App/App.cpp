#include "App.h"

#include "../Main/Global.h"

int Global::window_width = 0;
int Global::window_height = 0;
float Global::dt = 0.0f;

App::App(const char* title, bool fullscreen, int width, int height)
    : m_window_title(title), m_fullscreen(fullscreen), m_window_width(width), m_window_height(height) {}

int App::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "ERROR: SDL2 video subsystem couldn't be initialized: %s\n", SDL_GetError());
        return APP_FAILURE;
    }

    // Enable to be able to capture a frame in RenderDoc
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Enable to receive debug callback (need to uncomment lines 53-54 to specify the callback function)
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    if (m_fullscreen) {
        m_sdl_window = SDL_CreateWindow(
            m_window_title,
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            0, 0,
            SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    } else {
        m_sdl_window = SDL_CreateWindow(
            m_window_title,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            m_window_width, m_window_height,
            SDL_WINDOW_OPENGL);
    }

    m_gl_context = SDL_GL_CreateContext(m_sdl_window);

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        fprintf(stderr, "ERROR: Failed to initialize the OpenGL context.\n");
        return APP_FAILURE;
    }

//    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//    glDebugMessageCallback(gl_debug_message_callback, nullptr);

    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) == 0) {
        m_screen_width = dm.w;
        m_screen_height = dm.h;
        if (m_fullscreen) {
            m_window_width = m_screen_width;
            m_window_height = m_screen_height;
        }
    } else {
        printf("WARNING: SDL couldn't retrieve current display mode details.\n");
    }

    gltInit();
    printf("MESSAGE: OpenGL version %d.%d initialised.\n", GLVersion.major, GLVersion.minor);

    Global::window_height = m_window_height;
    Global::window_width = m_window_width;

    load_scene(IMMEDIATE);
    m_osd_full_mode = true;

    return APP_SUCCESS;
}

void App::load_scene(Scene_Type type) {
    if (m_current_scene_type == type)
        return;

    m_current_scene.reset();
    switch (m_current_scene_type) {
    case IMMEDIATE:
        scene_immediate->done();
        scene_immediate.reset();
        break;
    case MODERN:
        scene_modern->done();
        scene_modern.reset();
        break;
    case INSTANCED:
        scene_instanced->done();
        scene_instanced.reset();
        break;
    case INSTANCED_GLSL:
        scene_instanced_glsl->done();
        scene_instanced_glsl.reset();
        break;
    case ANIMATED:
        scene_animated->done();
        scene_animated.reset();
        break;
    case ANIMATED_GLSL:
        scene_animated_glsl->done();
        scene_animated_glsl.reset();
        break;
    default:
        break;
    }

    switch (type) {
    case IMMEDIATE:
        scene_immediate = std::make_unique<Scene_Immediate>();
        scene_immediate->init();
        m_current_scene = scene_immediate;
        break;
    case MODERN:
        scene_modern = std::make_unique<Scene_Modern>();
        scene_modern->init();
        m_current_scene = scene_modern;
        break;
    case INSTANCED:
        scene_instanced = std::make_unique<Scene_Instanced>();
        scene_instanced->init();
        m_current_scene = scene_instanced;
        break;
    case INSTANCED_GLSL:
        scene_instanced_glsl = std::make_unique<Scene_Instanced_GLSL>();
        scene_instanced_glsl->init();
        m_current_scene = scene_instanced_glsl;
        break;
    case ANIMATED:
        scene_animated = std::make_unique<Scene_Animated>();
        scene_animated->init();
        m_current_scene = scene_animated;
        break;
    case ANIMATED_GLSL:
        scene_animated_glsl = std::make_unique<Scene_Animated_GLSL>();
        scene_animated_glsl->init();
        m_current_scene = scene_animated_glsl;
        break;
    default:
        break;
    }

    m_current_scene_type = type;
}

void App::run() {
    bool quit = false;
    while (!quit) {
        Uint64 start_time = SDL_GetPerformanceCounter();
        static Uint32 last_time = 0;
        Uint32 current_time = SDL_GetTicks();
        Uint32 dt_milli = current_time - last_time;
        last_time = current_time;
        Global::dt = (float) dt_milli * 0.001f;

        quit = tick();
        Uint64 end_time = SDL_GetPerformanceCounter();
        if (m_elapsed_frames == FRAME_RATE_INTERVAL) {
            m_framerate = 1.0f / ((float) (end_time - start_time) / (float) SDL_GetPerformanceFrequency());
            m_framerate_accum += m_framerate;
            m_framerate_iterations++;
        }

        m_elapsed_frames = (m_elapsed_frames + 1) % (FRAME_RATE_INTERVAL + 1);
    }
}

void App::render() {
    glClearColor(0.25, 0.25, 0.25, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_window_width, m_window_height);

    render_osd();
    m_current_scene->display();

    SDL_GL_SwapWindow(m_sdl_window);
}

void App::render_osd() {
    GLfloat x = 10.0f;
    GLfloat y = 10.0f;
    GLfloat offset = 40.0f;

    GLTtext* fps = gltCreateText();
    gltSetText(fps, ("FPS: " + std::to_string((unsigned int) m_framerate)).c_str());
    gltBeginDraw();
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(fps, x, y, 2.0);
    gltEndDraw();
    gltDeleteText(fps);
    y += offset;

    if (!m_osd_full_mode) {
        glUseProgram(0);
        return;
    }

    GLTtext* scene_name = gltCreateText();
    gltSetText(scene_name, ("Scene: " + std::string(m_current_scene->m_name)).c_str());
    gltBeginDraw();
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(scene_name, x, y, 2.0);
    gltEndDraw();
    gltDeleteText(scene_name);
    y += offset;

    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);

    GLTtext* resolution = gltCreateText();
    gltSetText(resolution, ("Resolution: " + std::to_string(dm.w) + "x" + std::to_string(dm.h)).c_str());
    gltBeginDraw();
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(resolution, x, y, 2.0);
    gltEndDraw();
    gltDeleteText(resolution);
    y += offset;

    GLTtext* refresh_rate = gltCreateText();
    gltSetText(refresh_rate, ("Refresh Rate: " + std::to_string(dm.refresh_rate) + " (hz)").c_str());
    gltBeginDraw();
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(refresh_rate, x, y, 2.0);
    gltEndDraw();
    gltDeleteText(refresh_rate);
    y += offset;

    GLTtext* subdivision = gltCreateText();
    gltSetText(subdivision, ("Subdivision: " + std::to_string(m_current_scene->m_subdivision)).c_str());
    gltBeginDraw();
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(subdivision, x, y, 2.0);
    gltEndDraw();
    gltDeleteText(subdivision);
    y += offset;

    GLTtext* vertices = gltCreateText();
    gltSetText(vertices, ("Vertices: " + std::to_string(m_current_scene->m_mesh->m_data->size())).c_str());
    gltBeginDraw();
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(vertices, x, y, 2.0);
    gltEndDraw();
    gltDeleteText(vertices);
    y += offset;

    GLTtext* faces = gltCreateText();
    gltSetText(faces, ("Faces: " + std::to_string(m_current_scene->m_mesh->m_indices->size() / 3)).c_str());
    gltBeginDraw();
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(faces, x, y, 2.0);
    gltEndDraw();
    gltDeleteText(faces);
    y += offset;

    GLTtext* data = gltCreateText();
    std::string bytes = std::to_string(m_current_scene->m_mesh->m_data->size() * 12);
    long long int n = (signed) bytes.length() - 3;
    while (n > 0) {
        bytes.insert(n, ",");
        n -= 3;
    }
    gltSetText(data, ("Data: " + bytes + " (bytes)").c_str());
    gltBeginDraw();
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(data, x, y, 2.0);
    gltEndDraw();
    gltDeleteText(data);
    y += offset;

    std::string enabled = "Enabled";
    std::string disabled = "Disabled";

    GLTtext* lighting = gltCreateText();
    gltSetText(lighting, ("Lighting: " + (m_current_scene->m_lighting_enabled ? enabled : disabled)).c_str());
    gltBeginDraw();
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(lighting, x, y, 2.0);
    gltEndDraw();
    gltDeleteText(lighting);
    y += offset;

    GLTtext* depth_testing = gltCreateText();
    gltSetText(depth_testing,
        ("Depth Testing: " + (m_current_scene->m_depth_buffering_enabled ? enabled : disabled)).c_str());
    gltBeginDraw();
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(depth_testing, x, y, 2.0);
    gltEndDraw();
    gltDeleteText(depth_testing);
    y += offset;

    GLTtext* backface_culling = gltCreateText();
    gltSetText(backface_culling,
        ("Backface Culling: " + (m_current_scene->m_backface_culling_enabled ? enabled : disabled)).c_str());
    gltBeginDraw();
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(backface_culling, x, y, 2.0);
    gltEndDraw();
    gltDeleteText(backface_culling);
    y += offset;
    GLTtext* number_of_lights = gltCreateText();
    gltSetText(number_of_lights,
        ("Number of Lights: " + std::to_string(
            m_current_scene->m_point_light_count)).c_str());
    gltBeginDraw();
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(number_of_lights, x, y, 2.0);
    gltEndDraw();
    gltDeleteText(number_of_lights);

    glUseProgram(0);
}

void App::handle_input() {
    std::unique_ptr<Camera>& camera = m_current_scene->m_camera;
    SDL_PumpEvents();

    int mouse_x = 0, mouse_y = 0;
    SDL_GetRelativeMouseState(&mouse_x, &mouse_y);

    if (mouse_y != 0)
        camera->pitch((float) -mouse_y * MOUSE_SENSITIVITY * CAMERA_ROTATION_FACTOR);
    if (mouse_x != 0)
        camera->yaw((float) -mouse_x * MOUSE_SENSITIVITY * CAMERA_ROTATION_FACTOR);

    const Uint8* keys;
    keys = SDL_GetKeyboardState(nullptr);
    if (keys) {
        if (keys[SDL_SCANCODE_ESCAPE])
            m_quit = true;

        // Uncomment for a different mode of control (need to comment lines 368-393)
        //if (keys[SDL_SCANCODE_UP])
        //    camera->translate(camera->m_position + camera->m_forward * (CAMERA_MOVE_FACTOR * Global::dt));
        //if (keys[SDL_SCANCODE_DOWN])
        //    camera->translate(camera->m_position - camera->m_forward * (CAMERA_MOVE_FACTOR * Global::dt));
        //if (keys[SDL_SCANCODE_LEFT])
        //    camera->translate(camera->m_position - camera->m_right * (CAMERA_MOVE_FACTOR * Global::dt));
        //if (keys[SDL_SCANCODE_RIGHT])
        //    camera->translate(camera->m_position + camera->m_right * (CAMERA_MOVE_FACTOR * Global::dt));
        //if (keys[SDL_SCANCODE_LCTRL])
        //    camera->translate(camera->m_position - camera->m_up * (CAMERA_MOVE_FACTOR * Global::dt));
        //if (keys[SDL_SCANCODE_SPACE])
        //    camera->translate(camera->m_position + camera->m_up * (CAMERA_MOVE_FACTOR * Global::dt));

        //if (keys[SDL_SCANCODE_W])
        //    camera->pitch(-CAMERA_ROTATION_FACTOR * Global::dt);
        //if (keys[SDL_SCANCODE_S])
        //    camera->pitch(CAMERA_ROTATION_FACTOR * Global::dt);
        //if (keys[SDL_SCANCODE_A])
        //    camera->roll(-CAMERA_ROTATION_FACTOR * Global::dt);
        //if (keys[SDL_SCANCODE_D])
        //    camera->roll(CAMERA_ROTATION_FACTOR * Global::dt);
        //if (keys[SDL_SCANCODE_Q])
        //    camera->yaw(CAMERA_ROTATION_FACTOR * Global::dt);
        //if (keys[SDL_SCANCODE_E])
        //    camera->yaw(-CAMERA_ROTATION_FACTOR * Global::dt);

        if (keys[SDL_SCANCODE_LCTRL])
            camera->translate(camera->m_position - camera->m_up * (CAMERA_MOVE_FACTOR * Global::dt));
        if (keys[SDL_SCANCODE_SPACE])
            camera->translate(camera->m_position + camera->m_up * (CAMERA_MOVE_FACTOR * Global::dt));

        if (keys[SDL_SCANCODE_W])
            camera->translate(camera->m_position + camera->m_forward * (CAMERA_MOVE_FACTOR * Global::dt));
        if (keys[SDL_SCANCODE_S])
            camera->translate(camera->m_position - camera->m_forward * (CAMERA_MOVE_FACTOR * Global::dt));
        if (keys[SDL_SCANCODE_A])
            camera->translate(camera->m_position - camera->m_right * (CAMERA_MOVE_FACTOR * Global::dt));
        if (keys[SDL_SCANCODE_D])
            camera->translate(camera->m_position + camera->m_right * (CAMERA_MOVE_FACTOR * Global::dt));

        if (keys[SDL_SCANCODE_UP])
            camera->pitch(-CAMERA_ROTATION_FACTOR * Global::dt);
        if (keys[SDL_SCANCODE_DOWN])
            camera->pitch(CAMERA_ROTATION_FACTOR * Global::dt);
        if (keys[SDL_SCANCODE_LEFT])
            camera->roll(-CAMERA_ROTATION_FACTOR * Global::dt);
        if (keys[SDL_SCANCODE_RIGHT])
            camera->roll(CAMERA_ROTATION_FACTOR * Global::dt);
        if (keys[SDL_SCANCODE_Q])
            camera->yaw(CAMERA_ROTATION_FACTOR * Global::dt);
        if (keys[SDL_SCANCODE_E])
            camera->yaw(-CAMERA_ROTATION_FACTOR * Global::dt);

        if ((SDL_GetModState() & KMOD_SHIFT) != KMOD_NONE && keys[SDL_SCANCODE_PERIOD] &&
            is_available(SDL_SCANCODE_PERIOD))
            m_current_scene->update_lighting(m_current_scene->m_point_light_count
                < (m_current_scene_type == IMMEDIATE ? MAX_POINT_LIGHTS_IMMEDIATE : MAX_POINT_LIGHTS_MODERN)
                ? m_current_scene->m_point_light_count + 1
                : m_current_scene->m_point_light_count);
        if ((SDL_GetModState() & KMOD_SHIFT) != KMOD_NONE && keys[SDL_SCANCODE_COMMA] &&
            is_available(SDL_SCANCODE_COMMA))
            m_current_scene->update_lighting(m_current_scene->m_point_light_count > 0
                ? m_current_scene->m_point_light_count - 1
                : m_current_scene->m_point_light_count);

        if (keys[SDL_SCANCODE_1])
            load_scene(IMMEDIATE);
        if (keys[SDL_SCANCODE_2])
            load_scene(MODERN);
        if (keys[SDL_SCANCODE_3])
            load_scene(INSTANCED);
        if (keys[SDL_SCANCODE_4])
            load_scene(INSTANCED_GLSL);
        if (keys[SDL_SCANCODE_5])
            load_scene(ANIMATED);
        if (keys[SDL_SCANCODE_6])
            load_scene(ANIMATED_GLSL);

        if (keys[SDL_SCANCODE_H] && is_available(SDL_SCANCODE_H))
            m_osd_full_mode = !m_osd_full_mode;
        if (keys[SDL_SCANCODE_L] && is_available(SDL_SCANCODE_L))
            m_current_scene->toggle_lighting();
        if (keys[SDL_SCANCODE_C] && is_available(SDL_SCANCODE_C))
            m_current_scene->toggle_backface_culling();
        if (keys[SDL_SCANCODE_Z] && is_available(SDL_SCANCODE_Z))
            m_current_scene->toggle_depth_buffering();

        if (keys[SDL_SCANCODE_KP_PLUS] && is_available(SDL_SCANCODE_KP_PLUS))
            m_current_scene->update_subdivision(m_current_scene->m_subdivision
                < (m_current_scene_type == IMMEDIATE ? MAX_SUBDIVISION_LEVEL_IMMEDIATE : MAX_SUBDIVISION_LEVEL_MODERN)
                ? m_current_scene->m_subdivision + 1
                : m_current_scene->m_subdivision);
        if (keys[SDL_SCANCODE_KP_MINUS] && is_available(SDL_SCANCODE_KP_MINUS))
            m_current_scene->update_subdivision(m_current_scene->m_subdivision > 1
                ? m_current_scene->m_subdivision - 1
                : m_current_scene->m_subdivision);
    }
}

bool App::is_available(SDL_Scancode key) {
    try {
        if (m_key_timers.at(key) == 0.0f) {
            m_key_timers.at(key) = 1.0f;
            return true;
        }
    } catch (std::out_of_range& e) {
        m_key_timers.insert({ key, 1.0f });
        return true;
    }

    return false;
}

bool App::tick() {
    m_quit = false;

    handle_input();
    update();
    render();

    return m_quit;
}

void App::update() {
    m_current_scene->update();

    for (auto&[key, timer]: m_key_timers) {
        timer -= Global::dt * 5.0f;
        timer = timer < 0.0f ? 0.0f : timer;
    }
}

void App::done() {
    load_scene(NONE);

    gltTerminate();
    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(m_sdl_window);
    SDL_Quit();

    printf("Average framerate: %d\n", (int) (m_framerate_accum / m_framerate_iterations));

    m_gl_context = nullptr;
    m_sdl_window = nullptr;
}
