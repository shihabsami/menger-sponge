#include "Scene_Animated.h"

#include "../Main/Global.h"
#include <glm/gtx/compatibility.hpp>

#include <algorithm>

void Scene_Animated::init() {
    Scene_Modern::init(
        R"(Src/Shader/Vertex.glsl)",
        R"(Src/Shader/Geometry_Animated.glsl)",
        R"(Src/Shader/Fragment.glsl)"
        );

    m_name = "Scene 5 (CPU Animated Geometry)";

    update_data();
}

//float ease_in_out_elastic(float t) {
//    float c = (2 * M_PI) / 4.5f;
//
//    t = t == 0.0f
//        ? 0.0f
//        : t == 1.0f
//            ? 1.0f
//            : t < 0.5
//                ? -(powf(2, 20 * t - 10) * sinf((20 * t - 11.125f) * c)) / 2
//                : (powf(2, -20 * t + 10) * sinf((20 * t - 11.125f) * c)) / 2 + 1;
//
//    float t_min = -0.116;
//    float t_max = 1.116;
//    return (t - t_min) / (t_max - t_min);
//}

float ease_in_out_quint(float t) {
    return t < 0.5f
        ? 16.0f * t * t * t * t * t
        : 1 - powf(-2 * t + 2, 5.0f) / 2.0f;
}

void Scene_Animated::update() {
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

    for (size_t i = 0; i < m_mesh->m_data->size(); i++)
        m_mesh->m_data->at(i) =
            glm::lerp(m_data_raw->at(i), m_data_morphed->at(i), abs(ease_in_out_quint(keyframe)));

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        (signed) (sizeof(glm::vec3) * m_mesh->m_data->size()), m_mesh->m_data->data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Scene_Animated::update_subdivision(unsigned int subdivision) {
    Scene_Modern::update_subdivision(subdivision);

    update_data();
}

void Scene_Animated::update_data() {
    m_data_raw.reset();
    m_data_morphed.reset();
    m_data_raw = std::make_unique<std::vector<glm::vec3>>(*m_mesh->m_data);
    m_data_morphed = std::make_unique<std::vector<glm::vec3>>(*m_mesh->m_data);

    /*
     * Algorithm
     * 1. the ratio to scale by to get the intersection point
     * 2. the intersection point, one of the x/y/z component = 0.5f
     * 3. the percentage this vector is if the intersection point is at 100%
     * 4. ratio of the scaled radius against the current length of the vector
     * 5. scale vector down by the ratio to match the radius
     */

    float max_radius = 0.5f;
    for (auto &v : *m_data_morphed) {
        float ratio_to_max = 0.5f / std::max({ abs(v.x), abs(v.y), abs(v.z) });
        glm::vec3 intersection = v * ratio_to_max;
        float percentage = glm::length(intersection) / glm::length(v);
        float scale = (max_radius / percentage) / glm::length(v);
        v *= scale;
    }
}

void Scene_Animated::done() {
    m_data_raw.reset();
    m_data_morphed.reset();

    Scene_Modern::done();
}
