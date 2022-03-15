#pragma once
#include "stl.hpp"
#include "Otter.hpp"
#include "Settings.hpp"

namespace otter {

struct SpeedPoint
{
    SpeedPoint(const Vec2& before, const Vec2& point, const Vec2& after, float edgeSlowdownPow)
    : point(point)
    {
        float angle = std::abs(
            std::atan2((float)(after.y - point.y), (float)(after.x - point.x))
             - std::atan2((float)(before.y - point.y), (float)(before.x - point.x)));
        if (angle > (float)M_PI)
        {
            angle = (float)M_PI - std::fmod(angle, (float)M_PI);
        }
        const float sharpness = std::fmod(std::abs(angle), 360.0f) / (float)M_PI;
        speed = std::pow(sharpness, edgeSlowdownPow);
    }

    SpeedPoint(const Vec2& point, float speed)
    : point(point)
    , speed(speed)
    {
    }

    Vec2 point;
    float speed;
};

struct AccellerationPath
{
public:
    AccellerationPath(const std::vector<Vec2>& pathPoints, float edgeSlowdownPow, float accellerationDist)
        : accellerationDist(accellerationDist)
    {
        std::vector<Vec2> pathPointsCleaned = {};
        pathPointsCleaned.reserve(pathPoints.size());
        for (VINDEX(pathPoints) i = 0; i < pathPoints.size(); i++)
        {
            if (i != 0 && pathPoints[i] == pathPoints[i - 1]) continue; // skip if point is the same as last
            pathPointsCleaned.push_back(pathPoints[i]);
        }

        std::vector<SpeedPoint> cornerPoints = {};
        cornerPoints.reserve(pathPointsCleaned.size());
        for (VINDEX(pathPointsCleaned) i = 0; i < pathPointsCleaned.size(); i++)
        {
            if (i != 0 && i != pathPointsCleaned.size() - 1)
            {
                cornerPoints.push_back(SpeedPoint(pathPointsCleaned[i - 1], pathPointsCleaned[i], pathPointsCleaned[i + 1], edgeSlowdownPow));
            }
            else
            {
                cornerPoints.push_back(SpeedPoint(pathPointsCleaned[i], 0.0f)); // start and end have to be speed 0
            }
        }
        doReverseSpeedPass(cornerPoints);
        points = ComputeInbetweenPoints(cornerPoints);
    }

    std::vector<SpeedPoint> ComputeInbetweenPoints(std::vector<SpeedPoint>& cornerPoints)
    {
        std::vector<SpeedPoint> outPoints = {};
        outPoints.reserve(cornerPoints.size() * 3);

        for (VINDEX(cornerPoints) i = 0; i < cornerPoints.size(); i++)
        {
            outPoints.push_back(cornerPoints[i]);
            if (i == cornerPoints.size() - 1) break;

            const Vec2& point = cornerPoints[i].point;
            const Vec2& next = cornerPoints[i + 1].point;

            const float segmentLength = (float)point.distance(next);
            const float maxAccellerationChange = segmentLength / accellerationDist;

            const float pointSpeed = cornerPoints[i].speed;
            const float nextSpeed = cornerPoints[i + 1].speed;

            const float pointNeededDistToMaxSpeed = (1.0f - pointSpeed) * accellerationDist;
            const float nextNeededDistToMaxSpeed = (1.0f - nextSpeed) * accellerationDist;

            if (segmentLength >= pointNeededDistToMaxSpeed + nextNeededDistToMaxSpeed)
            {
                const float pointFactorToPeak = pointNeededDistToMaxSpeed / segmentLength;
                const float nextFactorToPeak = nextNeededDistToMaxSpeed / segmentLength;
                outPoints.push_back(SpeedPoint(point + (next - point) * pointFactorToPeak, 1.0f));
                outPoints.push_back(SpeedPoint(next + (point - next) * nextFactorToPeak, 1.0f));
            }
            else if (segmentLength >= pointNeededDistToMaxSpeed && segmentLength >= pointNeededDistToMaxSpeed)
            {
                const float distNeededExceedingSegmentLength = (pointNeededDistToMaxSpeed + nextNeededDistToMaxSpeed) - segmentLength;
                const float pointDistToMidpoint = (pointNeededDistToMaxSpeed - distNeededExceedingSegmentLength * 0.5f);
                const float nextDistToMidpoint = (nextNeededDistToMaxSpeed - distNeededExceedingSegmentLength * 0.5f);
                const Vec2 midPoint = point + (next - point) * (pointDistToMidpoint / segmentLength);
                const float midSpeed = fmin(pointSpeed + (pointDistToMidpoint / accellerationDist), nextSpeed + (nextDistToMidpoint / accellerationDist));
                outPoints.push_back(SpeedPoint(midPoint, midSpeed));
            }
            else
            {
                if (nextSpeed < pointSpeed - maxAccellerationChange)
                {
                    cornerPoints[i + 1].speed = pointSpeed - maxAccellerationChange;
                }
                else if (nextSpeed > pointSpeed + maxAccellerationChange)
                {
                    cornerPoints[i + 1].speed = pointSpeed + maxAccellerationChange;
                }
            }
        }
        return outPoints;
    }

    void doReverseSpeedPass(std::vector<SpeedPoint>& accellerationPoints)
    {
        accellerationPoints[accellerationPoints.size() - 1].speed = 0.0f;
        for (VINDEX(accellerationPoints) i = accellerationPoints.size() - 1; i >= 1; i--)
        {
            const Vec2& point = accellerationPoints[i].point;
            const Vec2& next = accellerationPoints[i - 1].point;

            const float segmentLength = (float)point.distance(next);
            const float maxAccellerationChange = segmentLength / accellerationDist;

            const float pointSpeed = accellerationPoints[i].speed;
            const float nextSpeed = accellerationPoints[i - 1].speed;

            if (nextSpeed > pointSpeed + maxAccellerationChange)
            {
                accellerationPoints[i - 1].speed = pointSpeed + maxAccellerationChange;
            }
        }
    }

private:
    float accellerationDist;
public:

    std::vector<SpeedPoint> points = {};
};

}
