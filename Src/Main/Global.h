#ifndef MENGER_SPONGE_GLOBAL_H
#define MENGER_SPONGE_GLOBAL_H

#include "../Component/Material.h"

namespace Global {
    extern int window_width;
    extern int window_height;
    extern float dt;
}

namespace Material_Library {
    constexpr Material red{
        { 0.5f, 0.1f, 0.1f },
        { 0.8f, 0.1f, 0.1f },
        { 0.8f, 0.8f, 0.8f },
        128.0f };

    constexpr Material green{
        { 0.1f, 0.5f, 0.1f },
        { 0.0f, 0.8f, 0.0f },
        { 0.8f, 0.8f, 0.8f },
        128.0f };

    constexpr Material blue{
        { 0.1f, 0.1f, 0.5f },
        { 0.0f, 0.0f, 0.8f },
        { 0.8f, 0.8f, 0.8f },
        128.0f };

    constexpr Material gray{
        { 0.8f, 0.8f, 0.8f },
        { 0.8f, 0.8f, 0.8f },
        { 0.0f, 0.0f, 0.0f },
        0.0f };

    constexpr Material directional_light{
        { 0.5f, 0.5f, 0.5f },
        { 0.5f, 0.5f, 0.5f },
        { 0.5f, 0.5f, 0.5f },
        0.0f };

    constexpr Material point_light{
        { 0.2f, 0.2f, 0.2f },
        { 0.5f, 0.5f, 0.5f },
        { 0.5f, 0.5f, 0.5f },
        0.0f };

    constexpr Material gold{
        { 0.24725f, 0.1995f, 0.0745f },
        { 0.75164f, 0.60648f, 0.22648f },
        { 0.628281f, 0.555802f, 0.366065f },
        51.2f
    };

    constexpr Material copper{
        { 0.2295f, 0.08825f, 0.0275f },
        { 0.5508f, 0.2118f, 0.066f },
        { 0.580594f, 0.223257f, 0.0695701f },
        51.2f
    };

    constexpr Material turquoise{
        { 0.1f, 0.18725f, 0.1745f },
        { 0.396f, 0.74151f, 0.69102f },
        { 0.297254f, 0.30829f, 0.306678f },
        12.8f
    };
}

#endif // MENGER_SPONGE_GLOBAL_H
