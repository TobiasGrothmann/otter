#include "QueueSettings.hpp"
#include "../../Math/Math.hpp"

using namespace std;

namespace otter {

QueueSettings::QueueSettings()
    : pen(Pen::blackDefault)
{
}

void QueueSettings::serialize(ostream &os) const
{
    os  << speedDrawMin                 << splitter
        << speedDrawMax                 << splitter
        << accellerationDistDraw        << splitter
        << edgeSlowdownPow              << splitter
        << penPressure                  << splitter
        << speedHeadMin                 << splitter
        << speedHeadMax                 << splitter
        << accellerationDistFactorHead  << splitter
        << speedTravelMin               << splitter
        << speedTravelMax               << splitter
        << accellerationDistTravel      << splitter
        << pen;
}

shared_ptr<Serializable> QueueSettings::deserialize(const vector<string> &subElements)
{
    shared_ptr<QueueSettings> settings = make_shared<QueueSettings>();
    settings->speedDrawMin                  = (float)Math::doubleFromString(subElements[0]);
    settings->speedDrawMax                  = (float)Math::doubleFromString(subElements[1]);
    settings->accellerationDistDraw         = (float)Math::doubleFromString(subElements[2]);
    settings->edgeSlowdownPow               = (float)Math::doubleFromString(subElements[3]);
    settings->penPressure                   = (float)Math::doubleFromString(subElements[4]);
    settings->speedHeadMin                  = (float)Math::doubleFromString(subElements[5]);
    settings->speedHeadMax                  = (float)Math::doubleFromString(subElements[6]);
    settings->accellerationDistFactorHead   = (float)Math::doubleFromString(subElements[7]);
    settings->speedTravelMin                = (float)Math::doubleFromString(subElements[8]);
    settings->speedTravelMax                = (float)Math::doubleFromString(subElements[9]);
    settings->accellerationDistTravel       = (float)Math::doubleFromString(subElements[10]);
    
    shared_ptr<Serializable> itemSer = SerializableFactory::createPtr(subElements[11]);
    shared_ptr<Pen> item = dynamic_pointer_cast<Pen>(itemSer);
    settings->pen = *item.get();
    
    return settings;
}

string QueueSettings::getName() const
{
    return "QueueSettings";
}

}
