#ifndef MENGER_SPONGE_SCENE_INSTANCED_H
#define MENGER_SPONGE_SCENE_INSTANCED_H

#include "Scene_Modern.h"
#include "../Component/Shader.h"

class Scene_Instanced : public Scene_Modern {
public:
    void init() override;

    virtual void init_transforms();

    void display() override;

    std::array<std::tuple<glm::vec3, glm::vec3, float>, 9> m_transforms;
    std::array<glm::mat4, 9> m_matrices;
};

#endif // MENGER_SPONGE_SCENE_INSTANCED_H
