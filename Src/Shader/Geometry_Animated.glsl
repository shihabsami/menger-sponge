#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;

out vec3 fs_position;
out vec3 fs_normal;
flat out uint fs_material_id;

void main() {
    vec3 a = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 b = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 raw_normal = normalize(cross(a, b));

    for (int i = 0; i < gl_in.length(); i++) {
        vec3 normal = normalize(gl_in[i].gl_Position.xyz);

        float abs_x = abs(raw_normal.x);
        float abs_y = abs(raw_normal.y);
        float abs_z = abs(raw_normal.z);
        float max_component = max(max(abs_x, abs_y), abs_z);

        fs_material_id = 0;
        if (max_component == abs_y) {
            fs_material_id = 1;
        } else if (max_component == abs_z) {
            fs_material_id = 2;
        }

        fs_position = vec3(model_matrix * gl_in[i].gl_Position);
        fs_normal = normal_matrix * normal;
        gl_Position = projection_matrix * view_matrix * model_matrix * gl_in[i].gl_Position;

        EmitVertex();
    }

    EndPrimitive();
}
