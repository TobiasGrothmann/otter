#include "Font.hpp"
#include "Otter.hpp"
#define TTF_FONT_PARSER_IMPLEMENTATION
#include "ttfParser/ttfParser.h"

using namespace std;

namespace otter {

void parse(void* args, void* fontData, int error) {
    if (error) {
        *(uint8_t*)args = error;
        cout << "ERROR: Unable to parse font" << endl;
    }
    else {
        *(uint8_t*)args = 1;
    }
}

TTFFontParser::FontData getFontData(const string& fontPath)
{
    uint8_t finished = 0;
    TTFFontParser::FontData fontData;
    /*int8_t error = */TTFFontParser::parse_file(fontPath.c_str(), &fontData, &parse, &finished);
    while (!finished) {}
    return fontData;
}

Queue Font::getText(
                    const string& text,
                    double fontHeight,
                    double lineDistFactor,
                    const string& fontPath
                    )
{
    Queue output = Queue();
    Vec2 writeHead = Vec2(0,0);
    TTFFontParser::FontData fontData = getFontData(fontPath);
    const double scalingFactor = (1.0 / 1450.0) * fontHeight;
    for (const char& character : text)
    {
        if (character == '\n')
        {
            writeHead.x = 0;
            writeHead.y -= fontHeight * lineDistFactor;
            continue;
        }
        
        char usedChar = character;
        vector<shared_ptr<Plottable>> newPlottables = getCharacter(usedChar, fontData, scalingFactor);
        for (shared_ptr<Plottable> plottable : newPlottables)
        {
            plottable->offset(writeHead);
        }
        const TTFFontParser::Glyph& g = fontData.glyphs[usedChar];
        
        writeHead.x += g.advance_width * scalingFactor - g.left_side_bearing * scalingFactor * 0.0;
        output.add(newPlottables);
    }
    return output;
}

vector<shared_ptr<Plottable>> Font::getCharacter(const char character, TTFFontParser::FontData& fontData, double scalingFactor)
{
    vector<shared_ptr<Plottable>> output = {};
    
    const TTFFontParser::Glyph& g = fontData.glyphs[character];
    for (const TTFFontParser::Path& path : g.path_list)
    {
        shared_ptr<Path> p = make_shared<Path>();
        for (int c = 0; c < path.geometry.size(); c++)
        {
            const TTFFontParser::Curve& curve = path.geometry[c];
            const Vec2 startPoint = Vec2(curve.p0.x * scalingFactor, curve.p0.y * scalingFactor);
            const Vec2 endPoint = Vec2(curve.p1.x * scalingFactor, curve.p1.y * scalingFactor);
            p->add(startPoint);
            
            if (curve.is_curve)
            {
                const Vec2& controlPoint = Vec2(curve.c.x * scalingFactor, curve.c.y * scalingFactor);
                BezierPtr b = Bezier::create({ startPoint, endPoint, controlPoint }); // I'm assuming the control point should be in the middle but that's not the case here
                p->add(b->getPlottablePoints(SampleSettings(30.0, startPoint)));
            }
            else if (c == path.geometry.size() - 1)
            {
                p->add(endPoint);
            }
        }
        output.push_back(p);
    }
    
    return output;
}

}
