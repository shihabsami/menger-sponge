#version 460 core

layout (location = 0) in vec3 attr_position;

uniform float keyframe;

// various different interpolating functions, some may show artefacts as result values are < 0.0 or > 1.0
float ease_in_bounce(float t);
float ease_out_bounce(float t);
float ease_in_out_bounce(float t);
float ease_out_eastic(float t);
float ease_in_out_elastic(float t);
float ease_in_out_quint(float t);

void main() {
    float max_radius = 0.5;
    float ratio_to_max = 0.5 / max(abs(attr_position.x), max(abs(attr_position.y), abs(attr_position.z)));
    vec3 intersection = attr_position * ratio_to_max;
    float percentage = length(intersection) / length(attr_position);
    float scale = (max_radius / percentage) / length(attr_position);
    vec3 morphed_position = attr_position * scale;

    gl_Position = vec4(mix(attr_position, morphed_position, ease_in_out_elastic(keyframe)), 1.0);
}

float ease_in_bounce(float t) {
    return 1 - ease_out_bounce(1 - t);
}

float ease_out_bounce(float t) {
    float c1 = 7.5625;
    float c2 = 2.75;

    if (t < 1 / c2) {
        return c1 * t * t;
    } else if (t < 2 / c2) {
        return c1 * (t -= 1.5 / c2) * t + 0.75;
    } else if (t < 2.5 / c2) {
        return c1 * (t -= 2.25 / c2) * t + 0.9375;
    } else {
        return c1 * (t -= 2.625 / c2) * t + 0.984375;
    }
}

float ease_in_out_bounce(float t) {
    return t < 0.5
    ? (1 - ease_out_bounce(1 - 2 * t)) / 2
    : (1 + ease_out_bounce(2 * t - 1)) / 2;
}

float ease_out_eastic(float t) {
    float c = (2 * 3.1415926538) / 3;

    return t == 0.0
        ? 0.0
        : t == 1.0
            ? 1.0
            : pow(2, -10.0 * t) * sin((t * 10 - 0.75) * c) + 1.0;
}

float ease_in_out_elastic(float t) {
    float c = (2 * 3.1415926538) / 4.5;
        t = t == 0.0
            ? 0.0f
            : t == 1.0
                ? 1.0
                : t < 0.5
                    ? -(pow(2.0f, 20 * t - 10) * sin((20 * t - 11.125) * c)) / 2
                    : (pow(2.0f, -20 * t + 10) * sin((20 * t - 11.125) * c)) / 2 + 1;

    float t_min = -0.116;
    float t_max = 1.116;
//    return (t - t_min) / (t_max - t_min);
    return t;
}

float ease_in_out_quint(float t) {
    return t < 0.5
        ? 16 * t * t * t * t * t
        : 1 - pow(-2 * t + 2, 5.0) / 2.0;
}
