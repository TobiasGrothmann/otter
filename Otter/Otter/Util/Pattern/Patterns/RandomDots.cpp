#include "RandomDots.hpp"
#include "../../../Structures/Grid.hpp"

using namespace std;

namespace otter {

RandomDots::RandomDots(double dotDist, double dotSize, double randomness)
    : dotDist(dotDist)
    , dotSize(dotSize)
    , randomness(randomness)
{
}

Queue RandomDots::create(const shared_ptr<Rectangle>& bounds) const
{
    Queue outQueue = Queue();
    
    const Vec2 size = bounds->getSize();
    
    Vec2 num = size / dotDist;
    const int xs = (int) ceil(num.x);
    const int ys = (int) ceil(num.y);
    
    const double randomDistance = randomness * dotDist;
    
    Grid g = Grid(bounds, xs, ys, Vec2::xy(0.0));
    for (int x = 0; x < xs; x++)
    {
        for (int y = 0; y < ys; y++)
        {
            Rectangle cell = g.getCell(x, y);
            outQueue.circle(cell.getCenter() + Vec2::random(Vec2::xy(-randomDistance), Vec2::xy(randomDistance)), dotSize);
        }
    }
    
    return outQueue;
}

}
