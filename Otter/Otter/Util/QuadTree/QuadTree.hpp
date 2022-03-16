#pragma once
#include "stl.hpp"
#include "QuadTreeData.hpp"
#include "../../Geometry/Vec2.hpp"
#include "../../Plottables/Plottable.hpp"
#include "../../Queue/Queue.hpp"
#include "../../Plottables/Rectangle.hpp"

namespace otter {

class QuadTree
{
public:
    QuadTree() = default;
    QuadTree(const Queue& queue, int capacity = 3, bool oversampled = false, const SampleSettings& sampleSettings = SampleSettings());
    QuadTree(const Rectangle& bounds, int capacity = 3);
    QuadTree(const std::shared_ptr<Rectangle>& bounds, int capacity = 3);
    
    void add(const Vec2& point);
    void add(const std::vector<Vec2>& points);
    void add(const QuadTreeVec& point);
    void add(const std::shared_ptr<Plottable>& plottable, const SampleSettings& sampleSettings = SampleSettings());
    void addOversampled(const std::shared_ptr<Plottable>& plottable, const SampleSettings& sampleSettings = SampleSettings());
    
    std::vector<QuadTreeVec> getPoints(const Rectangle& queryBounds) const;
    std::vector<QuadTreeVec> getAllPoints() const;
    QuadTreeVec getClosestPoint(const Vec2& to, bool ignorePointItsself = true, const std::vector<std::shared_ptr<Plottable>>& ignored = {}) const;
    
    std::shared_ptr<Sector> sector = {};
    int capacity = 1;
    std::shared_ptr<Rectangle> bounds = {};
    
private:
    void query(const std::shared_ptr<Sector>& querySector, std::vector<QuadTreeVec>& found, const Rectangle& queryBounds) const;
    void queryAll(const std::shared_ptr<Sector>& querySector, std::vector<QuadTreeVec>& found) const;
    
    void queryClosest(const std::shared_ptr<Sector>& querySector, double& closestDistance, QuadTreeVec& closestPoint, const Vec2& targetPoint, bool ignorePointItsself = true, const std::vector<std::shared_ptr<Plottable>>& ignored = {}) const;
    double getMinDistSquared(const Vec2& point, const std::shared_ptr<Sector>& querySector) const;
};

}
