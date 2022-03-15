#pragma once
#include "stl.hpp"
#include "../../Serialize/Serializable.hpp"

namespace otter {

class Color : public Serializable
{
public:
    Color() = default;
    Color(double r, double g, double b, double a = 1.0);
    Color(double grey, double a = 1.0);
    
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;
    double a = 1.0;
    
    double getBrightness() const;
    
// serialize
    void serialize(std::ostream &os) const override;
    std::shared_ptr<Serializable> deserialize(const std::vector<std::string> &subElements) override;
    std::string getName() const override;
};

}
