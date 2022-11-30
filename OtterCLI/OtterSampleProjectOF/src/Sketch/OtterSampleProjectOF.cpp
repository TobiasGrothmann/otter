#include "OtterSampleProjectOF.hpp"

using namespace otter;
using namespace std;


void OtterSampleProjectOF::setup()
{
    updateOnParamChange = true;
}

Queue OtterSampleProjectOF::create(const State& state)
{
    Rand::seed();
    Queue q = Queue();

    const Vec2 size = Vec2(10.0, 10.0);
    const Frame f = Frame(size, Vec2::xy(size.min() * 0.1));
    q.add(f.getOuterRectangle());

    


    
    
    
    
    
    
    return q;
}
