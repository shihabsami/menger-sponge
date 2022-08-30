#include "Scene_Instanced.h"

#include "../Main/Global.h"

#include <random>

void Scene_Instanced::init() {
    Scene_Modern::init();

    m_name = "Scene 3 (Instanced Geometry 1)";
    m_camera->translate(glm::vec3{ 0.0f, 2.0f, 6.0f });

    init_transforms();
}

void Scene_Instanced::init_transforms() {
    float spacing = 2.0f;
    float x = -spacing, y = -spacing, z = 0.0f;
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            y += spacing;
            x = -spacing;
        }

        std::random_device device;
        std::mt19937 generator(device());
        std::uniform_real_distribution distribution{ 0.0, 1.0 };

        m_transforms.at(i) = std::make_tuple(
            glm::vec3(x, y, z),
            glm::vec3{ distribution(generator), distribution(generator), distribution(generator) },
            (float) distribution(generator) * 360.0f);

        m_matrices.at(i) = glm::translate(
            glm::mat4{ 1.0f }, std::get<0>(m_transforms.at(i)));
        m_matrices.at(i) = glm::rotate(
            m_matrices.at(i),glm::radians(
                std::get<2>(m_transforms.at(i))), std::get<1>(m_transforms.at(i)));

        x += spacing;
    }
}

void Scene_Instanced::display() {
    m_shader->use();
    glBindVertexArray(VAO);
    for (int i = 0; i < 9; i++) {
        std::get<2>(m_transforms.at(i)) = glm::mod(
            std::get<2>(m_transforms.at(i)) + 90.0f * Global::dt, 360.0f);
        m_matrices.at(i) = glm::translate(
            glm::mat4{ 1.0f }, std::get<0>(m_transforms.at(i)));
        m_matrices.at(i) = glm::rotate(
            m_matrices.at(i), glm::radians(
                std::get<2>(m_transforms.at(i))), std::get<1>(m_transforms.at(i)));
        m_shader->set_mat4("model_matrix", m_matrices.at(i));

        glDrawElements(GL_TRIANGLES, (signed) m_mesh->m_indices->size(), GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}
