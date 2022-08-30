#include "Scene_Instanced_GLSL.h"

#include "../Main/Global.h"

void Scene_Instanced_GLSL::init() {
    Scene_Instanced::Scene_Modern::init(
        R"(Src/Shader/Vertex_Instanced.glsl)",
        R"(Src/Shader/Geometry_Instanced.glsl)",
        R"(Src/Shader/Fragment.glsl)"
    );

    Scene_Instanced::init_transforms();

    m_name = "Scene 4 (Instanced Geometry 2)";
    m_camera->translate(glm::vec3{ 0.0f, 2.0f, 6.0f });
}

void Scene_Instanced_GLSL::init_buffers() {
    glGenBuffers(1, &transforms_VBO);

    Scene_Modern::init_buffers();

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, transforms_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * 9, m_matrices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, transforms_VBO);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) nullptr);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (1 * 4 * sizeof(GLfloat)));
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (2 * 4 * sizeof(GLfloat)));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (3 * 4 * sizeof(GLfloat)));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Scene_Instanced_GLSL::bind_buffers() {
    Scene_Modern::bind_buffers();

    glBindBuffer(GL_ARRAY_BUFFER, transforms_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * 9, m_matrices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Scene_Instanced_GLSL::display() {
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
    }

    glBindBuffer(GL_ARRAY_BUFFER, transforms_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * 9, m_matrices.data(), GL_DYNAMIC_DRAW);

    glDrawElementsInstanced(GL_TRIANGLES, (signed) m_mesh->m_indices->size(), GL_UNSIGNED_INT, nullptr, 9);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Scene_Instanced_GLSL::done() {
    glDeleteBuffers(1, &transforms_VBO);

    Scene_Modern::done();
}
