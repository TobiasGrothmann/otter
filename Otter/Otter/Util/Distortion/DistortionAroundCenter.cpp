#include "DistortionAroundCenter.hpp"

using namespace std;

namespace otter {

DistortionAroundCenter::DistortionAroundCenter(const Vec2& center, std::function<void(double& distance, double& angle)> function)
    : Distortion(
         [center, function] (const Vec2& point) -> Vec2
         {
             double angle = (point - center).getAngle(true);
             double distance = point.distance(center);
             
             function(distance, angle);
             return center + Vec2::polar(angle, distance);
         })
{
}

}
