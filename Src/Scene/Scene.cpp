#include <algorithm>
#include "Scene.h"

#include "../Main/Global.h"

void Scene::init() {
    m_name = "Scene (Base)";
    m_camera = std::make_unique<Camera>();
    m_camera->translate(glm::vec3{ 0.0f, 0.0f, 2.0f });
    m_mesh = std::make_unique<Mesh>();
    m_projection_matrix = std::make_unique<glm::mat4>(glm::perspective(PROJECTION_FOV,
        (float) Global::window_width / (float) Global::window_height, PROJECTION_Z_NEAR, PROJECTION_Z_FAR)
    );

    m_backface_culling_enabled = true;
    m_depth_buffering_enabled = true;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    m_mesh->init(m_subdivision);
}

void Scene::update() {
    m_camera->update();
}

void Scene::update_lighting(unsigned int point_light_count) {
    if (point_light_count != m_point_light_count)
        m_point_light_count = point_light_count;
}

void Scene::update_subdivision(unsigned int subdivision) {
    if (subdivision != m_subdivision) {
        m_subdivision = subdivision;
        m_mesh->clear();
        m_mesh->init(m_subdivision);
    }
}

void Scene::toggle_lighting() {
    if (!m_lighting_enabled) {
        glEnable(GL_LIGHTING);
        m_lighting_enabled = true;
    } else {
        glDisable(GL_LIGHTING);
        m_lighting_enabled = false;
    }
}

void Scene::toggle_backface_culling() {
    if (!m_backface_culling_enabled) {
        glEnable(GL_CULL_FACE);
        m_backface_culling_enabled = true;
    } else {
        glDisable(GL_CULL_FACE);
        m_backface_culling_enabled = false;
    }
}

void Scene::toggle_depth_buffering() {
    if (!m_depth_buffering_enabled) {
        glEnable(GL_DEPTH_TEST);
        m_depth_buffering_enabled = true;
    } else {
        glDisable(GL_DEPTH_TEST);
        m_depth_buffering_enabled = false;
    }
}

void Scene::done() {
    m_camera.reset();
    m_mesh.reset();
    m_projection_matrix.reset();
}
