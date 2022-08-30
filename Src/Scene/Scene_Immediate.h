#ifndef MENGER_SPONGE_SCENE_IMMEDIATE_H
#define MENGER_SPONGE_SCENE_IMMEDIATE_H

#include "Scene.h"

class Scene_Immediate : public Scene {
public:
    void init() override;

    void display() override;

    void update() override;

    void update_lighting(unsigned int point_light_count) override;

    void done() override;

    std::unique_ptr<GL_Light> m_directional_light;
    std::array<std::unique_ptr<GL_Light>, MAX_POINT_LIGHTS_IMMEDIATE> m_point_lights;
};

#endif // MENGER_SPONGE_SCENE_IMMEDIATE_H
