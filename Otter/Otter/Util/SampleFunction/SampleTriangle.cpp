#include "SampleTriangle.hpp"

using namespace std;

namespace otter {

bool SampleTriangle::getIntersection(double targetValue, shared_ptr<Path>& outPath) const
{
    if (targetValue < minValue && targetValue > maxValue) return false;
    
    const Sample* samples[3] = {a, b, c};
    int numAbove = (samples[0]->value > targetValue) + (samples[1]->value > targetValue) + (samples[2]->value > targetValue);
    if (numAbove == 0 || numAbove == 3) return false;
    
    for (int i = 0; i < 3; i++)
    {
        const Sample* from = samples[i];
        const Sample* to = samples[(i + 1) % 3];
        
        if ((from->value > targetValue) == (to->value > targetValue)) continue;
        
        const double distFrom = abs(targetValue - from->value);
        const double distTo = abs(targetValue - to->value);
        const double factor = distFrom / (distFrom + distTo);
        outPath->add(from->point + (to->point - from->point) * factor);
    }
    
    // skip if points are the same (value edge is exactly on a vertex)
    if (outPath->points[0] == outPath->points[1])
        return false;
    
    return true;
}

}
