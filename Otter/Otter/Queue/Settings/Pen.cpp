#include "Pen.hpp"
#include "../../Math/Math.hpp"

using namespace std;

namespace otter {

Pen Pen::blackDefault = Pen(Color(0.0, 0.0, 0.0, 0.8), 0.05);

Pen::Pen(const Color& color, double lineDiameter)
    : color(color)
    , lineDiameter(lineDiameter)
{
}

void Pen::serialize(ostream &os) const
{
    os << color << splitter << lineDiameter;
}

shared_ptr<Serializable> Pen::deserialize(const vector<string> &subElements)
{
    shared_ptr<Pen> pen = make_shared<Pen>();
    
    shared_ptr<Serializable> itemSer = SerializableFactory::createPtr(subElements[0]);
    shared_ptr<Color> item = dynamic_pointer_cast<Color>(itemSer);
    pen->color = *item.get();
    
    pen->lineDiameter = Math::doubleFromString(subElements[1]);
    return pen;
}

string Pen::getName() const
{
    return "Pen";
}

}
