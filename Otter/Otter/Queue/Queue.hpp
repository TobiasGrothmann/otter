#pragma once
#include "stl.hpp"
#include "../Geometry/Vec2.hpp"
#include "../Plottables/Plottable.hpp"
#include "../Serialize/Serializable.hpp"
#include "Settings/QueueSettings.hpp"

namespace otter {

/// contains a number of plottables to plot, preview or modify toghether
class Queue : public Serializable
{
public:
    Queue() = default;
    Queue(const Queue& other);
    Queue(Queue&& other);
    Queue(const std::vector<std::shared_ptr<Plottable>>& items);
    Queue& operator=(const Queue& other);
    Queue& operator=(Queue&& other);
    
    /// add a new item at the end
    /// @param newItem item to add
    void add(std::shared_ptr<Plottable> newItem);
    /// add list of new items at the end
    /// @param newItems items to add
    void add(const std::vector<std::shared_ptr<Plottable>>& newItems);
    /// add all items of other queue at the end; will make a copy of each plottable
    /// @param otherQueue other queue to add
    void add(const Queue& otherQueue);
    /// add item at the beginning of this queue
    /// @param item item to add
    void addFront(std::shared_ptr<Plottable> item);
    
    /// helper method to add a circle
    /// \code{.cpp}
    /// q.add(Circle::create([...]);
    /// \endcode
    /// @param center circle center
    /// @param radius circle radius
    void circle(const Vec2& center, double radius);
    /// helper method to add a rectangle
    /// \code{.cpp}
    /// q.add(Rectangle::create([...]);
    /// \endcode
    /// @param bottomLeft rectangle bottom left
    /// @param topRight rectangle top right
    void rectangle(const Vec2& bottomLeft, const Vec2& topRight);
    /// helper method to add a path
    /// \code{.cpp}
    /// q.add(Path::create([...]);
    /// \endcode
    /// @param points list of points
    void path(const std::vector<Vec2>& points);
    /// helper method to add a small circle
    /// \code{.cpp}
    /// q.add(Circle::create([...], verySmallRadius);
    /// \endcode
    /// @param point dot location
    void dot(const Vec2& point);
    /// helper method to add a small dot on paper wiggling the pen
    void dotSpiral(const Vec2& point, double radius = 0.04);
    /// helper method to add a bezier curve
    /// \code{.cpp}
    /// q.add(Bezier::create([...]);
    /// \endcode
    /// @param points list of points
    void bezier(const std::vector<Vec2>& points);
    
    
    /// save as file to disk (.q)
    /// @param filePath path to save
    void save(std::string filePath) const;
    /// load a queue from file (.q)
    /// @param filePath path to the file to load
    static Queue load(std::string filePath);
    
    /// send to otterServer via network
    bool send() const;
    /// open otterUI application to preview
    void preview() const;
    
    
    /// offset all plottables
    /// @param dist offset vector
    void offset(const Vec2& dist);
    /// scale all plottables
    /// @param factor scale factor
    void scale(const double factor);
    /// scale all plottables at location
    /// @param pivot center point of scaling
    /// @param factor scale factor in both dimensions
    void scaleAt(const Vec2& pivot, double factor);
    /// rotate all plottables
    /// @param pivot rotation center
    /// @param angle angle of rotation
    void rotate(const Vec2& pivot, double angle);
    /// scale and move all plottables to fit into the target bounds
    /// @param targetBounds bounds to fit al plottables into
    /// @param moveToMid center the queue in the targetBounds
    void normalize(const Rectangle& targetBounds, bool moveToMid = true);
    
    /// optimize the to reduce travel time when plotting
    void optimize();
    /// combine plottables that end at the same point into a single plottable; reduces pen lifting and dropping
    void combinePlottables(const SampleSettings& sampleSettings = SampleSettings());
    
    /// get the combined bounds rectangle of all plottables
    Rectangle getBounds() const;
    /// find the path that defines the convex hull of all plottables
    std::vector<Vec2> getConvexHull(const SampleSettings& sampleSettings = SampleSettings()) const;
    /// get distances the pen needs to move to both paint and travel
    /// TODO: use struct for return
    /// @return array of size 2 with paint and travel distance
    const std::array<double, 2> getPaintAndTravelDistance() const;
    
    /// create new queue by discarding everything outside of mask
    /// @param mask defines the area, expected to be closed
    Queue getMaskedInside(const std::shared_ptr<Plottable>& mask) const;
    /// create new queue by discarding everything inside of mask
    /// @param mask defines the area, expected to be closed
    Queue getMaskedOutside(const std::shared_ptr<Plottable>& mask) const;
    

// serializable
    virtual void serialize(std::ostream& os) const override;
    virtual std::shared_ptr<Serializable> deserialize(const std::vector<std::string>& subElements) override;
    virtual std::string getName() const override;
    
// variables
    std::vector<std::shared_ptr<Plottable>> items;
    QueueSettings settings;
    
    
private:
    const std::array<Vec2, 2> getStartAndEndFromPlottalbe(const std::shared_ptr<Plottable> plottable) const;
    
    std::string getIp(const std::string& hostName, const std::string& port) const;
    bool sendMessage(const std::string& hostName, const std::string& port, const std::string& verb, const std::string& data, bool expectsReturn) const;
};

}
