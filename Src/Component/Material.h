#ifndef MENGER_SPONGE_MATERIAL_H
#define MENGER_SPONGE_MATERIAL_H

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

class Material {
public:
    void gl_use() const {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(m_ambient));
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(m_diffuse));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(m_specular));
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
    }

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    GLfloat m_shininess;
};

#endif // MENGER_SPONGE_MATERIAL_H
