#include "Color.hpp"
#include "../../Math/Math.hpp"

using namespace std;

namespace otter {

Color::Color(double r, double g, double b, double a)
    : r(r)
    , g(g)
    , b(b)
    , a(a)
{
}

Color::Color(double grey, double a)
    : r(grey)
    , g(grey)
    , b(grey)
    , a(a)
{
}

double Color::getBrightness() const
{
    return ((r + b + g) / 3.0) * a;
}

void Color::serialize(ostream &os) const
{
    os << r << splitter << g << splitter << b << splitter << a;
}

shared_ptr<Serializable> Color::deserialize(const vector<string> &subElements)
{
    shared_ptr<Color> color = make_shared<Color>();
    color->r = Math::doubleFromString(subElements[0]);
    color->g = Math::doubleFromString(subElements[1]);
    color->b = Math::doubleFromString(subElements[2]);
    color->a = Math::doubleFromString(subElements[3]);
    return color;
}

string Color::getName() const
{
    return "Color";
}

}
