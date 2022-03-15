#pragma once
#include "stl.hpp"
#include "SerializableFactory.hpp"

namespace otter {

class Serializable
{
public:
    virtual ~Serializable() = default;
    
    virtual void serialize(std::ostream& os) const = 0;
    virtual std::shared_ptr<Serializable> deserialize(const std::vector<std::string>& subElements) = 0;
    
    virtual std::string getName() const = 0;
    
    friend std::ostream& operator<<(std::ostream& os, const Serializable& object);
    friend std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Serializable>& object);
    friend std::ostream& operator<<(std::ostream& os, Serializable* object);
    friend std::ostream& operator<<(std::ostream& os, const Serializable* object);
    
    static bool parseElement(const std::string& element, std::string& name, std::vector<std::string>& subElements);
    
    static const char splitter = ';';
};

}
