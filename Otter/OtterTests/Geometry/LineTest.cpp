#include <stdio.h>
#include "catch_amalgamated.hpp"
#include "Otter.hpp"

using namespace std;
using namespace otter;

TEST_CASE("Line")
{
    SECTION("creation")
    {
        Line line = {Vec2(0, 0), Vec2(1, 1)};
        Line line2 = {Vec2(0, 0), M_PI * 0.25};
        CHECK(line2.b.isOnLine(line));
    }
    
    SECTION("angle")
    {
        Line line = {Vec2(0, 0), Vec2(1, 1)};
        CHECK(line.getAngle(true) == M_PI * 0.25);
    }
}
