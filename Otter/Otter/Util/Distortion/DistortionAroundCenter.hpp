#pragma once
#include "stl.hpp"
#include "Distortion.hpp"

namespace otter {

class DistortionAroundCenter : public Distortion
{
public:
    DistortionAroundCenter(const Vec2& center,
                           std::function<void(double& distance, double& angle)> function);
};

}
