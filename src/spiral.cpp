#include "spiral.hpp"

#include "fastmath.hpp"

namespace nick_sincos {
}
namespace takashiijiri{
}

shigenoy::vmouse::Spiral::Spiral(const long max_tick, const float A)
    : max_tick_(max_tick), A_(A), x_(0.), y_(0.), acc_dx_(0.), acc_dy_(0.), current_tick_(0)
{
}

bool
shigenoy::vmouse::Spiral::update(const long delta_tick, int& dx, int& dy)
{
    using namespace nick_sincos;
    using namespace takashiijiri;

    const float theta =
        6. * PI * (static_cast<float>(this->current_tick_) / static_cast<float>(this->max_tick_));
    const float r = this->A_ * sqrt(theta);

    {
        float nx = r * cos(theta);
        this->acc_dx_ += static_cast<float>(nx - this->x_);
        this->x_ = nx;
    }
    {
        float ny = r * sin(theta);
        this->acc_dy_ += static_cast<float>(ny - this->y_);
        this->y_ = ny;
    }
#ifdef _MSC_FULL_VER
    {
        double t{ 0. };
        this->acc_dx_ = std::modf(this->acc_dx_, &t);
        dx            = static_cast<int>(t);
    }
    {
        double t{ 0. };
        this->acc_dy_ = std::modf(this->acc_dy_, &t);
        dy            = static_cast<int>(t);
    }
#else
    dx = static_cast<int>(this->acc_dx_);
    this->acc_dx_ -= dx;

    dy = static_cast<int>(this->acc_dy_);
    this->acc_dy_ -= dy;
#endif

    this->current_tick_ += delta_tick;
    return (this->current_tick_ < this->max_tick_);
}

void
shigenoy::vmouse::Spiral::getCurrentPos(int& x, int& y) const
{
    x = static_cast<int>(this->x_);
    y = static_cast<int>(this->y_);
}
