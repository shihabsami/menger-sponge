#ifndef MENGER_SPONGE_SCENE_MODERN_H
#define MENGER_SPONGE_SCENE_MODERN_H

#include "Scene.h"
#include "../Component/Shader.h"

class Scene_Modern : public Scene {
public:
    void init() override;

    virtual void init(const char* vs_path, const char* gs_path, const char* fs_path);

    virtual void init_buffers();

    virtual void bind_buffers();

    void init_shaders(const char* vs_path, const char* gs_path, const char* fs_path);

    void init_lights();

    void display() override;

    void update() override;

    void toggle_lighting() override;

    void update_lighting(unsigned int point_light_count) override;

    void update_subdivision(unsigned int subdivision) override;

    void done() override;

    GLuint VAO{}, VBO{}, EBO{};
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Directional_Light> m_directional_light;
    std::array<std::unique_ptr<Point_Light>, MAX_POINT_LIGHTS_MODERN> m_point_lights;
};

#endif // MENGER_SPONGE_SCENE_MODERN_H
