#pragma once
#include "stl.hpp"
#include "Distortion.hpp"

namespace otter {

class BarrelDistortion : public Distortion
{
public:
    
    BarrelDistortion(const Vec2& center, double power);

};

}
