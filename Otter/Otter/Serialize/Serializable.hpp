#pragma once
#include "stl.hpp"
#include "SerializableFactory.hpp"

namespace otter {

/// interface for serialization and deserialization to and from string
class Serializable
{
public:
    virtual ~Serializable() = default;
    
    /// serialize this object into os; Serializable::splitter is used to separate elements
    virtual void serialize(std::ostream& os) const = 0;
    /// deserialize object from list of strings
    virtual std::shared_ptr<Serializable> deserialize(const std::vector<std::string>& subElements) = 0;
    
    /// get the unique name that is used to define this class
    virtual std::string getName() const = 0;
    
    friend std::ostream& operator<<(std::ostream& os, const Serializable& object);
    friend std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Serializable>& object);
    friend std::ostream& operator<<(std::ostream& os, Serializable* object);
    friend std::ostream& operator<<(std::ostream& os, const Serializable* object);
    
    static bool parseElement(const std::string& element, std::string& name, std::vector<std::string>& subElements);
    
    static const char splitter = ';';
};

}
