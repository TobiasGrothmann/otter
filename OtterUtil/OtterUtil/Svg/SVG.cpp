#include "SVG.hpp"
#include "Otter.hpp"
#define NANOSVG_IMPLEMENTATION
#include "nanosvg/nanosvg.h"
#include "simple_svg/simple_svg_1.0.0.hpp"

using namespace std;

namespace otter {

bool SVG::read(const string& path, Queue& outQueue)
{
    NSVGimage* image = NULL;

    image = nsvgParseFromFile(path.c_str(), "cm", 96.0f);

    if (image == NULL) {
        return false;
    }

    NSVGshape* shape = image->shapes;
    while (shape != nullptr)
    {
        const Rectangle shapeBounds = Rectangle(Vec2(shape->bounds[0], shape->bounds[1]),
                                                Vec2(shape->bounds[2], shape->bounds[3]));
        NSVGpath* segment = shape->paths;
        while (segment != nullptr)
        {
            shared_ptr<Path> newPath = make_shared<Path>();
            for (int i = 0; i < segment->npts-1; i += 3) {
                float* p = &segment->pts[i * 2];
                Bezier bezier = Bezier::create({
                    Vec2(p[0], -p[1]),
                    Vec2(p[2], -p[3]),
                    Vec2(p[4], -p[5]),
                    Vec2(p[6], -p[7])
                });
                newPath->add(bezier.getPlottablePoints());
            }
            if (segment->closed && newPath->points.size() >= 1)
            {
                newPath->add(newPath->points[newPath->points.size() - 1]);
            }

            outQueue.add(newPath);
            segment = segment->next;
        }
        shape = shape->next;
    }

    return true;
}


bool SVG::write(const Queue& queue, const string& path, double scale, double strokeWidth)
{
    Rectangle bounds = queue.getBounds();
    bounds.bottomLeft -= bounds.getSize() * 0.1; // add some margin
    bounds.topRight += bounds.getSize() * 0.1;
    const Vec2 offset = bounds.bottomLeft * -1.0; // offset svg origin to bottom left
    
    std::filesystem::path p(path);
    std::filesystem::path parentDir = p.parent_path();
    if (std::filesystem::create_directory(parentDir))
    {
        cout << "WARNING: created directory: " << parentDir << endl;
    }
    
    svg::Dimensions dimensions(bounds.getWidth() * scale, bounds.getHeight() * scale);
    svg::Document document(path, svg::Layout(dimensions, svg::Layout::BottomLeft));
    
    for (const shared_ptr<Plottable>& item : queue.items)
    {
        if (item->getIsClosed())
        {
            svg::Polygon polygon = svg::Polygon(svg::Stroke(strokeWidth, svg::Color::Black));
            const vector<Vec2>& points = item->getPlottablePoints();
            for (int i = 0; i < points.size() - 1; i++)
            {
                polygon << svg::Point((points[i].x + offset.x) * scale, (points[i].y + offset.y) * scale);
            }
            document << polygon;
        }
        else
        {
            svg::Polyline polyLine = svg::Polyline(svg::Stroke(strokeWidth, svg::Color::Black));
            for (const Vec2& point : item->getPlottablePoints())
            {
                polyLine << svg::Point((point.x + offset.x) * scale, (point.y + offset.y) * scale);
            }
            document << polyLine;
        }
    }
    document.save();
    
    return true;
}

}
