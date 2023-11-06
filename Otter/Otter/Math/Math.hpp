#pragma once
#include "stl.hpp"

namespace otter {

/// provides general math utility functions
class Math
{
public:
    
    template<class T>
    /// clamp value between min and max so that:   min <= value <= max
    /// @param value value to clamp
    /// @param minValue minimum return value
    /// @param maxValue maximum return value
    static T clamp(T value, T minValue, T maxValue)
    {
        return std::max(std::min(value, maxValue), minValue);
    }
    
    /// create double from string. Will return 0.0 if not parsable!
    /// TODO: bit silly to return 0.0 ;)
    /// @param value string value parsable to number
    static double doubleFromString(const std::string& value);
    
    /// convert radiants (one rotation: 0.0 -> 2*pi) to degree (one rotation: 0 - 360)
    /// @param rad radians value
    static double radToDeg(double rad);
    /// convert degree (one rotation: 0 - 360) to radiants (one rotation: 0.0 -> 2*pi)
    /// @param deg angle value in degree
    static double degToRad(double deg);
    
    /// round double to nearest int
    /// @param value value to be rounded
    static int round(double value);
    /// get sin value mapped between 0.0 to 1.0
    /// @param value angle value in radians
    static double sin2(double value);
    
    /// check if a is less than delta away from b
    /// @param a value a
    /// @param b value b
    /// @param delta delta value
    static bool isNearly(double a, double b, double delta = 0.00001);
    
    /// get the smallest distance between two integers in a modulo.
    /// examples:
    /// 1, 2 mod 5 -> 1 (steps 1 -> 2)
    /// 4, 5 mod 5 -> 1 (steps 4 -> 0)
    /// 0, 5 mod 5 -> 0 (no steps)
    /// 0, 3 mod 5 -> 2 (steps 0 -> 4 -> 3)
    /// @param a value a
    /// @param b value b
    /// @param mod modulo value
    static int distanceBetweenIntsMod(int a, int b, int mod);
};

}
