#pragma once
#include "stl.hpp"
#include "Plottable.hpp"
#include "../Geometry/Vec2.hpp"

namespace otter {

class Rectangle final : public Plottable
{
public:
    
    Rectangle() = default;
    Rectangle(const std::shared_ptr<Rectangle>& other);
    Rectangle(const Vec2& bottomLeft, const Vec2& topRight);
    
    static std::shared_ptr<Rectangle> create(const std::shared_ptr<Rectangle>& other);
    static std::shared_ptr<Rectangle> create(const Vec2& bottomLeft, const Vec2& topRight);
    
    // plottable
    virtual std::shared_ptr<Plottable> copy()  const override;
    
    virtual std::vector<Vec2> getPlottablePoints(const SampleSettings& sampleSettings = SampleSettings()) const override;
    
    virtual void offset(const Vec2& dist) override;
    virtual void scale(const double factor) override;
    virtual void flipX(double x) override;
    virtual void flipY(double y) override;
    virtual bool canBeRotated() const override;
    
    virtual bool surrounds(const Vec2& pointToCheck) const override;
    
    virtual double getLength() const override;
    virtual double getArea() const override;
    virtual Vec2 getCenterPointsAverage() const override;
    virtual bool getIsClosed() const override;
    
    // serialize
    virtual void serialize(std::ostream& os) const override;
    virtual std::shared_ptr<Serializable> deserialize(const std::vector<std::string>& subElements) override;
    virtual std::string getName() const override;
    
    
    // methods
    Vec2 getSize() const;
    Vec2 getCenter() const;
    double getHeight() const;
    double getWidth() const;
    double getAspectRatio() const;
    
    bool overlaps(const Rectangle& otherRect) const;
    bool isSquare() const;
    
    std::shared_ptr<Path> getTopSide() const;
    std::shared_ptr<Path> getRightSide() const;
    std::shared_ptr<Path> getBottomSide() const;
    std::shared_ptr<Path> getLeftSide() const;
    Vec2 getTopMid() const;
    Vec2 getRightMid() const;
    Vec2 getBottomMid() const;
    Vec2 getLeftMid() const;
    
    // variables
    Vec2 bottomLeft;
    Vec2 topRight;
    Vec2 getTopRight() const;
    Vec2 getBottomLeft() const;
    Vec2 getBottomRight() const;
    Vec2 getTopLeft() const;
};

}
