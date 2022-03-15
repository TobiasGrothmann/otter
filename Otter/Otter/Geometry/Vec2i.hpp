#pragma once
#include "stl.hpp"

namespace otter { class Vec2; }

namespace otter {

class Vec2i
{
public:
    
    Vec2i();
    Vec2i(int x, int y);
    
    static Vec2i round(const Vec2& vec2);
    static Vec2i floor(const Vec2& vec2);
    static Vec2i ceil(const Vec2& vec2);
    
    int x;
    int y;
};

}
