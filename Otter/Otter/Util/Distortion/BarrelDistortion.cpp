#include "BarrelDistortion.hpp"

namespace otter {

BarrelDistortion::BarrelDistortion(const Vec2& center, double power)
    : Distortion(
        [center, power] (const Vec2& point) -> Vec2
        {
            double angle = (point - center).getAngle(true);
            double distance = point.distance(center);
            distance = pow(distance, power);
            return center + Vec2::polar(angle, distance);
        })
{
}

}
