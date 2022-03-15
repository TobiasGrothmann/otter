#include "Otter.hpp"
#include "OtterUtil.hpp"

using namespace std;
using namespace otter;

int main(int argc, const char * argv[]) {
    Rand::seed();
    Queue q = Queue();

    // setup sketch size and frame
    const Vec2 size = Vec2::a5;                                 // set the size of the sketch
    const Frame f = Frame(size, Vec2::xy(size.min() * 0.1));    // create a virtual frame with 10% margin
    
    q.add(f.getOuterRectangle()); // show outline of the paper size

    // settings
    const Vec2 center = size * 0.5;                 // define the location of the center of the spiral
    const double maxDist = size.getLength();        // use diagonal length of sketch as spiral size
    const double circleSize = 0.05;                 // size of the individual dots
    const double anglePerStep = M_GR * 2.0 * M_PI;  // increase angle by golden ratio (radians) every step
    const double distanceFactor = 0.15;             // factor that spaces out points
    
    // setup
    double distFromCenter = 0.0;
    double angle = Rand::angle();
    int i = 0;
    
    // create spiral
    while (distFromCenter < maxDist)
    {
        i++;
        distFromCenter = sqrt(i) * distanceFactor;  // set distance from center a little bigger for each iteration
        angle += anglePerStep;                      // increase angle
        
        const Vec2 newPoint = center + Vec2::polar(angle, distFromCenter);
        if (f.getInnerRectangle()->surrounds(newPoint)) // only draw when inside the frame margin
        {
            CirclePtr circle = Circle::create(newPoint, circleSize);
            q.add(circle); // can also be done by using q.circle(center, radius)
        }
    }
    
    // open OtterUI
    q.preview();
    
    return 0;
}
