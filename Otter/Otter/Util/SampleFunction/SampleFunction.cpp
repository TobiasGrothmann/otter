#include "SampleFunction.hpp"
#include "SampleTriangle.hpp"

using namespace std;

namespace otter {

SampleFunction::SampleFunction(const shared_ptr<Rectangle>& bounds, std::function<double(const Vec2&)> func, double desiredResolution)
    : bounds(Rectangle::create(bounds))
    , func(func)
    , desiredResolution(desiredResolution)
{
    createTriangles();
}

SampleFunction::~SampleFunction()
{
    for (const Sample* sample : samples)
        delete sample;
    for (const SampleTriangle* triangle : triangles)
        delete triangle;
}

void SampleFunction::createTriangles()
{
    const Vec2 size = bounds->getSize();
    const VINDEX_TYPE(Sample*) numX = (VINDEX_TYPE(Sample*)) ceil(size.x / desiredResolution);
    const VINDEX_TYPE(Sample*) numY = (VINDEX_TYPE(Sample*)) ceil(size.y / desiredResolution);
    Vec2 resolution = size / Vec2(numX, numY);
 
    samples = {};
    samples.reserve((numX + 1) * (numY + 1) * 2);
    triangles = {};
    triangles.reserve((numX + 1) * (numY + 1) * 4);
    
    vector<const Sample*> lastRow = {};
    for (VINDEX(lastRow) y = 0; y < numY + 1; y++)
    {
        vector<const Sample*> thisRow = {};
        thisRow.reserve(numX + 1);
        for (VINDEX(lastRow) x = 0; x < numX + 1; x++)
        {
            const Vec2 botRightCoord = bounds->bottomLeft + Vec2(x, y) * resolution;
            const Sample* botRight = new Sample(botRightCoord, func(botRightCoord));
            thisRow.push_back(botRight);
            samples.push_back(botRight);
            
            if (x == 0 || y == 0) continue;
            
            const Vec2 centerCoord = botRightCoord - resolution * 0.5;
            
            const Sample* topLeft = lastRow[x - 1];
            const Sample* topRight = lastRow[x];
            const Sample* botLeft = thisRow[x - 1];
            const Sample* center = new Sample(centerCoord, func(centerCoord));
            samples.push_back(center);
            
            triangles.push_back(new SampleTriangle(center, topLeft, topRight));
            triangles.push_back(new SampleTriangle(center, topRight, botRight));
            triangles.push_back(new SampleTriangle(center, botRight, botLeft));
            triangles.push_back(new SampleTriangle(center, botLeft, topLeft));
        }
        lastRow = thisRow;
    }
}

Queue SampleFunction::getPaths(double valueTarget) const
{
    Queue out = Queue();
    
    for (const SampleTriangle* triangle : triangles)
    {
        shared_ptr<Path> p = make_shared<Path>();
        if (triangle->getIntersection(valueTarget, p))
        {
            if (p->points.size() != 2)
                cout << "WARNING: SampleFunction encountered triangle that has" << p->points.size() << " intersections. (must be 2)" << endl;
            
            out.add(p);
        }
    }
    
    out.combinePlottables();
    
    return out;
}

}
