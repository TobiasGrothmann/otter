#pragma once
#include "stl.hpp"
#include "../Geometry/Vec2.hpp"
#include "../Geometry/Line.hpp"
#include "../Serialize/Serializable.hpp"

namespace otter { class Path; class Rectangle; class Circle; }

namespace otter {

/// settings of the resolution and from where plottables are sampled
/// This is either used when plottables like circles of bezier are converted into a polyline or when plottables need to be oversampled e.g. for distortion
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
    
    double resolution = 30.0;           ///< in points per centimeter
    bool useStartPoint = false;         ///< wether the point where the pen is coming from should be considered;
                                        ///< e.g. a circle will be plotted from the closest point to the startPoint
    Vec2 startPoint = Vec2(0.0, 0.0);   ///< point where the pen is coming from
};

/// parent class (interface) for shapes that can be plotted
class Plottable : public Serializable, public std::enable_shared_from_this<Plottable>
{
public:
    virtual ~Plottable() = default;
    
// plottable interface
    /// create deep copy
    virtual std::shared_ptr<Plottable> copy() const = 0;
    /// move complete plottable uniformly
    virtual void offset(const Vec2& dist) = 0;
    /// scale in both dimentsions
    virtual void scale(const double factor) = 0;
    /// mirror at vertical line at x
    virtual void flipX(double x) = 0;
    /// mirror at horizontal line at y
    virtual void flipY(double y) = 0;
    /// wether or not the plottable can be rotated
    virtual bool canBeRotated() const = 0;
    /// rotate around a pivot by angle
    virtual void rotate(const Vec2& pivot, double angle);
    
    /// get list of points to be plotted as a polyline
    virtual std::vector<Vec2> getPlottablePoints(const SampleSettings& sampleSettings = SampleSettings()) const = 0;
    
    /// does this plottable surround pointToCheck
    virtual bool surrounds(const Vec2& pointToCheck) const = 0;
    
    /// get complete length of plottable
    virtual double getLength() const = 0;
    /// get area of plottable
    virtual double getArea() const = 0;
    /// get a crude centery point that is just an average of the points in path and bezier, and the center of circle and rectangle
    virtual Vec2 getCenterPointsAverage() const = 0;
    /// is this shape closed; is the first point the same as the last
    virtual bool getIsClosed() const = 0;
    
// overridable methods for performance
    /// get the rectangle that tightly encloses this complete shape
    virtual Rectangle getBounds() const;
    /// compute the points that form the convex hull of this shape; will start at leftmost point and be in clockwise order
    virtual std::vector<Vec2> getConvexHull(const SampleSettings& sampleSettings = SampleSettings());
    /// invert in circle; see https://en.wikipedia.org/wiki/Inversive_geometry#Circle
    /// @param inversionCircle circle to invert shape in
    virtual std::shared_ptr<Plottable> GetInvertedInCircle(const std::shared_ptr<Circle>& inversionCircle, const SampleSettings& sampleSettings = SampleSettings()) const;
    
    /// get points as polyline, but oversample such that segments have a maximum length defined by sampleSettings.resolution
    virtual std::vector<Vec2> getOversampledPoints(const SampleSettings& sampleSettings = SampleSettings()) const;
    /// get points as polyline in form of list of segments of two points
    virtual std::vector<Line> getAsSegments(const SampleSettings& sampleSettings = SampleSettings()) const;
    
// non virtual methods
    /// scale at location
    /// @param pivot center point of scaling
    /// @param scale scale factor in both dimensions
    void scaleAt(const Vec2& pivot, double scale);
    /// scale and offset so that plottable exactly fits inside of bounds
    /// @param bounds rectangle shape to tightly fit in
    void normalize(const Rectangle& bounds, bool moveToMid = true, bool fillInBothDimensions = false);
    
// masking
    /// return struct for masking a plottable. contains a list of all parts of plottables that are in- and outside of mask.
    struct Masked
    {
        std::vector<std::shared_ptr<Plottable>> inside = {};
        std::vector<std::shared_ptr<Plottable>> outside = {};
    };
    /// mask with another plottable; other plottable should be closed; returns a list of all parts that are in- and outside of mask.
    /// @param mask shape that defines in- and outside
    virtual Masked mask(std::shared_ptr<Plottable> mask) const;
    /// mask multiple other plottables; this plottable should be closed; returns a list of all parts of the other shapes that are in- and outside of this
    /// @param shapes shape to be masked
    virtual Masked mask(const std::vector<std::shared_ptr<Plottable>>& shapes);
    
    /// get list of shapes by discarding everything inside of this plottable
    /// @param shapes shapes to be masked
    virtual std::vector<std::shared_ptr<Plottable>> getMaskedOutside(const std::vector<std::shared_ptr<Plottable>>& shapes);
    /// get list of shapes by discarding everything of the shapes that is inside of mask
    /// @param shapes shapes to be masked
    /// @param masks mask, expected to be closed
    static std::vector<std::shared_ptr<Plottable>> getMaskedOutside(const std::vector<std::shared_ptr<Plottable>>& shapes, const std::vector<std::shared_ptr<Plottable>>& masks);
    
    /// get list of shapes by discarding everything outside of this plottable
    /// @param shapes shapes to be masked
    virtual std::vector<std::shared_ptr<Plottable>> getMaskedInside(const std::vector<std::shared_ptr<Plottable>>& shapes);
    /// get list of shapes by discarding everything of the shapes that is outside of mask
    /// @param shapes shapes to be masked
    /// @param masks mask, expected to be closed
    static std::vector<std::shared_ptr<Plottable>> getMaskedInside(const std::vector<std::shared_ptr<Plottable>>& shapes, const std::vector<std::shared_ptr<Plottable>>& masks);
};

}
