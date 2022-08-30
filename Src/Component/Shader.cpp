#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(const char* vs_path, const char* gs_path, const char* fs_path) {
    std::string vs_source_str, gs_source_str, fs_source_str;
    std::ifstream vs_file, gs_file, fs_file;

    // enable throwing the specified exceptions
    vs_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gs_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fs_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vs_file.open(vs_path);
        gs_file.open(gs_path);
        fs_file.open(fs_path);

        // read the source code into stream
        std::stringstream vs_stream, gs_stream, fs_stream;
        vs_stream << vs_file.rdbuf();
        gs_stream << gs_file.rdbuf();
        fs_stream << fs_file.rdbuf();

        vs_file.close();
        gs_file.close();
        fs_file.close();

        vs_source_str = vs_stream.str();
        gs_source_str = gs_stream.str();
        fs_source_str = fs_stream.str();
    }
    catch (const std::ifstream::failure& e) {
        fprintf(stderr, "ERROR: Failed to read Shader file: %s\n", e.what());
    }

    const char* vs_source = vs_source_str.c_str();
    const char* gs_source = gs_source_str.c_str();
    const char* fs_source = fs_source_str.c_str();

    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, nullptr);
    glCompileShader(vs);
    check_errors(vs, VERTEX_SHADER);

    unsigned int gs = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(gs, 1, &gs_source, nullptr);
    glCompileShader(gs);
    check_errors(gs, GEOMETRY_SHADER);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, nullptr);
    glCompileShader(fs);
    check_errors(fs, FRAGMENT_SHADER);

    id = glCreateProgram();
    glAttachShader(id, vs);
    glAttachShader(id, gs);
    glAttachShader(id, fs);
    glLinkProgram(id);
    check_errors(id, SHADER_PROGRAM);

    glDeleteShader(vs);
    glDeleteShader(gs);
    glDeleteShader(fs);
}

Shader::~Shader() { glDeleteProgram(id); }

void Shader::use() const { glUseProgram(id); }

void Shader::check_errors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar info_log[1024];
    if (type != SHADER_PROGRAM) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, info_log);
            fprintf(stderr, "ERROR: Failed to compile %s: %s\n", type.c_str(), info_log);
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, info_log);
            fprintf(stderr, "ERROR: Failed to link %s: %s\n", info_log, type.c_str());
        }
    }
}

void Shader::set_uint(const char* label, unsigned int ui) const {
    glUniform1ui(glGetUniformLocation(id, label), ui);
}

void Shader::set_float(const char* label, float f) const {
    glUniform1f(glGetUniformLocation(id, label), f);
}

void Shader::set_vec3(const char* label, const glm::vec3& v) const {
    glUniform3fv(glGetUniformLocation(id, label), 1, glm::value_ptr(v));
}

void Shader::set_mat3(const char* label, const glm::mat3& m) const {
    glUniformMatrix3fv(glGetUniformLocation(id, label), 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::set_mat4(const char* label, const glm::mat4& m) const {
    glUniformMatrix4fv(glGetUniformLocation(id, label), 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::set_material(const std::string& label, const Material& material) const {
    set_vec3((label + ".ambient").c_str(), material.m_ambient);
    set_vec3((label + ".diffuse").c_str(), material.m_diffuse);
    set_vec3((label + ".specular").c_str(), material.m_specular);
    set_float((label + ".shininess").c_str(), material.m_shininess);
}

void Shader::set_directional_light(const std::string& label, const Directional_Light& light) const {
    set_vec3((label + ".direction").c_str(), light.m_direction);

    set_vec3((label + ".ambient").c_str(), light.m_ambient);
    set_vec3((label + ".diffuse").c_str(), light.m_diffuse);
    set_vec3((label + ".specular").c_str(), light.m_specular);
}

void Shader::set_point_light(const std::string& label, const Point_Light& light) const {
    set_vec3((label + ".position").c_str(), light.m_position);

    set_float((label + ".constant").c_str(), light.m_constant);
    set_float((label + ".linear").c_str(), light.m_linear);
    set_float((label + ".quadratic").c_str(), light.m_quadratic);

    set_vec3((label + ".ambient").c_str(), light.m_ambient);
    set_vec3((label + ".diffuse").c_str(), light.m_diffuse);
    set_vec3((label + ".specular").c_str(), light.m_specular);
}

void Shader::set_bool(const char* label, bool b) const {
    glUniform1i(glGetUniformLocation(id, label), b);
}
