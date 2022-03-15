#pragma once
#include "Settings.hpp"

namespace otter {

class SpeedDelayHandler
{
public:
    SpeedDelayHandler(float speedMin, float speedMax, float distPerStep)
        : speedMin(speedMin)
        , speedMax(speedMax)
        , distPerStep(distPerStep)
    {
    }

    void setSpeedFactor(float newSpeedFactor)
    {
        speedFactor = newSpeedFactor;
    }

    float getDelay()
    {
        const float speed = speedMin + (speedMax - speedMin) * speedFactor;
        return (distPerStep / speed) * 1000.0f * 1000.0f - 2.0f;
    }

private:
    float speedMin;
    float speedMax;

    float distPerStep;

    float speedFactor = 0.0f;
};

}
