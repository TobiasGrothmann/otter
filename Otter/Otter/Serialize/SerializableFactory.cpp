#include "SerializableFactory.hpp"
#include "../Queue/Queue.hpp"
#include "../Geometry/Vec2.hpp"
#include "../Plottables/Path.hpp"
#include "../Plottables/Circle.hpp"
#include "../Plottables/Rectangle.hpp"
#include "../Plottables/Bezier.hpp"
#include "../Queue/Command/Command.hpp"
#include "Serializable.hpp"

using namespace std;

namespace otter {

shared_ptr<SerializableFactory> SerializableFactory::instance = nullptr;

SerializableFactory::SerializableFactory()
{
    possibleObjects =
    {
        make_shared<Queue>(),
        make_shared<QueueSettings>(),
        make_shared<Vec2>(),
        make_shared<Path>(),
        make_shared<Circle>(),
        make_shared<Rectangle>(),
        make_shared<Bezier>(),
        make_shared<Pen>(),
        make_shared<Color>(),
        make_shared<Command>(),
    };
    for (shared_ptr<Serializable>& object : possibleObjects)
    {
        objectByName[object->getName()] = object;
    }
}

shared_ptr<SerializableFactory> SerializableFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = make_shared<SerializableFactory>();
    }
    return instance;
}





shared_ptr<Serializable> SerializableFactory::createPtr(const string& elementString)
{
    string name = {};
    vector<string> subElements = {};
    bool success = Serializable::parseElement(elementString, name, subElements);
    if (!success)
    {
        cout << "ERROR: failed to parse element" << endl;
        return nullptr;
    }
    
    shared_ptr<SerializableFactory> factory = SerializableFactory::GetInstance();
    if (factory->objectByName.find(name) == factory->objectByName.end())
    {
        cout << "ERROR: failed to deserialize: '" << name << "' in: '" << elementString << "'" << endl;
        return nullptr;
    }
    shared_ptr<Serializable> correctObj = factory->objectByName[name];
    return correctObj->deserialize(subElements);
}

void SerializableFactory::registerSerializableObject(shared_ptr<Serializable> possibleObject)
{
    shared_ptr<SerializableFactory> factory = SerializableFactory::GetInstance();
    factory->possibleObjects.push_back(possibleObject);
    factory->objectByName[possibleObject->getName()] = possibleObject;
}

}
