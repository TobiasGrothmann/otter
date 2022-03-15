#pragma once
#include "stl.hpp"
#include "Plottable.hpp"
#include "../Geometry/Vec2.hpp"

namespace otter {

class Bezier final : public Plottable
{
public:
    
    Bezier();
    Bezier(const std::shared_ptr<Bezier>& other);
    Bezier(const std::vector<Vec2>& points);
    
    static std::shared_ptr<Bezier> create();
    static std::shared_ptr<Bezier> create(const std::shared_ptr<Bezier>& other);
    static std::shared_ptr<Bezier> create(const std::vector<Vec2>& points);
    
    // methods
    void add(const Vec2& newPoint);
    void add(const std::vector<Vec2>& newPoints);
    void add(const Bezier& BezierToAppend, bool reversed = false);
    void add(const std::shared_ptr<Bezier>& BezierToAppend, bool reversed = false);
    
    void addFront(Vec2 newPoint);
    void addFront(const Bezier& BezierToAppend, bool reversed = false);
    void addFront(const std::shared_ptr<Bezier>& BezierToAppend, bool reversed = false);
    
    void reverse();
    void close();
    
    bool getLastPoint(Vec2& lastPointOut) const;
    
    
    // plottable
    virtual std::shared_ptr<Plottable> copy() const override;
    
    virtual std::vector<Vec2> getPlottablePoints(const SampleSettings& sampleSettings = SampleSettings()) const override;
    
    virtual void offset(const Vec2& dist) override;
    virtual void scale(const double factor) override;
    void scale2D(const Vec2& factor);
    void scale2DAt(const Vec2& pivot, const Vec2& factor);
    virtual void flipX(double x) override;
    virtual void flipY(double y) override;
    virtual bool canBeRotated() const override;
    virtual void rotate(const Vec2& pivot, double angle) override;
    
    virtual bool surrounds(const Vec2& pointToCheck) const override;
    
    virtual double getLength() const override;
    virtual double getArea() const override;
    virtual Vec2 getCenterPointsAverage() const override;
    virtual bool getIsClosed() const override;
    
    // serialize
    virtual void serialize(std::ostream& os) const override;
    virtual std::shared_ptr<Serializable> deserialize(const std::vector<std::string>& subElements) override;
    virtual std::string getName() const override;
    
    
    // variables
    std::vector<Vec2> points;
};

}
