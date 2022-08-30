#ifndef MENGER_SPONGE_SCENE_INSTANCED_GLSL_H
#define MENGER_SPONGE_SCENE_INSTANCED_GLSL_H

#include "Scene_Instanced.h"
#include "../Component/Shader.h"

class Scene_Instanced_GLSL : public Scene_Instanced {
public:
    void init() override;

    void init_buffers() override;

    void bind_buffers() override;

    void display() override;

    void done() override;

    GLuint transforms_VBO{};

};

#endif // MENGER_SPONGE_SCENE_INSTANCED_GLSL_H
