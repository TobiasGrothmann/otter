#pragma once
#include "stl.hpp"

namespace otter { class Vec2; }

namespace otter {

/// random number utillity
class Rand
{
public:
    /// set a random seed based on current time
    static void seed();
    /// set a specific seed
    /// @param seed seed value
    static void seed(unsigned int seed);
    
    /// get uniform random double
    /// @param min minimum value included
    /// @param max included max value
    static double randF(double min, double max);
    /// get uniform random integer
    /// @param min minimum value included
    /// @param max included max value
    static int randI(int min, int max);
    
    /// get random value from normal distribution
    /// @param mean mean value / center value
    /// @param stdDeviation standart deviation
    static double normalDistValue(double mean, double stdDeviation);
    
    /// get random bool based on percentage
    static bool coin(double percentage = 0.5);
    /// get either -1.0 or 1.0 based on percentage
    static double sign(double percentage = 0.5);
    /// get uniform random double between 0.0 and 2*pi
    static double angle();

    template<typename T>
    /// get uniform random value from list
    /// @param list list of values to choose from
    static T choice(const std::vector<T>& list)
    {
        return list[Rand::randI(0, list.size() - 1)];
    }
    
    /// get 1D perlin value
    /// @param pos 2D location
    /// @param z optional additional z value
    static double perlin(const Vec2& pos, double z = 0.0);

private:
    static std::array<int, 512> p;
    
    // perlin
    static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    static double lerp(double t, double a, double b) { return a + t * (b - a); }
    static double grad(int hash, double x, double y, double z)
    {
        int h = hash & 15;
        double u = h<8 ? x : y, v = h<4 ? y : h==12||h==14 ? x : z;
        return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
    }
};

}
