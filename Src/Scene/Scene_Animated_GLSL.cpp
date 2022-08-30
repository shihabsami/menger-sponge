#include "Scene_Animated_GLSL.h"

#include "../Main/Global.h"

void Scene_Animated_GLSL::init() {
    Scene_Modern::init(
        R"(Src/Shader/Vertex_Animated.glsl)",
        R"(Src/Shader/Geometry_Animated.glsl)",
        R"(Src/Shader/Fragment.glsl)");

    m_name = "Scene 6 (GPU Animated Geometry)";
}

void Scene_Animated_GLSL::update() {
    Scene_Modern::update();

    if (ascending) {
        keyframe = keyframe < 1.0f ? keyframe + ANIMATION_KEYFRAME_FACTOR * Global::dt : keyframe;
        ascending = keyframe < 1.0f;
        keyframe = keyframe > 1.0f ? 1.0f : keyframe;
    } else {
        keyframe = keyframe > 0.0f ? keyframe - ANIMATION_KEYFRAME_FACTOR * Global::dt : keyframe;
        ascending = keyframe < 0.0f;
        keyframe = keyframe < 0.0f ? 0.0f : keyframe;
    }

    m_shader->use();
    m_shader->set_float("keyframe", keyframe);
}
