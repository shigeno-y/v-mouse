#ifndef SHIGENO_Y_VMOUSE_FASTMATH_INCLUDED
#define SHIGENO_Y_VMOUSE_FASTMATH_INCLUDED

#ifdef _MSC_FULL_VER
#    include <cmath>
#else

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

namespace takashiijiri {
//---Algorithm float(IEEE754)用------
inline float
sqrt(const float& x)
{
    float xHalf = 0.5f * x;
    int tmp     = 0x5F3759DF - (*(int*)&x >> 1); //initial guess
    float xRes  = *(float*)&tmp;

    xRes *= (1.5f - (xHalf * xRes * xRes));
    //xRes *= ( 1.5f - ( xHalf * xRes * xRes ) );//コメントアウトを外すと精度が上がる
    return xRes * x;
}
//---Algorithm double(IEEE754)用------
inline double
sqrt(const double& x)
{
    double xHalf      = 0.5 * x;
    long long int tmp = 0x5FE6EB50C7B537AAl - (*(long long int*)&x >> 1); //initial guess
    double xRes       = *(double*)&tmp;

    xRes *= (1.5 - (xHalf * xRes * xRes));
    //xRes *= ( 1.5 - ( xHalf * xRes * xRes ) );//コメントアウトを外すと精度が上がる
    return xRes * x;
}
} // namespace takashiijiri

#endif
#endif
