#ifndef SHIGENO_Y_VMOUSE_SPIRAL_INCLUDED
#define SHIGENO_Y_VMOUSE_SPIRAL_INCLUDED

namespace shigenoy::vmouse {
#ifndef PI
constexpr float PI = 3.141592653589793;
#endif

class Spiral
{
private:
    const long max_tick_;
    const float A_;

    float x_;
    float y_;

    float acc_dx_;
    float acc_dy_;

    long current_tick_;

public:
    Spiral(const long max_tick, const float A);

public:
    bool update(const long delta_tick, int& dx, int& dy);
    void getCurrentPos(int& x, int& y) const;
};
} // namespace shigenoy::vmouse

#endif
