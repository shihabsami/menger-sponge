#version 460 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Directional_Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Point_Light {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fs_position;
in vec3 fs_normal;
flat in uint fs_material_id;

out vec4 fs_color;

uniform Material materials[3];
uniform Directional_Light d_light;
uniform Point_Light p_lights[16];
uniform vec3 view_position;
uniform bool lighting_enabled;
uniform uint point_light_count;

vec3 calculate_directional_light(vec3 N, vec3 V) {
    vec3 ambient = d_light.ambient * materials[fs_material_id].ambient; // La Ma

    vec3 L = normalize(-d_light.direction); // light direction vectorm, L
    vec3 diffuse = d_light.diffuse * materials[fs_material_id].diffuse * max(dot(N, L), 0.0); // Ld Md (N . L)

    vec3 R = reflect(-L, N); // reflect direction, R
    float specular_coeff = pow(max(dot(V, R), 0.0), materials[fs_material_id].shininess); // (R . V)^n (n = shininess)
    vec3 specular = d_light.specular * materials[fs_material_id].specular * specular_coeff; // Ls Ms (R . V)^n

    return ambient + diffuse + specular;
}

vec3 calculate_point_lights(vec3 N, vec3 V) {
    vec3 final;
    for (int i = 0; i < point_light_count; i++) {
        vec3 ambient = p_lights[i].ambient * materials[fs_material_id].ambient;

        vec3 L = normalize(p_lights[i].position - fs_position);
        vec3 diffuse = p_lights[i].diffuse * materials[fs_material_id].diffuse * max(dot(N, L), 0.0);

        vec3 H = normalize(L + V); // halfway vector, L + V / (|L + V|)
        float specular_coeff = pow(max(dot(N, H), 0.0), materials[fs_material_id].shininess);
        vec3 specular = p_lights[i].specular * materials[fs_material_id].specular * specular_coeff;

        float distance = length(p_lights[i].position - fs_position);
        float attenuation = 1.0 / (p_lights[i].constant + p_lights[i].linear * distance + p_lights[i].quadratic * (distance * distance));

        final += ambient * attenuation + diffuse * attenuation + specular * attenuation;
    }

    return final;
}

void main() {
    if (lighting_enabled) {
        vec3 N = normalize(fs_normal); // normal vector, N
        vec3 V = normalize(view_position - fs_position); // view direction, V

        fs_color = vec4(calculate_directional_light(N, V) + calculate_point_lights(N, V), 1.0f);
    } else {
        fs_color = vec4(materials[fs_material_id].ambient + materials[fs_material_id].diffuse, 1.0f);
    }
}
