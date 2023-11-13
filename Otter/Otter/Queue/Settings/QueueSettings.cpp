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
        << speedHeadDownMin             << splitter
        << speedHeadDownMax             << splitter
        << speedHeadUpMin               << splitter
        << speedHeadUpMax               << splitter
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
    settings->speedHeadDownMin              = (float)Math::doubleFromString(subElements[5]);
    settings->speedHeadDownMax              = (float)Math::doubleFromString(subElements[6]);
    settings->speedHeadUpMin                = (float)Math::doubleFromString(subElements[7]);
    settings->speedHeadUpMax                = (float)Math::doubleFromString(subElements[8]);
    settings->accellerationDistFactorHead   = (float)Math::doubleFromString(subElements[9]);
    settings->speedTravelMin                = (float)Math::doubleFromString(subElements[10]);
    settings->speedTravelMax                = (float)Math::doubleFromString(subElements[11]);
    settings->accellerationDistTravel       = (float)Math::doubleFromString(subElements[12]);
    
    shared_ptr<Serializable> penSer = SerializableFactory::createPtr(subElements[13]);
    shared_ptr<Pen> item = dynamic_pointer_cast<Pen>(penSer);
    settings->pen = *item.get();
    
    return settings;
}

string QueueSettings::getName() const
{
    return "QueueSettings";
}

}
