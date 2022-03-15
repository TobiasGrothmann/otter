#pragma once
#include "stl.hpp"

namespace otter { class Vec2; }

namespace otter {

class Rand
{
public:
    static void seed();
    static void seed(unsigned int seed);
    
    static double randF(double min, double max);
    static int randI(int min, int max);
    
    static bool coin(double percentage = 0.5);
    static double sign(double percentage = 0.5);
    static double angle();

    template<typename T>
    static T choice(const std::vector<T>& list)
    {
        return list[Rand::randI(0, list.size() - 1)];
    }
    
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
