#ifndef MENGER_SPONGE_SCENE_ANIMATED_H
#define MENGER_SPONGE_SCENE_ANIMATED_H

#include "Scene_Modern.h"
#include "../Component/Shader.h"

class Scene_Animated : public Scene_Modern {
public:
    void init() override;

    void update() override;

    void update_subdivision(unsigned int subdivision) override;

    void update_data();

    void done() override;

    float keyframe{};
    bool ascending{ true };
    std::unique_ptr<std::vector<glm::vec3>> m_data_raw;
    std::unique_ptr<std::vector<glm::vec3>> m_data_morphed;
};

#endif // MENGER_SPONGE_SCENE_ANIMATED_H
