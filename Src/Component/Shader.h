#ifndef MENGER_SPONGE_SHADER_H
#define MENGER_SPONGE_SHADER_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Material.h"
#include "Light.h"

constexpr auto VERTEX_SHADER = "Vertex Shader";
constexpr auto GEOMETRY_SHADER = "Geometry Shader";
constexpr auto FRAGMENT_SHADER = "Fragment Shader";
constexpr auto SHADER_PROGRAM = "Shader Program";

class Shader {
public:
    Shader(const char* vs_path, const char* gs_path, const char* fs_path);

    ~Shader();

    void set_uint(const char* label, unsigned int i) const;

    void set_bool(const char* label, bool b) const;

    void set_float(const char* label, float f) const;

    void set_vec3(const char* label, const glm::vec3& v) const;

    void set_mat3(const char* label, const glm::mat3& m) const;

    void set_mat4(const char* label, const glm::mat4& m) const;

    void set_material(const std::string& label, const Material& material) const;

    void set_directional_light(const std::string& label, const Directional_Light& light) const;

    void set_point_light(const std::string& label, const Point_Light& light) const;

    void use() const;

    static void check_errors(GLuint shader, const std::string& type);

    unsigned int id;
};

#endif // MENGER_SPONGE_SHADER_H
