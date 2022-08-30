#ifndef MENGER_SPONGE_CAMERA_H
#define MENGER_SPONGE_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

constexpr glm::vec3 VEC3_FORWARD{ 0.0f, 0.0f, -1.0f };
constexpr glm::vec3 VEC3_RIGHT{ 1.0f, 0.0f, 0.0f };
constexpr glm::vec3 VEC3_UP{ 0.0f, 1.0f, 0.0f };
constexpr glm::quat QUAT_IDENTITY{ 1.0f, 0.0f, 0.0f, 0.0f };
constexpr glm::mat4 MAT4_IDENTITY{ 1.0f };

class Camera {
public:
    Camera();

    void translate(const glm::vec3& position);

    void rotate(const glm::quat& orientation);

    void update();

    void pitch(float angle);

    void roll(float angle);

    void yaw(float angle);

    glm::vec3 m_position{};
    glm::vec3 m_forward;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::quat m_orientation;
    glm::mat4 m_view;
    float t{};
};

#endif // MENGER_SPONGE_CAMERA_H
