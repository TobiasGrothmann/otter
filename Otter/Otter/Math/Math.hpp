#pragma once
#include "stl.hpp"

namespace otter {

class Math
{
public:
    
    template<class T>
    static T clamp(T value, T minValue, T maxValue)
    {
        return std::max(std::min(value, maxValue), minValue);
    }
    
    static double doubleFromString(std::string value);
    
    static double radToDeg(double rad);
    static double degToRad(double deg);
    
    static int round(double value);
    static double sin2(double value);
    
    static bool isNearly(double a, double b, double delta = 0.00001);
    
    static int distanceBetweenIntsMod(int a, int b, int mod);
};

}
