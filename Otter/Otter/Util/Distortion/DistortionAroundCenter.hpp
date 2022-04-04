#pragma once
#include "stl.hpp"
#include "Distortion.hpp"

namespace otter {

/// disortion subclass for distortion by distance and angle around a center point
class DistortionAroundCenter : public Distortion
{
public:
    DistortionAroundCenter(const Vec2& center,
                           std::function<void(double& distance, double& angle)> function);
};

}
