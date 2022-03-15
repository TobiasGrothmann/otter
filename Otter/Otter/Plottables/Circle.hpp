#pragma once
#include "stl.hpp"
#include "Plottable.hpp"
#include "../Geometry/Vec2.hpp"

namespace otter {

class Circle final : public Plottable
{
public:
    
    Circle() = default;
    Circle(const std::shared_ptr<Circle>& other);
    Circle(const Vec2& center, double radius);
    
    static std::shared_ptr<Circle> create(const Vec2& center, double radius);
    static std::shared_ptr<Circle> create(const std::shared_ptr<Circle>& other);
    
    // plottable
    virtual std::shared_ptr<Plottable> copy() const override;
    
    virtual std::vector<Vec2> getPlottablePoints(const SampleSettings& sampleSettings = SampleSettings()) const override;
    
    virtual void offset(const Vec2& dist) override;
    virtual void scale(const double factor) override;
    virtual void flipX(double x) override;
    virtual void flipY(double y) override;
    virtual bool canBeRotated() const override;
    virtual void rotate(const Vec2& pivot, double angle) override;
    
    virtual std::shared_ptr<Plottable> GetInvertedInCircle(const std::shared_ptr<Circle>& inversionCircle, const SampleSettings& sampleSettings = SampleSettings()) const override;
    
    virtual bool surrounds(const Vec2& pointToCheck) const override;
    
    virtual double getLength() const override;
    virtual double getArea() const override;
    virtual Vec2 getCenterPointsAverage() const override;
    Rectangle getBounds() const override;
    virtual bool getIsClosed() const override;
    
    // serialize
    virtual void serialize(std::ostream& os) const override;
    virtual std::shared_ptr<Serializable> deserialize(const std::vector<std::string>& subElements) override;
    virtual std::string getName() const override;
    
    
    // variables
    Vec2 center;
    
    double radius;
};

}
