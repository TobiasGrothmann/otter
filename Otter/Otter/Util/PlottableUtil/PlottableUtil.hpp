#pragma once
#include "stl.hpp"
#include "../../Plottables/Path.hpp"
#include "../../Plottables/Plottable.hpp"
#include "../../Queue/Queue.hpp"
#include "../../Geometry/Vec2.hpp"

namespace otter {

class PlottableUtil
{
public:
    static std::shared_ptr<Path> getFractionOfPlottable(const std::shared_ptr<Plottable> plottable,
                                                        double startFraction,
                                                        double endFraction,
                                                        const SampleSettings& sampleSettings = SampleSettings());
    
    static std::vector<Vec2> getPointsAlongPlottable(const std::shared_ptr<Plottable> plottable,
                                                     double desiredStepSize,
                                                     const SampleSettings& sampleSettings = SampleSettings());
    
    static Vec2 getPointAlongPlottableFactor(const std::shared_ptr<Plottable> plottable,
                                             double factor,
                                             const SampleSettings& sampleSettings = SampleSettings());
    static Vec2 getPointAlongPlottableDistance(const std::shared_ptr<Plottable> plottable,
                                               double targetDistance,
                                               const SampleSettings& sampleSettings = SampleSettings());
    
    static Queue getDashed(const std::shared_ptr<Plottable> plottable,
                           double desiredDistance,
                           const SampleSettings& sampleSettings = SampleSettings());
    
    static Queue getDotted(const std::shared_ptr<Plottable> plottable,
                           double desiredDistance,
                           const SampleSettings& sampleSettings = SampleSettings());
    
    static Queue createKaleido(Queue input,
                               const Vec2& center,
                               double num,
                               bool mask = false,
                               double angleOffset = 0.0);
};

}
