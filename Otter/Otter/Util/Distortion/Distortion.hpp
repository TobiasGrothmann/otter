#pragma once
#include "stl.hpp"
#include "../../Geometry/Vec2.hpp"
#include "../../Queue/Queue.hpp"
#include "../../Plottables/Plottable.hpp"
#include "../../Plottables/Path.hpp"

namespace otter {
    
class Distortion
{
public:
    Distortion() = delete;
    Distortion(std::function<Vec2(const Vec2&)> function);
    
    virtual Queue getDistorted(const Queue& queue,
                               const SampleSettings& sampleSettings = SampleSettings()) const;
    virtual std::shared_ptr<Path> getDistorted(const std::shared_ptr<Plottable>& plottable,
                                               const SampleSettings& sampleSettings = SampleSettings()) const;
    
protected:
    std::function<Vec2(const Vec2&)> function;
};

}
