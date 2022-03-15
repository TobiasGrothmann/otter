#pragma once
#include "stl.hpp"
#include "../Geometry/Vec2.hpp"
#include "../Plottables/Plottable.hpp"
#include "../Serialize/Serializable.hpp"
#include "Settings/QueueSettings.hpp"

namespace otter {

class Queue : public Serializable {
public:
    Queue() = default;
    Queue(const Queue& other);
    Queue(Queue&& other);
    Queue(const std::vector<std::shared_ptr<Plottable>>& items);
    Queue& operator=(const Queue& other);
    Queue& operator=(Queue&& other);
    
    void add(std::shared_ptr<Plottable> newItem);
    void add(const std::vector<std::shared_ptr<Plottable>>& newItems);
    void add(const Queue& otherQueue);
    void addFront(std::shared_ptr<Plottable> item);
    
    void circle(const Vec2& center, double radius);
    void rectangle(const Vec2& bottomLeft, const Vec2& topRight);
    void path(const std::vector<Vec2>& points);
    void dot(const Vec2& point);
    void bezier(const std::vector<Vec2>& points);
    
    
    void save(std::string filePath) const;
    static Queue load(std::string filePath);
    
    bool send() const;
    void preview() const;
    
    
    void offset(const Vec2& dist);
    void scale(const double factor);
    void rotate(const Vec2& pivot, double angle);
    void normalize(const Rectangle& targetBounds, bool moveToMid = true);
    
    void optimize();
    void combinePlottables(const SampleSettings& sampleSettings = SampleSettings());
    
    Rectangle getBounds() const;
    std::vector<Vec2> getConvexHull(const SampleSettings& sampleSettings = SampleSettings()) const;
    const std::vector<double> getPaintAndTravelDistance() const;
    
    Queue getMaskedInside(const std::shared_ptr<Plottable>& mask) const;
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
