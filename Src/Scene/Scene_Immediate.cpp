#include <cstdio>
#include "Scene_Immediate.h"

#include "../Main/Global.h"

void Scene_Immediate::init() {
    Scene::init();

    m_name = "Scene 1 (Static Geometry with Immediate Mode)";

    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(*m_projection_matrix));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(m_camera->m_view * *m_mesh->m_model_matrix));

    m_directional_light = std::make_unique<GL_Light>(GL_LIGHT0, Material_Library::directional_light,
        glm::vec4{ -m_camera->m_forward.x, -m_camera->m_forward.y, -m_camera->m_forward.z, 0.0f });
    m_directional_light->enable();

    m_point_lights.at(0) = std::make_unique<GL_Light>(GL_LIGHT1, Material_Library::point_light,
        glm::vec4{ 0.0f, 0.0f, 5.0f, 1.0f });
    m_point_lights.at(1) = std::make_unique<GL_Light>(GL_LIGHT2, Material_Library::point_light,
        glm::vec4{ -5.0f, 0.0f, 0.0f, 1.0f });
    m_point_lights.at(2) = std::make_unique<GL_Light>(GL_LIGHT3, Material_Library::point_light,
        glm::vec4{ 0.0f, 0.0f, -5.0f, 1.0f });
    m_point_lights.at(3) = std::make_unique<GL_Light>(GL_LIGHT4, Material_Library::point_light,
        glm::vec4{ 5.0f, 0.0f, 0.0f, 1.0f });
    m_point_lights.at(4) = std::make_unique<GL_Light>(GL_LIGHT5, Material_Library::point_light,
        glm::vec4{ 0.0f, 5.0f, 0.0f, 1.0f });
    m_point_lights.at(5) = std::make_unique<GL_Light>(GL_LIGHT6, Material_Library::point_light,
        glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
    m_point_lights.at(6) = std::make_unique<GL_Light>(GL_LIGHT7, Material_Library::point_light,
        glm::vec4{ 0.0f, -5.0f, 0.0f, 1.0f });
    m_lighting_enabled = true;
}

void Scene_Immediate::update() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(m_camera->m_view * *m_mesh->m_model_matrix));
    m_directional_light->set_position(
        glm::vec4{ -m_camera->m_forward.x, -m_camera->m_forward.y, -m_camera->m_forward.z, 0.0f });
}

void Scene_Immediate::update_lighting(unsigned int point_light_count) {
    if (point_light_count == m_point_light_count)
        return;

    for (int i = 0; i < m_point_light_count; i++)
        m_point_lights.at(i)->disable();

    m_point_light_count = point_light_count;
    for (int i = 0; i < m_point_light_count; i++)
        m_point_lights.at(i)->enable();
}

void Scene_Immediate::display() {
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < m_mesh->m_indices->size();) {
        glm::vec3& v0 = m_mesh->m_data->at(m_mesh->m_indices->at(i++));
        glm::vec3& v1 = m_mesh->m_data->at(m_mesh->m_indices->at(i++));
        glm::vec3& v2 = m_mesh->m_data->at(m_mesh->m_indices->at(i++));
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        if (m_lighting_enabled)
            glNormal3fv(glm::value_ptr(normal));

        if (abs(normal.x) > 0.0f) {
            if (!m_lighting_enabled)
                glColor3fv(glm::value_ptr(Material_Library::gold.m_ambient + Material_Library::gold.m_diffuse));
            else
                Material_Library::gold.gl_use();
        } else if (abs(normal.y) > 0.0f) {
            if (!m_lighting_enabled)
                glColor3fv(glm::value_ptr(Material_Library::turquoise.m_ambient + Material_Library::turquoise.m_diffuse));
            else
                Material_Library::turquoise.gl_use();
        } else {
            if (!m_lighting_enabled)
                glColor3fv(glm::value_ptr(Material_Library::copper.m_ambient + Material_Library::copper.m_diffuse));
            else
                Material_Library::copper.gl_use();
        }

        glVertex3fv(glm::value_ptr(v0));
        glVertex3fv(glm::value_ptr(v1));
        glVertex3fv(glm::value_ptr(v2));
    }
    glEnd();
}

void Scene_Immediate::done() {
    Scene::done();

    glDisable(m_directional_light->m_id);
    m_directional_light.reset();
    for (int i = 0; i < m_point_light_count; i++) {
        glDisable(m_point_lights.at(i)->m_id);
        m_point_lights.at(i).reset();
    }

    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
