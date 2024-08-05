#ifndef SHIGENO_Y_VMOUSE_FASTMATH_INCLUDED
#define SHIGENO_Y_VMOUSE_FASTMATH_INCLUDED

namespace nick_sincos {
// thanks to:
// https://yuqlid.sakura.ne.jp/dokuwiki/fast_sin_cos

inline float
sin(float theta)
{
    using T = float;
    float sin;
    constexpr T pi     = 3.14159265358979323846264338327950288;
    constexpr T two_pi = T(2.0) * pi;
    constexpr T B      = T(4.0) / pi;
    constexpr T C      = T(4.0) / (pi * pi);
    constexpr T zero   = T(0.0);
    constexpr T P      = T(0.225);

    while (theta < -pi)
    {
        theta += two_pi;
    }
    while (theta > pi)
    {
        theta -= two_pi;
    }

    // sin
    if (theta < zero)
    {
        sin = theta * B + C * theta * theta;
        if (sin < zero)
        {
            sin = P * (sin * -sin - sin) + sin;
        }
        else
        {
            sin = P * (sin * sin - sin) + sin;
        }
    }
    else
    {
        sin = theta * B - C * theta * theta;
        if (sin < zero)
        {
            sin = P * (sin * -sin - sin) + sin;
        }
        else
        {
            sin = P * (sin * sin - sin) + sin;
        }
    }
    return sin;
}

inline float
cos(float theta)
{
    using T = float;

    T cos;
    constexpr T pi      = 3.14159265358979323846264338327950288;
    constexpr T half_pi = pi / T(2.0);
    constexpr T two_pi  = T(2.0) * pi;
    constexpr T B       = T(4.0) / pi;
    constexpr T C       = T(4.0) / (pi * pi);
    constexpr T zero    = T(0.0);
    constexpr T P       = T(0.225);

    while (theta < -pi)
    {
        theta += two_pi;
    }
    while (theta > pi)
    {
        theta -= two_pi;
    }
    theta += half_pi;
    if (theta > pi)
    {
        theta -= two_pi;
    }
    // cos
    if (theta < zero)
    {
        cos = theta * B + C * theta * theta;
        if (cos < zero)
        {
            cos = P * (cos * -cos - cos) + cos;
        }
        else
        {
            cos = P * (cos * cos - cos) + cos;
        }
    }
    else
    {
        cos = theta * B - C * theta * theta;
        if (cos < zero)
        {
            cos = P * (cos * -cos - cos) + cos;
        }
        else
        {
            cos = P * (cos * cos - cos) + cos;
        }
    }
    return cos;
}

} // namespace nick_sincos
#endif
