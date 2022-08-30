#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;

in mat4 gs_model_matrix[];

out vec3 fs_position;
out vec3 fs_normal;
flat out uint fs_material_id;

void main() {
    vec3 a = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 b = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 normal = normalize(cross(a, b));

    for (int i = 0; i < gl_in.length(); i++) {
        if (abs(normal.x) > 0.0) {
            fs_material_id = 0;
        } else if (abs(normal.y) > 0.0) {
            fs_material_id = 1;
        } else {
            fs_material_id = 2;
        }

        fs_position = vec3(gs_model_matrix[0] * gl_in[i].gl_Position);
        fs_normal = normal; // mat3(transpose(inverse(gs_model_matrix[0])))

        gl_Position = projection_matrix * view_matrix * gs_model_matrix[0] * gl_in[i].gl_Position;

        EmitVertex();
    }

    EndPrimitive();
}
