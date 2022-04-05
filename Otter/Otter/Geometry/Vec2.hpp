#pragma once
#include "stl.hpp"
#include <complex>
#include "../Serialize/Serializable.hpp"

namespace otter { class Line; class Vec2i; class Circle; }

namespace otter {

/// two dimensional vector
class Vec2 final : public Serializable {
public:
    Vec2();
    Vec2(double x, double y);
    Vec2(const Vec2i& vec2i);
    
    /// create vector with the same x and y value
    /// @param xAndY value for x and y
    static Vec2 xy(double xAndY);
    /// create vector from polar coordinates
    /// @param angle angle in radians
    /// @param distance radius
    static Vec2 polar(double angle, double distance);
    /// create random vector within min and max values
    /// @param bottomLeft minimum values or bottom left corner
    /// @param topRight maximum values or topright corner
    static Vec2 random(Vec2 bottomLeft, Vec2 topRight);
    
// Vec2i conversion
    /// convert to Vec2i by rounding both x and y
    Vec2i round() const;
    /// convert to Vec2i by flooring both x and y
    Vec2i floor() const;
    /// convert to Vec2i by ceiling both x and y
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
    /// get vector in direction of other vector by length
    /// @param to other point to move towards
    /// @param length length to move by
    Vec2 moveDist(const Vec2& to, double length) const;
    /// get vector in direction of other vector by factor of distance
    /// @param to other point to move towards
    /// @param factor factor of the distance to move (1.0 means other point is reached)
    Vec2 moveFactor(const Vec2& to, double factor) const;
    
    /// get normal vector (orthogonal) to the left; will have the same length
    Vec2 normalLeft() const;
    
    /// get vector rotated
    /// @param angle angle to rotate by in radians
    Vec2 rotate(double angle) const;
    /// get vector rotated around a pivot
    /// @param pivot pivot point to rotate around
    /// @param angle angle to rotate by in radians
    Vec2 rotate(const Vec2& pivot, double angle) const;
    
    /// get vector moved; equal to adding
    /// @param direction direction to move in
    Vec2 offset(const Vec2& direction) const;
    /// get vector moved only x; equal to adding x,0
    /// @param xDirection direction to move x in
    Vec2 offsetX(double xDirection) const;
    /// get vector moved only y; equal to adding 0,y
    /// @param yDirection direction to move y in
    Vec2 offsetY(double yDirection) const;
    
    /// get vector normalized; results in vector with length 1
    Vec2 normalize() const;
    /// get vector scaled to be a certain length
    /// @param length target legnth
    Vec2 normalize(double length) const;
    /// get vector scaled uniformly by a factor; equal to multiplication
    /// @param factor factor to scale both x and y
    Vec2 scale(double factor) const;
    /// get vector scaled in both x and y direction; equal to multiplication of this->x with factor->x and this-> with factor->y
    /// @param factor factor to scale
    Vec2 scale(const Vec2& factor) const;
    /// get vector scaled uniformly around a center by factor
    /// @param center pivot center to scale around
    /// @param factor factor to scale both x and y
    Vec2 scale(const Vec2& center, double factor) const;
    
    /// get vector with swapped x and y components
    Vec2 swapAxes() const;
    /// get vector with flipped sign for both x and y; same as multiplying with -1
    Vec2 flipSign() const;
    /// get vector with flipped sign for x; same as multiplying x with -1
    Vec2 flipSignX() const;
    /// get vector with flipped sign for y; same as multiplying y with -1
    Vec2 flipSignY() const;
    /// get vector with mirrored x at value atX
    Vec2 flipX(double atX) const;
    /// get vector with mirrored y at value atY
    Vec2 flipY(double atY) const;
    
    /// gert vector only on x axis; y will be 0
    Vec2 onlyX() const;
    /// gert vector only on y axis; x will be 0
    Vec2 onlyY() const;
    
    /// invert vector in circle; see https://en.wikipedia.org/wiki/Inversive_geometry#Circle
    /// @param inversionC circle to invert in
    Vec2 invert(const std::shared_ptr<Circle>& inversionC) const;
    
    // getters
    /// get length of vector
    double getLength() const;
    /// get angle of vector in radians
    /// @param onlyPositive only positive (0 -> 2pi; else -pi -> pi)
    double getAngle(bool onlyPositive) const;
    
    /// get distance of point to other point
    /// @param other other point to compute distance to
    double distance(const Vec2& other) const;
    /// get squared distance of point to other point
    /// @param other other point to compute squared distance to
    double distanceSquared(const Vec2& other) const;
    /// get manhattan distance of point to other point
    /// @param other other point to compute manhattan distance to
    double distanceManhattan(const Vec2& other) const;
    /// check if point is the same or almost the same as the other point
    /// @param other other point to check
    /// @param delta maximum delta for manhattan distance to count as the same point
    bool isNearly(const Vec2& other, double delta = 0.0001) const;
    
    /// check wether point is to the left of the line from a to b
    /// @param line line
    bool isLeftOfLine(const Line& line) const;
    /// check wether point is on line; line has infinite length and is defined by a and b
    /// @param line line of infinite length defined by two points
    bool isOnLine(const Line& line) const;
    /// check wether the other point is less than 90 degree (pi/2) off of this one; is the same as checking wether the dot product is bigger than 0.0
    /// @param other other point to check
    bool isFacingLessThan90Off(const Vec2& other) const;
    
    /// compute dot product
    double dot(const Vec2& other) const;
    /// compute dot product
    double cross(const Vec2& other) const;
    
    double min() const;
    double max() const;
    
    // complex
    /// convert to std::complex<double> where x is the real and y is imaginary
    std::complex<double> toComplex() const;
    /// create vector from std::complex<double> where real is x and imag is y
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
