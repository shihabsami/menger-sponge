#version 460 core

layout (location = 0) in vec3 attr_position;
layout (location = 1) in mat4 model_matrix;

out mat4 gs_model_matrix;

void main() {
    gl_Position = vec4(attr_position, 1.0);
    gs_model_matrix = model_matrix;
}
