#include "Rand.hpp"
#include "../Geometry/Vec2.hpp"
#include <random>

using namespace std;

namespace otter {

void Rand::seed()
{
    srand((unsigned int) time(0));
}

void Rand::seed(unsigned int seed)
{
    srand(seed);
    shuffle(p.begin(), p.end(), default_random_engine());
}

double Rand::randF(double min, double max)
{
    return ((double) rand() / (double) (RAND_MAX / (max - min))) + min;
}

int Rand::randI(int min, int max)
{
    return (int) randF(min, max + 1);
}

double Rand::normalDistValue(double mean, double stdDeviation)
{
    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<> dist(mean, stdDeviation);
    return dist(e2);
}

bool Rand::coin(double percentage)
{
    return ((double) rand() / (double) (RAND_MAX)) < percentage;
}

double Rand::angle()
{
    return randF(0, 2.0 * M_PI);
}

double Rand::sign(double percentage)
{
    return Rand::coin(percentage) ? -1.0 : 1.0;
}

double Rand::perlin(const Vec2& pos, double z)
{
    using ST = array<int, 512>::size_type;
    
    double x = pos.x;
    double y = pos.y;
    int X = (int)floor(x) & 255,
        Y = (int)floor(y) & 255,
        Z = (int)floor(z) & 255;
        x -= floor(x);
        y -= floor(y);
        z -= floor(z);
    double  u = fade(x),
            v = fade(y),
            w = fade(z);
    int A = p[(ST)X  ]+Y, AA = p[(ST)A]+Z, AB = p[(ST)A+1]+Z,
        B = p[(ST)X+1]+Y, BA = p[(ST)B]+Z, BB = p[(ST)B+1]+Z;

    return lerp(w, lerp(v,  lerp(u, grad(p[(ST)AA  ], x  , y  , z   ),
                                    grad(p[(ST)BA  ], x-1, y  , z   )),
                            lerp(u, grad(p[(ST)AB  ], x  , y-1, z   ),
                                    grad(p[(ST)BB  ], x-1, y-1, z   ))),
                    lerp(v, lerp(u, grad(p[(ST)AA+1], x  , y  , z-1 ),
                                    grad(p[(ST)BA+1], x-1, y  , z-1 )),
                            lerp(u, grad(p[(ST)AB+1], x  , y-1, z-1 ),
                                    grad(p[(ST)BB+1], x-1, y-1, z-1 ))));
}

array<int, 512> Rand::p =
{
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

}
