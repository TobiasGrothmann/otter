#include "Otter.hpp"
#include "OtterUtil.hpp"

using namespace std;
using namespace otter;

int main(int argc, const char * argv[]) {
    Rand::seed();
    Queue q = Queue();

    const Vec2 size = Vec2(10.0, 10.0);
    const Frame f = Frame(size, Vec2::xy(size.min() * 0.1));
    q.add(f.getOuterRectangle());

    


    
    
    
    
    
    
    q.preview();
    return 0;
}
