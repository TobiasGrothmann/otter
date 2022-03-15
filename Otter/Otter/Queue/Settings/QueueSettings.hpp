#pragma once
#include "stl.hpp"
#include "Pen.hpp"
#include "../../Serialize/Serializable.hpp"

namespace otter {

class QueueSettings : public Serializable
{
public:
    QueueSettings();
    
// settings
    // drawing
    float speedDrawMin = 0.75f;                 // cm / second
    float speedDrawMax = 4.5f;                  // cm / second
    float accellerationDistDraw = 0.22f;        // cm
    float edgeSlowdownPow = 10.0f;              // the bigger, the less speed at edges
    
    // z axis
    float penPressure = 0.825f;                 // between 0.0f to 1.0f
    float speedHeadMin = 1.8f;                  // cm / second
    float speedHeadMax = 20.0f;                 // cm / second
    float accellerationDistFactorHead = 0.17f;  // between 0.0f and 0.5f, fraction of distance used to accellerate/decellerate
    
    // travel
    float speedTravelMin = 4.0f;                // cm / second
    float speedTravelMax = 28.0f;               // cm / second
    float accellerationDistTravel = 0.25f;      // cm
    
    Pen pen;
    
// serialize
    void serialize(std::ostream &os) const override;
    std::shared_ptr<Serializable> deserialize(const std::vector<std::string> &subElements) override;
    std::string getName() const override;
};

}
