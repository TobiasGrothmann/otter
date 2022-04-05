#pragma once
#include "stl.hpp"
#include "Color.hpp"
#include "../../Serialize/Serializable.hpp"

namespace otter {

/// definition for the pen that is used to plot the queue
class Pen : public Serializable
{
public:
    
    Pen() = default;
    Pen(const Color& color, double lineDiameter);
    
    Color color = Color(0, 0, 0);
    double lineDiameter = 0.05;
    
// presets
    static Pen blackDefault;
    
// serialize
    void serialize(std::ostream &os) const override;
    std::shared_ptr<Serializable> deserialize(const std::vector<std::string> &subElements) override;
    std::string getName() const override;
};

}
