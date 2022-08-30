#include "Mesh.h"

#include <glm/ext/matrix_transform.hpp>

Mesh::Mesh()
    :
    m_data(std::make_unique<std::vector<glm::vec3>>()),
    m_indices(std::make_unique<std::vector<GLsizei>>()),
    m_model_matrix(std::make_unique<glm::mat4>(1.0f)) {}

Mesh::~Mesh() {
    clear();

    m_data.reset();
    m_indices.reset();
    m_model_matrix.reset();
}

void Mesh::init(unsigned int level) {
    clear();

    float length = 1.0f;
    float x_min = -length / 2;
    float y_min = -length / 2;
    float z_min = -length / 2;

    if (level < 2)
        add_faces(x_min, y_min, z_min, length);
    else
        next_level(x_min, y_min, z_min, length / 3, level);
}

void Mesh::next_level(float x_min, float y_min, float z_min, float length, unsigned int current_level) {
    float x, y, z;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (i % 2 + j % 2 + k % 2 < 2) {
                    x = x_min + (float) i * length;
                    y = y_min + (float) j * length;
                    z = z_min + (float) k * length;
                    if (current_level > 2) {
                        float new_length = length / 3;
                        next_level(x, y, z, new_length, current_level - 1);
                    } else {
                        add_faces(x, y, z, length);
                    }
                }
            }
        }
    }
}

void Mesh::add_faces(float x_min, float y_min, float z_min, float length) const {
    float x_max = x_min + length;
    float y_max = y_min + length;
    float z_max = z_min + length;

    size_t i = m_data->size();

    m_data->emplace_back(x_min, y_min, z_max); // v0
    m_data->emplace_back(x_max, y_min, z_max); // v1
    m_data->emplace_back(x_max, y_max, z_max); // v2
    m_data->emplace_back(x_min, y_max, z_max); // v3
    m_data->emplace_back(x_min, y_min, z_min); // v4
    m_data->emplace_back(x_max, y_min, z_min); // v5
    m_data->emplace_back(x_max, y_max, z_min); // v6
    m_data->emplace_back(x_min, y_max, z_min); // v7

    // right facing, bottom right
    m_indices->emplace_back(i + 1);
    m_indices->emplace_back(i + 5);
    m_indices->emplace_back(i + 2);

    // right facing, top left
    m_indices->emplace_back(i + 2);
    m_indices->emplace_back(i + 5);
    m_indices->emplace_back(i + 6);

    // left facing, bottom right
    m_indices->emplace_back(i + 3);
    m_indices->emplace_back(i + 4);
    m_indices->emplace_back(i);

    // left facing, top left
    m_indices->emplace_back(i + 7);
    m_indices->emplace_back(i + 4);
    m_indices->emplace_back(i + 3);

    // top facing, bottom right
    m_indices->emplace_back(i + 3);
    m_indices->emplace_back(i + 2);
    m_indices->emplace_back(i + 6);

    // top facing, top left
    m_indices->emplace_back(i + 6);
    m_indices->emplace_back(i + 7);
    m_indices->emplace_back(i + 3);

    // bottom facing, bottom right
    m_indices->emplace_back(i + 5);
    m_indices->emplace_back(i + 1);
    m_indices->emplace_back(i + 0);

    // bottom facing, top left
    m_indices->emplace_back(i + 0);
    m_indices->emplace_back(i + 4);
    m_indices->emplace_back(i + 5);

    // front facing, bottom right
    m_indices->emplace_back(i + 0);
    m_indices->emplace_back(i + 1);
    m_indices->emplace_back(i + 2);

    // front facing, top left
    m_indices->emplace_back(i + 0);
    m_indices->emplace_back(i + 2);
    m_indices->emplace_back(i + 3);

    // back facing, bottom right
    m_indices->emplace_back(i + 6);
    m_indices->emplace_back(i + 5);
    m_indices->emplace_back(i + 4);

    // back facing, top left
    m_indices->emplace_back(i + 7);
    m_indices->emplace_back(i + 6);
    m_indices->emplace_back(i + 4);
}

void Mesh::clear() const {
    m_data->clear();
    m_indices->clear();
}
