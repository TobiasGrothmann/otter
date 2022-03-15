#pragma once
#include "stl.hpp"
#include "../Geometry/Vec2.hpp"
#include "../Geometry/Line.hpp"
#include "../Serialize/Serializable.hpp"

namespace otter { class Path; class Rectangle; class Circle; }

namespace otter {

struct SampleSettings
{
    SampleSettings() = default;
    SampleSettings(double resolution)
        : resolution(resolution)
    {};
    SampleSettings(const Vec2& startPoint)
        : useStartPoint(true)
        , startPoint(startPoint)
    {};
    SampleSettings(double resolution, const Vec2& startPoint)
        : resolution(resolution)
        , useStartPoint(true)
        , startPoint(startPoint)
    {};
    
    double resolution = 30.0;
    bool useStartPoint = false;
    Vec2 startPoint = Vec2(0.0, 0.0);
};

class Plottable : public Serializable, public std::enable_shared_from_this<Plottable>
{
public:
    virtual ~Plottable() = default;
    virtual std::shared_ptr<Plottable> copy() const = 0;
    
    struct Masked
    {
        std::vector<std::shared_ptr<Plottable>> inside = {};
        std::vector<std::shared_ptr<Plottable>> outside = {};
    };
    
    
    virtual void offset(const Vec2& dist) = 0;
    virtual void scale(const double factor) = 0;
    virtual void flipX(double x) = 0;
    virtual void flipY(double y) = 0;
    virtual void scaleAt(const Vec2& pivot, double scale);
    virtual bool canBeRotated() const = 0;
    virtual void rotate(const Vec2& pivot, double angle);
    virtual void normalize(const Rectangle& bounds, bool moveToMid = true, bool fillInBothDimensions = false);
    
    virtual std::vector<Vec2> getPlottablePoints(const SampleSettings& sampleSettings = SampleSettings()) const = 0;
    virtual std::vector<Vec2> getOversampledPoints(const SampleSettings& sampleSettings = SampleSettings()) const;
    virtual std::vector<Line> getAsSegments(const SampleSettings& sampleSettings = SampleSettings()) const;
    
    virtual std::shared_ptr<Plottable> GetInvertedInCircle(const std::shared_ptr<Circle>& inversionCircle, const SampleSettings& sampleSettings = SampleSettings()) const;
    
    virtual bool surrounds(const Vec2& pointToCheck) const = 0;
    
    virtual double getLength() const = 0;
    virtual double getArea() const = 0;
    virtual Vec2 getCenterPointsAverage() const = 0;
    virtual Rectangle getBounds() const;
    virtual std::vector<Vec2> getConvexHull(const SampleSettings& sampleSettings = SampleSettings());
    virtual bool getIsClosed() const = 0;
    
    virtual Masked mask(std::shared_ptr<Plottable> mask) const;
    virtual Masked mask(const std::vector<std::shared_ptr<Plottable>>& shapes);
    
    virtual std::vector<std::shared_ptr<Plottable>> getMaskedOutside(const std::vector<std::shared_ptr<Plottable>>& shapes);
    static std::vector<std::shared_ptr<Plottable>> getMaskedOutside(const std::vector<std::shared_ptr<Plottable>>& shapes, const std::vector<std::shared_ptr<Plottable>>& masks);
    
    virtual std::vector<std::shared_ptr<Plottable>> getMaskedInside(const std::vector<std::shared_ptr<Plottable>>& shapes);
    static std::vector<std::shared_ptr<Plottable>> getMaskedInside(const std::vector<std::shared_ptr<Plottable>>& shapes, const std::vector<std::shared_ptr<Plottable>>& masks);
};

}
