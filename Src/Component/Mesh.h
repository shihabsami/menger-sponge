#ifndef MENGER_SPONGE_MESH_H
#define MENGER_SPONGE_MESH_H

#include <vector>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Mesh {
public:
    Mesh();

    ~Mesh();

    void init(unsigned int level);

    void next_level(float x_min, float y_min, float z_min, float length, unsigned int current_level);

    void add_faces(float x_min, float y_min, float z_min, float length) const;

    void clear() const;

    std::unique_ptr<std::vector<glm::vec3>> m_data;
    std::unique_ptr<std::vector<GLsizei>> m_indices;
    std::unique_ptr<glm::mat4> m_model_matrix;
};

#endif // MENGER_SPONGE_MESH_H
