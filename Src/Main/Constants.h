#ifndef MENGER_SPONGE_CONSTANTS_H
#define MENGER_SPONGE_CONSTANTS_H

constexpr unsigned int FRAME_RATE_INTERVAL = 100;

constexpr float PROJECTION_FOV = 45.0f;
constexpr float PROJECTION_Z_NEAR = 0.1f;
constexpr float PROJECTION_Z_FAR = 100.0f;

constexpr float CAMERA_MOVE_FACTOR = 5.0f;
constexpr float CAMERA_ROTATION_FACTOR = 90.0f;
constexpr float MOUSE_SENSITIVITY = 0.001f;

constexpr unsigned int MAX_SUBDIVISION_LEVEL_IMMEDIATE = 5;
constexpr unsigned int MAX_SUBDIVISION_LEVEL_MODERN = 6;
constexpr unsigned int MAX_POINT_LIGHTS_IMMEDIATE = 7;
constexpr unsigned int MAX_POINT_LIGHTS_MODERN = 16;

constexpr float ANIMATION_KEYFRAME_FACTOR = 0.5f;

#endif // MENGER_SPONGE_CONSTANTS_H
