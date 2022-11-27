#pragma once
#include "stl.hpp"
#include "../Geometry/Vec2.hpp"
#include "../Plottables/Path.hpp"
#include "../Plottables/Plottable.hpp"

namespace otter {

struct Intersection
{
    Vec2 pos;
    VINDEX_TYPE(Vec2) sectionIndex;
};

/// provides general geometry utility functions
class Geometry {
public:
    
    static bool getIntersection(
                                const Vec2& aStart,
                                const Vec2& aEnd,
                                const Vec2& bStart,
                                const Vec2& bEnd,
                                Vec2& outVec,
                                bool checkIfOnLines = true);
    
    static std::vector<Vec2> getAllIntersections(
                                const Vec2& lineStart,
                                const Vec2& lineEnd,
                                const Plottable* plottable);
    static std::vector<Vec2> getAllIntersections(
                                const Vec2& lineStart,
                                const Vec2& lineEnd,
                                const std::vector<Vec2>& segments);
    
    static std::vector<Intersection> getAllIntersectionsWithSegment(
                                const Vec2& lineStart,
                                const Vec2& lineEnd,
                                const Plottable* plottable);
    static std::vector<Intersection> getAllIntersectionsWithSegment(
                                const Vec2& lineStart,
                                const Vec2& lineEnd,
                                const std::vector<Vec2>& segments);
    
    static std::shared_ptr<Path> getSmoothCorners(const std::shared_ptr<Plottable> plottable,
                                                  double factor,
                                                  int iterations = 10,
                                                  double minDistManhattan = 0.03);
    
    static std::shared_ptr<Path> getCutCorners(const std::shared_ptr<Plottable> shape,
                                               double cutDist,
                                               int iterations = 1);
    
    static Vec2 getClosestPointOnCircle(const Vec2& center, double radius, Vec2 point);
    static double getDistanceToCircle(const Vec2& center, double radius, Vec2 point);
    static double getDistanceOfCircles(const Vec2& centerA, double radiusA, const Vec2& centerB, double radiusB);
    static bool isCircleCompletelyInOther(const Vec2& centerA, double radiusA, const Vec2& centerB, double radiusB);
    
    static std::vector<Vec2> findConvexHull(std::vector<Vec2> points);
    
    static bool getCircleFromPoints(const Vec2& a,
                                    const Vec2& b,
                                    const Vec2& c,
                                    Vec2& outCenter,
                                    double& outRadius);
    
    static Vec2 getAverageOfPoints(const std::vector<Vec2>& points);
    
    static bool circleCircleIntersections(const Vec2& centerA,
                                          double radiusA,
                                          const Vec2& centerB,
                                          double radiusB,
                                          Vec2& outIntersectionA,
                                          Vec2& outIntersectionB);
};

}
