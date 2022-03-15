#pragma once
#include "stl.hpp"
#include "../../Geometry/Vec2.hpp"
#include "../../Plottables/Path.hpp"
#include "../../Plottables/Rectangle.hpp"
#include "../../Queue/Queue.hpp"

namespace otter { class Sample; class SampleTriangle; }

namespace otter {

class SampleFunction
{
public:
    SampleFunction(const std::shared_ptr<Rectangle>& bounds,
                   std::function<double(const Vec2&)> func,
                   double desiredResolution = 0.05);
    ~SampleFunction();
    
    Queue getPaths(double valueTarget) const;
    
private:
    void createTriangles();
    
    std::shared_ptr<Rectangle> bounds;
    std::function<double(const Vec2&)> func;
    double desiredResolution;
    
    std::vector<const Sample*> samples = {};
    std::vector<const SampleTriangle*> triangles = {};
};

}
