#include "Math.hpp"

using namespace std;

namespace otter {

double Math::doubleFromString(string value)
{
    try {
        return stod(value);
    } catch (out_of_range e) {
        return 0.0;
    }
}

double Math::radToDeg(double rad)
{
    return (rad / (M_PI * 2.0)) * 360.0;
}

double Math::degToRad(double deg)
{
    return (deg / 360.0) * (2.0 * M_PI);
}

int Math::round(double value)
{
    return (int)lround(value);
}

double Math::sin2(double value)
{
    return sin(value) * 0.5 + 0.5;
}

bool Math::isNearly(double a, double b, double delta)
{
    return abs(a - b) < delta;
}

int Math::distanceBetweenIntsMod(int a, int b, int mod)
{
    int aMod = a % mod;
    int bMod = b % mod;
    int minValue = min(aMod, bMod);
    int maxValue = max(aMod, bMod);
    int distValue = maxValue - minValue;
    int modValue = mod - maxValue + minValue;
    return min(distValue, modValue);
}

}
