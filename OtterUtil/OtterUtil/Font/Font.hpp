#pragma once
#include "stl.hpp"
#include "Otter.hpp"

namespace otter { class Plottable; class Vec2; }
namespace TTFFontParser { struct FontData; }

namespace otter {

/// class to create paths for text using fonts
class Font
{
public:
    /// create a queue from text
    /// @param text string to create paths from
    /// @param fontHeight font height in cm
    /// @param lineDistFactor factor for the y distance for new lines
    /// @param fontPath path to font file (.ttf)
    static Queue getText(
                        const std::string& text,
                        double fontHeight,
                        double lineDistFactor = 1.333333,
                        const std::string& fontPath = "/Users/admin/Library/Fonts/cnc_v.ttf");
    
private:
    static std::vector<std::shared_ptr<Plottable>> getCharacter(const char character, TTFFontParser::FontData& fontData, double scalingFactor);
};

}
