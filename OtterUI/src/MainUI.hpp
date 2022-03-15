#pragma once
#include "core.hpp"
#include "State.hpp"
#include "ScaleHandler.hpp"

struct CanvasOptions
{
    bool drawTravel = true;
    bool travelLongLinesMode = true;
};

class MainUI
{
public:
    MainUI() = default;
    
    static MainUI* Get();
    void render();
    
    State state = {};
    
private:
    ScaleHandler scaleHandler = {};
    
    CanvasOptions canvasOptions = {};
    
    void drawDetailsPanel(const ImVec2& size);
    void drawCanvasOptions(const ImVec2& size);
    void drawCanvas(const ImVec2& size);
    
    void TextRightBound(const std::string& text);
    void TextCentered(const std::string& text);
};
