#ifndef MENGER_SPONGE_LIGHT_H
#define MENGER_SPONGE_LIGHT_H

#include <glm/gtc/type_ptr.hpp>

#include "Material.h"

class GL_Light {
public:
    GL_Light(const Material& material, const glm::vec4& position)
        : m_material(material), m_position(position) {}

    GL_Light(GLenum id, const Material& material, const glm::vec4& position)
        : m_id(id), m_material(material), m_position(position) {}

    void enable() {
        if (!m_enabled) {
            glLightfv(m_id, GL_AMBIENT, glm::value_ptr(m_material.m_ambient));
            glLightfv(m_id, GL_DIFFUSE, glm::value_ptr(m_material.m_diffuse));
            glLightfv(m_id, GL_SPECULAR, glm::value_ptr(m_material.m_specular));
            glLightfv(m_id, GL_POSITION, glm::value_ptr(m_position));

            glEnable(m_id);
            m_enabled = true;
        }
    }

    void disable() {
        if (m_enabled) {
            glDisable(m_id);
            m_enabled = false;
        }
    }

    void set_position(const glm::vec4& position) {
        m_position = position;
        glLightfv(m_id, GL_POSITION, glm::value_ptr(m_position));
    }

    GLenum m_id{};
    bool m_enabled{};
    Material m_material;
    glm::vec4 m_position;
};

class Directional_Light {
public:
    Directional_Light(
        const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse,const glm::vec3& specular)
        : m_direction(direction), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular) {}

    glm::vec3 m_direction;

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
};

class Point_Light {
public:
    Point_Light(const glm::vec3& position, float constant, float linear, float quadratic, const glm::vec3& ambient,
        const glm::vec3& diffuse, const glm::vec3& specular)
        : m_position(position), m_constant(constant), m_linear(linear), m_quadratic(quadratic), m_ambient(ambient),
        m_diffuse(diffuse), m_specular(specular) {}

    glm::vec3 m_position;

    float m_constant;
    float m_linear;
    float m_quadratic;

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
};

#endif // MENGER_SPONGE_LIGHT_H
