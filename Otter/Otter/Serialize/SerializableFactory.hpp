#pragma once
#include "stl.hpp"

namespace otter {

class Serializable;

class SerializableFactory
{
public:
    
    SerializableFactory();
    static std::shared_ptr<SerializableFactory> GetInstance();
private:
    static std::shared_ptr<SerializableFactory> instance;
public:
    
    static std::shared_ptr<Serializable> createPtr(const std::string& elementString);
    
    static void registerSerializableObject(std::shared_ptr<Serializable> possibleObject);
    
private:
    std::vector<std::shared_ptr<Serializable>> possibleObjects = {};
    std::map<std::string, std::shared_ptr<Serializable>> objectByName = {};
};

}
