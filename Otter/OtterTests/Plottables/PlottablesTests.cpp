#include <stdio.h>
#include "catch_amalgamated.hpp"
#include "Otter.hpp"

using namespace std;
using namespace otter;

TEST_CASE("Plottables")
{
    vector<PlottablePtr> plottables = {
        Circle::create(Vec2(0, 0), 1.0),
        Circle::create(Vec2(-10, 0.5), 0.3),
        
        Rectangle::create(Vec2(0, 0), Vec2(1, 1)),
        Rectangle::create(Vec2(-0.3, 0.4), Vec2(0.3, 0.2)),
        
        Path::create({Vec2(0, 0), Vec2(1, 0), Vec2(1, 1), Vec2(0, 1), Vec2(0, 0)}),
        Path::create({Vec2::xy(-0.2), Vec2(0.8, 0), Vec2(0.3, 1), Vec2(0, 1), Vec2::xy(-0.2)}),
    };
    
    SECTION("interface")
    {
        for (const PlottablePtr& p : plottables)
        {
            CHECK(p->getArea() > 0);
            CHECK(p->getArea() < 10);
            
            vector<Vec2> points = p->getPlottablePoints();
            CHECK(points.size() > 3);
            CHECK(points.size() < 1000);
            
            CHECK(p->getLength() > 1.0);
            CHECK(p->getLength() < 10.0);
            
            vector<Line> segments = p->getAsSegments();
            CHECK(segments.size() > 2);
            CHECK(segments.size() < 1000);
            
            vector<Vec2> oversampled = p->getOversampledPoints();
            CHECK(oversampled.size() > 2);
            CHECK(oversampled.size() < 1000);
            CHECK(oversampled.size() >= points.size());
            
            CHECK(p->getIsClosed());
            
            Rectangle bounds = p->getBounds();
            CHECK(bounds.surrounds(p->getCenterPointsAverage()));
            CAPTURE(bounds);
            for (const Vec2& point : points)
            {
                CAPTURE(point);
                CHECK(bounds.surrounds(point));
            }
        }
    }
    
    SECTION("copy")
    {
        const Vec2 getPointsStartVector = Vec2(4, -2);
        const double resolution = 15.5;
        
        for (const PlottablePtr& p : plottables)
        {
            CAPTURE(p);
            vector<Vec2> points = p->getPlottablePoints(SampleSettings(resolution, getPointsStartVector));
            vector<Line> segments = p->getAsSegments();
            
            // check copy
            PlottablePtr copy = p->copy();
            vector<Line> segmentsCopy = p->getAsSegments();
            vector<Vec2> pointsCopy = p->getPlottablePoints(SampleSettings(resolution, getPointsStartVector));
            REQUIRE(points.size() == pointsCopy.size());
            REQUIRE(segments.size() == segmentsCopy.size());
            REQUIRE(copy->getArea() == p->getArea());
            REQUIRE(copy->getLength() == p->getLength());
            
            for (int i = 0; i < points.size(); i++)
            {
                CHECK(points[i] == pointsCopy[i]);
            }
            for (int i = 0; i < segments.size(); i++)
            {
                CHECK(segments[i].a == segmentsCopy[i].a);
                CHECK(segments[i].b == segmentsCopy[i].b);
            }
        }
    }
    
    SECTION("offset")
    {
        for (const PlottablePtr& p : plottables)
        {
            CAPTURE(p);
            PlottablePtr copy = p->copy();
            
            const Vec2 offset = Vec2(-4, 10.55);
            copy->offset(offset);
            
            CHECK(p->getBounds().bottomLeft + offset == copy->getBounds().bottomLeft);
            CHECK(p->getBounds().topRight + offset == copy->getBounds().topRight);
        }
    }
    
    SECTION("scale")
    {
        for (const PlottablePtr& p : plottables)
        {
            CAPTURE(p);
            PlottablePtr copyLarge = p->copy();
            PlottablePtr copySmall = p->copy();
            
            copyLarge->scale(1.3);
            copySmall->scale(0.9);
            
            CHECK(p->getArea() > copySmall->getArea());
            CHECK(p->getArea() < copyLarge->getArea());
            
            CHECK(p->getLength() > copySmall->getLength());
            CHECK(p->getLength() < copyLarge->getLength());
        }
        
        for (const PlottablePtr& p : plottables)
        {
            CAPTURE(p);
            PlottablePtr copy = p->copy();
            copy->scaleAt(p->getCenterPointsAverage(), 0.6);

            Rectangle bounds = p->getBounds();
            Rectangle copyBounds = copy->getBounds();
            CHECK(bounds.getArea() > copyBounds.getArea());
            CHECK(bounds.getLength() > copyBounds.getLength());
            CHECK(bounds.surrounds(copyBounds.bottomLeft));
            CHECK(bounds.surrounds(copyBounds.topRight));
        }
    }
    
    SECTION("normalize")
    {
        for (const PlottablePtr& p : plottables)
        {
            CAPTURE(p);
            PlottablePtr copy = p->copy();
            Rectangle targetBounds = Rectangle(Vec2(1.0, 1.5), Vec2(1.4, 1.9));
            copy->normalize(targetBounds);
            
            Rectangle copyBounds = copy->getBounds();
            
            CHECK(copyBounds.getArea() <= targetBounds.getArea());
            CHECK(copyBounds.getLength() <= targetBounds.getLength());
            
            CHECK(targetBounds.surrounds(copyBounds.bottomLeft));
            CHECK(targetBounds.surrounds(copyBounds.topRight));
        }
    }
    
//    SECTION("mask")
//    {
//        vector<PlottablePtr> masks = {
//            Circle::create(Vec2(0.1), 0.8),
//            Rectangle::create(Vec2(0.1), Vec2(0.9)),
//        };
//
//        for (const PlottablePtr& mask : masks)
//        {
//            for (const PlottablePtr& p : plottables)
//            {
//                if (p == mask) continue;
//
//                Plottable::Masked masked = p->mask(mask);
//                for (PlottablePtr insidePath : masked.inside)
//                {
//                    for (const Vec2& point : insidePath->getPlottablePoints(15.0, Vec2(0, 0)))
//                    {
//                        CAPTURE(p);
//                        CAPTURE(mask);
//                        CAPTURE(insidePath);
//                        CAPTURE(point);
//                        CHECK(mask->surrounds(point));
////                        if (!mask->surrounds(point))
////                        {
////                            Queue q = Queue({ insidePath, mask });
////                            q.preview();
////                        }
//                    }
//                }
//
//                for (PlottablePtr insidePath : masked.outside)
//                {
//                    for (const Vec2& point : insidePath->getPlottablePoints(15.0, Vec2(0, 0)))
//                    {
//                        CAPTURE(p);
//                        CAPTURE(mask);
//                        CAPTURE(insidePath);
//                        CAPTURE(point);
//                        CHECK(!mask->surrounds(point));
////                        if (!mask->surrounds(point))
////                        {
////                            Queue q = Queue({ insidePath, mask });
////                            q.preview();
////                        }
//                    }
//                }
//            }
//        }
//    }
    
    SECTION("pathSurrounds")
    {
        PathPtr pRight = Path::create({ Vec2(0, 0), Vec2(1, 0), Vec2(1, 1), Vec2(0, 1), Vec2(0, 0) });
        PathPtr pLeft = Path::create({ Vec2(0, 0), Vec2(0, 1), Vec2(1, 1), Vec2(1, 0), Vec2(0, 0) });
        
        CHECK(pRight->surrounds(Vec2(0.5, 0.5)));
        CHECK(pLeft->surrounds(Vec2(0.5, 0.5)));
        
        vector<Vec2> pointsInside =
        {
            Vec2(0.5, 0.5),
            Vec2(0.01, 0.01),
            Vec2(0.99, 0.01),
            Vec2(0.99, 0.99),
            Vec2(0.01, 0.99),
        };
        
        vector<Vec2> pointsOutside =
        {
            Vec2(-0.1, -0.1),
            Vec2(-0.1,  0.0),
            Vec2(-0.1,  0.5),
            Vec2(-0.1,  1.0),
            Vec2(-0.1,  1.1),
            Vec2(0.0,  1.1),
            Vec2(0.5,  1.1),
            Vec2(1.0,  1.1),
            Vec2(1.1,  1.1),
            Vec2(1.1,  1.0),
            Vec2(1.1,  0.5),
            Vec2(1.1,  0.0),
            Vec2(1.1,  -0.1),
            Vec2(1.0,  -0.1),
            Vec2(0.5,  -0.1),
            Vec2(0.0,  -0.1),
        };
        
        for (const Vec2& pointInside : pointsInside)
        {
            CAPTURE(pointInside);
            CHECK(pRight->surrounds(pointInside));
            CHECK(pLeft->surrounds(pointInside));
        }
        
        for (const Vec2& pointOutside : pointsOutside)
        {
            CAPTURE(pointOutside);
            CHECK(!pRight->surrounds(pointOutside));
            CHECK(!pLeft->surrounds(pointOutside));
        }
    }
}
