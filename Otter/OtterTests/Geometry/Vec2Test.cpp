#include <stdio.h>
#include "catch_amalgamated.hpp"
#include "Otter.hpp"

using namespace std;
using namespace otter;

TEST_CASE("Vec2")
{
    Vec2 vZero = Vec2(0.0, 0.0);
    
    SECTION("vector == and !=")
    {
        Vec2 v1 = Vec2::random(Vec2::xy(-10), Vec2::xy(0));
        CAPTURE(v1);
        
        Vec2 v2 = Vec2::xy(5);
        CHECK(Vec2(1.0, 1.0) == Vec2(1.0, 1.0));
        CHECK(v1 != v2);
        CHECK(v1 + 0.00000000000001 == v1);
    }
    
    SECTION("vector + and -")
    {
        Vec2 v1 = Vec2(1.3311, 0.5);
        Vec2 v2 = Vec2(1.3311, 0.5);
        
        CHECK(vZero + v1 == v1);
        CHECK(v1 - vZero == v1);
        CHECK(vZero * 10 == Vec2(0, 0));
        CHECK(vZero * 10 == vZero);
        CHECK(v1 * 2 == v1 + v1);
        CHECK(v1 + v2 == v2 + v1);
    }
    
    SECTION("vector * and /")
    {
        Vec2 v1 = Vec2(0.512, 0.331);
        Vec2 v2 = Vec2(1, 0.5);
        
        CHECK(v1 + v1 == v1 * 2);
        CHECK(v1 * v2 / v2 == v1);
        CHECK(v1 * 1 == v1);
        CHECK(v1 * 1.0 == v1);
        CHECK(v1 / 1.0 == v1);
        CHECK(v1 / 1 == v1);
        CHECK(v1 / v1 == Vec2::xy(1));
        CHECK(v1 / v1 == Vec2(1, 1));
    }
    
    SECTION("vector methods")
    {
        Vec2 v1 = Vec2::xy(1);
        CHECK(v1.getLength() == sqrt(2));
        v1 = v1.normalize();
        CHECK(v1 == Vec2::xy(1 / sqrt(2)));
        
        Vec2 v2 = Vec2(0, 1).rotate(M_PI * 2);
        CHECK(v2 == Vec2(0, 1));
        
        v2 = v2.normalize(10);
        CHECK(v2 == Vec2(0, 10));
        
        Vec2 v3 = Vec2(1, 0).rotate(M_PI * 0.5);
        CHECK(v3 == Vec2(0, 1));
    }
    
    SECTION("vector creation")
    {
        CHECK(Vec2::polar(M_PI * 0.5, 1) == Vec2(0, 1));
        CHECK(Vec2::polar(-M_PI, 10) == Vec2(-10, 0));
        
        CHECK(Vec2() == Vec2::xy(0));
        CHECK(Vec2() == Vec2(0.0, 0.0));
        
        CHECK(Vec2::xy(2) == Vec2(2, 2));
        CHECK(Vec2::xy(-0.4411) == Vec2(-0.4411, -0.4411));
        
        const double randRange = 0.1;
        for (int i = 0; i < 10000; i++)
        {
            Vec2 vec = Vec2::random(Vec2::xy(-randRange), Vec2::xy(randRange));
            if (   vec.x < -randRange
                || vec.y < -randRange
                || vec.x > randRange
                || vec.y > randRange)
            {
                CAPTURE(vec);
                CHECK(false);
            }
        }
        
        CHECK(Vec2(0.5, 1.0).moveFactor(Vec2(0, 0), 1.0) == Vec2::xy(0));
        CHECK(Vec2(0.5, 1.0).moveFactor(Vec2(0, 0), 0.5) == Vec2(0.25, 0.5));
        CHECK(Vec2(-1, 0).moveDist(Vec2(0, 0), 1.0) == Vec2::xy(0));
        
        CHECK(Vec2(0, 1).normalLeft() == Vec2(-1, 0));
        CHECK(Vec2(10, 5).normalLeft() == Vec2(-5, 10));
    }
    
    SECTION("Line")
    {
        Line line = {Vec2(0, 0), Vec2(1, 1)};
        CHECK(Vec2::xy(2).isOnLine(line));
        CHECK(Vec2::xy(0).isOnLine(line));
        CHECK(Vec2::xy(-4.1).isOnLine(line));
        CHECK(Vec2::xy(-1).isOnLine(line));
        CHECK(Vec2(0, 10).isLeftOfLine(line));
        CHECK(!Vec2(10, 0).isLeftOfLine(line));

        line = {Vec2(0, 0), M_PI * 0.25};
        CHECK(Vec2::xy(2).isOnLine(line));
        CHECK(Vec2::xy(0).isOnLine(line));
        CHECK(Vec2::xy(-4.1).isOnLine(line));
        CHECK(Vec2::xy(-1).isOnLine(line));
        CHECK(Vec2(0, 10).isLeftOfLine(line));
        CHECK(!Vec2(10, 0).isLeftOfLine(line));
        
        line = {Vec2(0.551, 1.0), Vec2(5.0, 6.0)};
        CHECK(Vec2(0, 10).isLeftOfLine(line));
        CHECK(!Vec2(10, 0).isLeftOfLine(line));
    }

    // CAPTURE
    // REQUIRE
    // Approx
    // INFO
}
