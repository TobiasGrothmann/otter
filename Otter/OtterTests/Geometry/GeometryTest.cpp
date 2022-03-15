#include <stdio.h>
#include "catch_amalgamated.hpp"
#include "Otter.hpp"

using namespace std;
using namespace otter;

TEST_CASE("Geometry")
{
    SECTION("getClosestPointOnCircle")
    {
        Vec2 center = Vec2(1, 1);
        double radius = 1.5;
        
        CHECK(Geometry::getClosestPointOnCircle(center, radius, Vec2(1, 5)) == Vec2(1, 2.5));
        CHECK(Geometry::getClosestPointOnCircle(center, radius, Vec2(1, -10)) == Vec2(1, -0.5));
        CHECK(Geometry::getClosestPointOnCircle(center, radius, center) == Vec2(2.5, 1)); // directly on center -> point to the right
    }
    
    SECTION("getDistanceToCircle")
    {
        Vec2 center = Vec2(1, 1);
        double radius = 1.5;
        
        int n = 50;
        for (int i = 0; i < n; i++)
        {
            double angle = (2 * M_PI / n) * i;
            Vec2 point = center + Vec2::polar(angle, radius);
            CHECK(Math::isNearly(Geometry::getDistanceToCircle(center, radius, point), 0.0, 0.000000001));
            Vec2 point2 = center + Vec2::polar(angle, radius * 2);
            CHECK(Math::isNearly(Geometry::getDistanceToCircle(center, radius, point2), radius, 0.000000001));
        }
        
        CHECK(Math::isNearly(Geometry::getDistanceToCircle(Vec2::xy(0.0), 1.0, Vec2::xy(0.0)), 1.0, 0.000000001));
    }
    
    SECTION("convexHullTest")
    {
        vector<Vec2> points = {};
        for (int i = 0; i < 100; i++)
        {
            points.push_back(Vec2::random(Vec2::xy(-10), Vec2::xy(10)));
        }
        
        vector<Vec2> hull = Geometry::findConvexHull(points);
        CHECK(hull.size() > 3);
        CHECK(hull.size() < 50);
        
        PathPtr hullPath = Path::create(hull);
        
        for (const Vec2& point : points)
        {
            if (find(hull.begin(), hull.end(), point) == hull.end())
            {
                CHECK(hullPath->surrounds(point));
            }
        }
    }
}
