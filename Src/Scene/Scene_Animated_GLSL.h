#ifndef MENGER_SPONGE_SCENE_ANIMATED_GLSL_H
#define MENGER_SPONGE_SCENE_ANIMATED_GLSL_H

#include "Scene_Modern.h"
#include "../Component/Shader.h"

class Scene_Animated_GLSL : public Scene_Modern {
public:
    void init() override;

    void update() override;

    float keyframe{};
    bool ascending{ true };
};

#endif // MENGER_SPONGE_SCENE_ANIMATED_GLSL_H
