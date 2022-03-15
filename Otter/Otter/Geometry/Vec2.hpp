#pragma once
#include "stl.hpp"
#include <complex>
#include "../Serialize/Serializable.hpp"

namespace otter { class Line; class Vec2i; class Circle; }

namespace otter {

class Vec2 final : public Serializable {
public:
    Vec2();
    Vec2(double x, double y);
    Vec2(const Vec2i& vec2i);
    
    static Vec2 xy(double xAndY);
    static Vec2 polar(double angle, double distance);
    static Vec2 random(Vec2 bottomLeft, Vec2 topRight);
    
// Vec2i conversion
    Vec2i round() const;
    Vec2i floor() const;
    Vec2i ceil() const;
    
// variables
    double x;
    double y;
    
// DIN-A formats
    static const Vec2 a7;
    static const Vec2 a6;
    static const Vec2 a5;
    static const Vec2 a4;
    static const Vec2 a3;
    static const Vec2 a2;
    static const Vec2 a1;
    static const Vec2 a0;

    // manipulation
    Vec2 moveDist(const Vec2& to, double length) const;
    Vec2 moveFactor(const Vec2& to, double factor) const;
    
    Vec2 normalLeft() const;
    
    Vec2 rotate(double angle) const;
    Vec2 rotate(const Vec2& pivot, double angle) const;
    
    Vec2 offset(const Vec2& direction) const;
    Vec2 offsetX(double xDirection) const;
    Vec2 offsetY(double yDirection) const;
    
    Vec2 normalize() const;
    Vec2 normalize(double length) const;
    Vec2 scale(double factor) const;
    Vec2 scale(const Vec2& factor) const;
    Vec2 scale(const Vec2& center, double factor) const;
    
    Vec2 swapAxes() const;
    Vec2 flipSign() const;
    Vec2 flipSignX() const;
    Vec2 flipSignY() const;
    Vec2 flipX(double atX) const;
    Vec2 flipY(double atY) const;
    
    Vec2 onlyX() const;
    Vec2 onlyY() const;
    
    Vec2 invert(const std::shared_ptr<Circle>& inversionC) const;
    
    // getters
    double getLength() const;
    double getAngle(bool onlyPositive) const;
    
    double distance(const Vec2& other) const;
    double distanceSquared(const Vec2& other) const;
    double distanceManhattan(const Vec2& other) const;
    bool isNearly(const Vec2& other) const;
    
    bool isLeftOfLine(const Line& line) const;
    bool isOnLine(const Line& line) const;
    bool isFacingLessThan90Off(const Vec2& other) const;
    
    double dot(const Vec2& other) const;
    double cross(const Vec2& other) const;
    
    double min() const;
    double max() const;
    
    // complex
    std::complex<double> toComplex() const;
    static Vec2 fromComplex(const std::complex<double>& complexNumber);
    
// serializable
    virtual void serialize(std::ostream& os) const override;
    virtual std::shared_ptr<Serializable> deserialize(const std::vector<std::string>& subElements) override;
    virtual std::string getName() const override;
    
// operators
    Vec2 operator+(const Vec2& other) const;
    Vec2 operator+(const double& other) const;
    void operator+=(const Vec2& other);
    void operator+=(const double& other);
    friend Vec2 operator+(const double& a, const Vec2& b);
    
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator-(const double& other) const;
    void operator-=(const Vec2& other);
    void operator-=(const double& other);
    friend Vec2 operator-(const double& a, const Vec2& b);
    
    Vec2 operator*(const Vec2& other) const;
    Vec2 operator*(const double& other) const;
    void operator*=(const Vec2& other);
    void operator*=(const double& other);
    friend Vec2 operator*(const double& a, const Vec2& b);
    
    Vec2 operator/(const Vec2& other) const;
    Vec2 operator/(const double& other) const;
    void operator/=(const Vec2& other);
    void operator/=(const double& other);
    friend Vec2 operator/(const double& a, const Vec2& b);
    
    bool operator==(const Vec2& other) const;
    bool operator!=(const Vec2& other) const;
};

}
