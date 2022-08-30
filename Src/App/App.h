#ifndef MENGER_SPONGE_APP_H
#define MENGER_SPONGE_APP_H

#include "../Scene/Scene_Immediate.h"
#include "../Scene/Scene_Modern.h"
#include "../Scene/Scene_Instanced.h"
#include "../Scene/Scene_Instanced_GLSL.h"
#include "../Scene/Scene_Animated.h"
#include "../Scene/Scene_Animated_GLSL.h"

#include <glad/glad.h>

#define SDL_MAIN_HANDLED
#include <sdl/SDL.h>

#define GLT_IMPLEMENTATION
#include <gltext/gltext.h>

#include <iostream>
#include <array>
#include <unordered_map>

#define APP_SUCCESS EXIT_SUCCESS
#define APP_FAILURE EXIT_FAILURE

enum Scene_Type {
    IMMEDIATE, MODERN, INSTANCED, INSTANCED_GLSL, ANIMATED, ANIMATED_GLSL, NONE
};

class App {
public:
    explicit App(const char* title, bool fullscreen = false, int width = 1280, int height = 720);

    int init();

    void load_scene(Scene_Type type);

    void run();

    void render();

    void render_osd();

    void handle_input();

    bool is_available(SDL_Scancode key);

    bool tick();

    void update();

    void done();

protected:
    const char* m_window_title;
    bool m_fullscreen;
    int m_window_width;
    int m_window_height;
    int m_screen_width{};
    int m_screen_height{};
    SDL_Window* m_sdl_window{};
    SDL_GLContext m_gl_context{};

    std::shared_ptr<Scene_Immediate> scene_immediate;
    std::shared_ptr<Scene_Modern> scene_modern;
    std::shared_ptr<Scene_Instanced> scene_instanced;
    std::shared_ptr<Scene_Instanced_GLSL> scene_instanced_glsl;
    std::shared_ptr<Scene_Animated> scene_animated;
    std::shared_ptr<Scene_Animated_GLSL> scene_animated_glsl;
    std::shared_ptr<Scene> m_current_scene;
    Scene_Type m_current_scene_type{ NONE };

    bool m_quit{};
    bool m_osd_full_mode{};
    float m_framerate{};
    unsigned int m_elapsed_frames{};
    double m_framerate_accum{};
    unsigned long int m_framerate_iterations{};
    std::unordered_map<SDL_Scancode, float> m_key_timers;
};

#endif // MENGER_SPONGE_APP_H
