#pragma once
#include "stl.hpp"
#include "Distortion.hpp"

namespace otter {

/// see https://en.wikipedia.org/wiki/Distortion_(optics)
class BarrelDistortion : public Distortion
{
public:
    BarrelDistortion(const Vec2& center, double power);
};

}
