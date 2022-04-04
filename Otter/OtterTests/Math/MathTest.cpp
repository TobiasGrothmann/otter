#include "catch_amalgamated.hpp"
#include "Otter.hpp"

using namespace std;
using namespace otter;

TEST_CASE("Math")
{
    SECTION("distanceBetweenIntsMod")
    {
        CHECK(Math::distanceBetweenIntsMod(0, 1, 10) == 1);
        CHECK(Math::distanceBetweenIntsMod(1, 0, 10) == 1);
        CHECK(Math::distanceBetweenIntsMod(9, 8, 10) == 1);
        CHECK(Math::distanceBetweenIntsMod(8, 9, 10) == 1);
        CHECK(Math::distanceBetweenIntsMod(0, 9, 10) == 1);
        CHECK(Math::distanceBetweenIntsMod(9, 0, 10) == 1);
        CHECK(Math::distanceBetweenIntsMod(9, 9, 10) == 0);
        CHECK(Math::distanceBetweenIntsMod(1, 4, 6) == 3);
        CHECK(Math::distanceBetweenIntsMod(1, 11, 10) == 0);
        CHECK(Math::distanceBetweenIntsMod(0, 19, 10) == 1);
    }
}
