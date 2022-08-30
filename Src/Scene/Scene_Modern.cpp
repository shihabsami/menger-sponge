#include "Scene_Modern.h"

#include "../Main/Global.h"
#include "../Main/Debug.h"

#include <algorithm>

void Scene_Modern::init() {
    Scene::init();

    m_name = "Scene 2 (Static Geometry with Modern Shaders)";
    m_lighting_enabled = true;

    init_buffers();
    init_lights();
    init_shaders(
        R"(Src/Shader/Vertex.glsl)",
        R"(Src/Shader/Geometry.glsl)",
        R"(Src/Shader/Fragment.glsl)");
}

void Scene_Modern::init(const char* vs_path, const char* gs_path, const char* fs_path) {
    Scene::init();

    m_name = "Scene 2 (Static Geometry with MODERN Shaders)";
    m_lighting_enabled = true;

    init_buffers();
    init_lights();
    init_shaders(vs_path, gs_path, fs_path);
}

void Scene_Modern::init_buffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    bind_buffers();
}

void Scene_Modern::bind_buffers() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        (signed) (sizeof(glm::vec3) * m_mesh->m_data->size()), m_mesh->m_data->data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        (signed) (sizeof(GLsizei) * m_mesh->m_indices->size()), m_mesh->m_indices->data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*) nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Scene_Modern::init_shaders(const char* vs_path, const char* gs_path, const char* fs_path) {
    m_shader = std::make_unique<Shader>(vs_path, gs_path, fs_path);
    m_shader->use();

    m_shader->set_mat4("model_matrix", *m_mesh->m_model_matrix);
    m_shader->set_mat4("view_matrix", m_camera->m_view);
    m_shader->set_mat4("projection_matrix", *m_projection_matrix);
    m_shader->set_mat3("normal_matrix",
        glm::mat3(glm::transpose(glm::inverse(*m_mesh->m_model_matrix))));

    m_shader->set_vec3("view_position", m_camera->m_position);

    m_shader->set_material("materials[0]", Material_Library::gold);
    m_shader->set_material("materials[1]", Material_Library::turquoise);
    m_shader->set_material("materials[2]", Material_Library::copper);

    m_shader->set_directional_light("d_light", *m_directional_light);
    for (int i = 0; i < MAX_POINT_LIGHTS_MODERN; i++)
        m_shader->set_point_light(
            std::string("p_lights[") + std::to_string(i) + "]", *m_point_lights.at(i));

    m_shader->set_bool("lighting_enabled", m_lighting_enabled);
    m_shader->set_uint("point_light_count", m_point_light_count);
}

void Scene_Modern::init_lights() {
    m_directional_light = std::make_unique<Directional_Light>(
        m_camera->m_forward,
        Material_Library::directional_light.m_ambient,
        Material_Library::directional_light.m_diffuse,
        Material_Library::directional_light.m_specular
    );

    std::vector<glm::vec3> point_light_positions;
    point_light_positions.emplace_back(0.0f, 0.0f, 5.0f);
    point_light_positions.emplace_back(-5.0f, 0.0f, 0.0f);
    point_light_positions.emplace_back(0.0f, 0.0f, -5.0f);
    point_light_positions.emplace_back(5.0f, 0.0f, 0.0f);
    point_light_positions.emplace_back(0.0f, 5.0f, 0.0f);
    point_light_positions.emplace_back(0.0f, 0.0f, 0.0f);
    point_light_positions.emplace_back(0.0f, -5.0f, 0.0f);
    point_light_positions.emplace_back(-5.0f, -5.0f, 5.0f);
    point_light_positions.emplace_back(-5.0f, -5.0f, -5.0f);
    point_light_positions.emplace_back(5.0f, -5.0f, -5.0f);
    point_light_positions.emplace_back(5.0f, -5.0f, 5.0f);
    point_light_positions.emplace_back(-5.0f, 5.0f, 5.0f);
    point_light_positions.emplace_back(-5.0f, 5.0f, -5.0f);
    point_light_positions.emplace_back(5.0f, 5.0f, -5.0f);
    point_light_positions.emplace_back(5.0f, 5.0f, 5.0f);
    point_light_positions.emplace_back(0.0f, 0.0f, 0.0f);


    for (int i = 0; i < MAX_POINT_LIGHTS_MODERN; i++) {
        m_point_lights.at(i) = std::make_unique<Point_Light>(
            point_light_positions.at(i),
            1.0f, 0.09f, 0.032f,
            Material_Library::point_light.m_ambient,
            Material_Library::point_light.m_diffuse,
            Material_Library::point_light.m_specular);
    }
}

void Scene_Modern::display() {
    m_shader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (signed) m_mesh->m_indices->size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Scene_Modern::update() {
    Scene::update();

    m_shader->use();
    m_shader->set_mat4("view_matrix", m_camera->m_view);
    m_shader->set_vec3("view_position", m_camera->m_position);
    m_shader->set_vec3("d_light.direction", m_camera->m_forward);
}

void Scene_Modern::toggle_lighting() {
    m_shader->use();
    m_lighting_enabled = !m_lighting_enabled;
    m_shader->set_bool("lighting_enabled", m_lighting_enabled);
}

void Scene_Modern::update_lighting(unsigned int point_light_count) {
    Scene::update_lighting(point_light_count);

    m_shader->use();
    m_shader->set_uint("point_light_count", m_point_light_count);
}

void Scene_Modern::update_subdivision(unsigned int subdivision) {
    Scene::update_subdivision(subdivision);

    bind_buffers();
}

void Scene_Modern::done() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    m_shader.reset();
    m_directional_light.reset();
    for (int i = 0; i < m_point_light_count; i++)
        m_point_lights.at(i).reset();

    Scene::done();
}
