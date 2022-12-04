#include "MainUI.hpp"
#include <iomanip>

#define GRAY(VAL) GetColorU32(ImVec4(VAL, VAL, VAL, 1.0f))
#define GRID_COL(VAL) GetColorU32(ImVec4((VAL) * 0.8f, VAL, (VAL) * 0.8f, 0.6f))
#define COLOR(R, G, B) GetColorU32(ImVec4(R, G, B, 1.0f))
#define COLOR_A(R, G, B, A) GetColorU32(ImVec4(R, G, B, A))
#define COLOR_HSV(H, S, V) GetColorU32((ImVec4)ImColor::HSV(H, S, V))

using namespace ImGui;
using namespace std;
using namespace otter;

MainUI* MainUI::Get()
{
    static MainUI* Instance = new MainUI();
    return Instance;
}

void MainUI::render()
{
    // CHECK STATE
    if (!state.isValid())
        return; // TODO
    
    // SET FULLSCREEN
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoSavedSettings;
    const ImGuiViewport* viewport = GetMainViewport();
    SetNextWindowPos(viewport->WorkPos);
    SetNextWindowSize(viewport->WorkSize);
    
//     DEV
//    ShowDemoWindow();

    // SET STYLE
    ImGuiStyle& style = GetStyle();
    const float roundingVal = 2.0f;
    style.WindowRounding = roundingVal;
    style.TabRounding = roundingVal;
    style.FrameRounding = roundingVal;
    style.ChildRounding = roundingVal;
    
#define SL SameLine()
    
    // SHOW UI
    if (Begin("OtterUI", nullptr, flags))
    {
        const ImVec2 settingsSize = ImVec2(330, 0);
        BeginChild("info", settingsSize, true);
        drawDetailsPanel(settingsSize);
        EndChild();
        SL;
        
        {
            BeginChild("canvas");
            
            const ImVec2 canvasOptionsSize = ImVec2(0, 23);
            BeginChild("canvasOptions", canvasOptionsSize, false);
            drawCanvasOptions(canvasOptionsSize);
            EndChild();
            
            const ImVec2 canvasSize = ImVec2(0, 0);
            BeginChild("queueCanvas", canvasSize, false);
            drawCanvas(canvasSize);
            EndChild();
            
            EndChild();
        }
    }
    End();
}

void MainUI::drawDetailsPanel(const ImVec2& size)
{
    const int sliderWidth = 170;
    
    TextCentered("INFO");
    Spacing();
    Text("FPS:     %.2f", GetIO().Framerate);
    Separator();
    Text("items:   %d", (int)round(state.queue->items.size()));
    Separator();
    
    Text("paint:   %.1f cm", state.paintDist);
    SL;
    SetCursorPosX(170);
    Text("%.f %%", (state.paintDist / (state.paintDist + state.travelDist)) * 100.0f);
    Text("travel:  %.1f cm", state.travelDist);
    SL;
    SetCursorPosX(170);
    Text("%.f %%", (state.travelDist / (state.paintDist + state.travelDist)) * 100.0f);
    
    Separator();
    NewLine();
    
    TextCentered("SETTINGS");
    Spacing();
    Text("DRAW");
    SetNextItemWidth(sliderWidth);
    SliderFloat("min speed (draw)", &state.queue->settings.speedDrawMin, 0.1f, 5.0f, "%.2f cm/s", ImGuiSliderFlags_AlwaysClamp);
    SetNextItemWidth(sliderWidth);
    SliderFloat("max speed (draw)", &state.queue->settings.speedDrawMax, 0.5f, 15.0f, "%.2f cm/s", ImGuiSliderFlags_AlwaysClamp);
    SetNextItemWidth(sliderWidth);
    SliderFloat("acc. distance (draw)", &state.queue->settings.accellerationDistDraw, 0.05f, 2.0f, "%.2f cm", ImGuiSliderFlags_AlwaysClamp);
    SetNextItemWidth(sliderWidth);
    SliderFloat("edge slowdown power", &state.queue->settings.edgeSlowdownPow, 1.0f, 100.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
    Spacing();
    Text("TRAVEL");
    SetNextItemWidth(sliderWidth);
    SliderFloat("min speed (trav)", &state.queue->settings.speedTravelMin, 0.1f, 5.0f, "%.2f cm/s", ImGuiSliderFlags_AlwaysClamp);
    SetNextItemWidth(sliderWidth);
    SliderFloat("max speed (trav)", &state.queue->settings.speedTravelMax, 5.0f, 40.0f, "%.2f cm/s", ImGuiSliderFlags_AlwaysClamp);
    SetNextItemWidth(sliderWidth);
    SliderFloat("acc. distance (trav)", &state.queue->settings.accellerationDistTravel, 0.1f, 2.0f, "%.2f cm", ImGuiSliderFlags_AlwaysClamp);
    NewLine();
    
    // keep maxSpeed >= minSpeed
    state.queue->settings.speedDrawMax = max(state.queue->settings.speedDrawMin, state.queue->settings.speedDrawMax);
    state.queue->settings.speedTravelMax = max(state.queue->settings.speedTravelMin, state.queue->settings.speedTravelMax);

    // commands
    const float buttonWidth = size.x - 20.0f;
    const float buttonRowWidth = buttonWidth / 3.0f - 5.33f;
    const float buttonHeight = 27.0f;
    
    Separator();
    NewLine();
    TextCentered("OPERATIONS");
    Spacing();
    if (Button("optimize path", ImVec2(buttonWidth, buttonHeight)))
    {
        state.queue->optimize();
        state.updateStats();
    }
    Spacing();
    if (Button("reverse queue", ImVec2(buttonWidth, buttonHeight)))
    {
        std::reverse(begin(state.queue->items), end(state.queue->items));
        state.updateStats();
    }
    Spacing();
    if (Button("combine plottables", ImVec2(buttonWidth, buttonHeight)))
    {
        state.queue->combinePlottables();
        state.updateStats();
    }
    NewLine();
    
    Separator();
    NewLine();
    TextCentered("COMMANDS");
    Spacing();
    if (Button("pause", ImVec2(buttonRowWidth, buttonHeight)))
    {
        Command::Pause.send();
    } SL;
    if (Button("resume", ImVec2(buttonRowWidth, buttonHeight)))
    {
        Command::Resume.send();
    } SL;
    if (Button("abort", ImVec2(buttonRowWidth, buttonHeight)))
    {
        Command::Abort.send();
    }
    Spacing();
    if (Button("send", ImVec2(buttonWidth, buttonHeight)))
    {
        state.queue->send();
    }
}

void MainUI::drawCanvasOptions(const ImVec2&)
{
    const ImVec2 buttonSize = ImVec2(120, 0);
    
    // travel
    string travelString = canvasOptions.drawTravel ? "travel" : "travel hidden";
    if (Button(travelString.c_str(), buttonSize))
    {
        canvasOptions.drawTravel = !canvasOptions.drawTravel;
    }
    
    // long lines mode
    SL;
    string travelModeString = canvasOptions.travelLongLinesMode ? "long lines" : "complete travel";
    if (Button(travelModeString.c_str(), buttonSize))
    {
        canvasOptions.travelLongLinesMode = !canvasOptions.travelLongLinesMode;
    }
    
    // info about hovered plottable
    SL;
    if (state.currentlyHoveredPlottable)
    {
        const double length = state.currentlyHoveredPlottable->getLength();
        const bool closed = state.currentlyHoveredPlottable->getIsClosed();
        std::stringstream ss;
        ss << setprecision(3);
        ss << state.currentlyHoveredPlottable->getName();
        ss << "; length: " << length << "cm";
        ss << "; " << (closed ? "closed" : "open");
        TextRightBound(ss.str());
    }
}

void MainUI::drawCanvas(const ImVec2&)
{
    ImDrawList* draw = GetWindowDrawList();
    const ImGuiIO& io = GetIO();
    
    const ImVec2 topLeft = GetWindowPos();
    const ImVec2 canvasSize = GetWindowSize();
    
    // setup scaling
    scaleHandler.setScale(canvasSize, topLeft, state.queueBounds);
    
    if (IsWindowFocused())
    {
        if (IsMouseDragging(ImGuiMouseButton_Left))
            scaleHandler.addScroll(io.MouseDelta);
        scaleHandler.addZoom(io.MouseWheel);
        if (IsMouseDoubleClicked(0))
        {
            scaleHandler.resetScroll();
            scaleHandler.resetZoom();
        }
    }
    
    // calculations
    const double lineThickness = state.queue->settings.pen.lineDiameter;
    const float lineThicknessScaled = (float)scaleHandler.scale(lineThickness);
    const float travelThickness = lineThicknessScaled;
    
    // colors
    static const ImU32 colorBackground = GRAY(1.0f);
    static const ImU32 colorGrid = GRID_COL(1.0f - 0.185f);
    static const ImU32 colorGridHighlight = GRID_COL(1.0f - 0.42f);
    static const ImU32 colorGridZero = GRID_COL(1.0f - 0.9f);
    static const ImU32 colorPen = GetColorU32(state.queue->settings.pen.color);
    static const ImU32 colorPenHovered = COLOR(1.0f, 0.0f, 0.0f);
    
    // background
    draw->AddRectFilled(topLeft, Vec2(topLeft) + Vec2(canvasSize), colorBackground);
    
    // draw grid
    const Vec2 minOtterSpace = scaleHandler.scaleReverse(Vec2(topLeft) + Vec2(0.0, canvasSize.y));
    const Vec2 maxOtterSpace = scaleHandler.scaleReverse(Vec2(topLeft) + Vec2(canvasSize.x, 0.0));
    
    const float step = 1.0f;
    const int highlightStep = 10;
    
    double x = floor(minOtterSpace.x);
    while (x <= maxOtterSpace.x)
    {
        x += step;
        const int xInt = (int)std::round(x);
        draw->AddLine(scaleHandler.scale(Vec2(x, minOtterSpace.y)),
                      scaleHandler.scale(Vec2(x, maxOtterSpace.y)),
                      xInt % highlightStep != 0 ? colorGrid : colorGridHighlight);
    }
    double y = floor(minOtterSpace.y);
    while (y <= maxOtterSpace.y)
    {
        y += step;
        const int yInt = (int)std::round(y);
        draw->AddLine(scaleHandler.scale(Vec2(minOtterSpace.x, y)),
                      scaleHandler.scale(Vec2(maxOtterSpace.x, y)),
                      yInt % highlightStep != 0 ? colorGrid : colorGridHighlight);
    }
    draw->AddLine(scaleHandler.scale(Vec2(0, minOtterSpace.y)), scaleHandler.scale(Vec2(0, maxOtterSpace.y)), colorGridZero);
    draw->AddLine(scaleHandler.scale(Vec2(minOtterSpace.x, 0)), scaleHandler.scale(Vec2(maxOtterSpace.x, 0)), colorGridZero);
    
    // draw queue
    Vec2 currentPos = Vec2(0, 0);
    double traveled = 0.0;
    double hue = 0.0;
    for (const PlottablePtr& p : state.queue->items)
    {
        const vector<Vec2> points = p->getPlottablePoints(SampleSettings(30.0, currentPos));
        if (points.size() <= 1) continue;
        
        if (canvasOptions.drawTravel)
        {
            const double travelDist = currentPos.distance(points[0]);
            const double travelRedMax = state.queueSize.max() * 0.25;
            if (canvasOptions.travelLongLinesMode)
                hue = 0.43 - (Math::clamp(travelDist, 0.0, travelRedMax) / travelRedMax) * 0.48;
            else
                hue = (traveled / state.travelDist) * 0.4f + 0.25f;
            draw->AddLine(scaleHandler.scale(currentPos), scaleHandler.scale(points[0]), COLOR_HSV((float)hue, 1.0f, 0.75f), travelThickness);
            traveled += currentPos.distance(points[0]);
            currentPos = points[points.size() - 1];
        }
        
        const bool isClosed = p->getIsClosed();
        const ImU32 colorUsed = p == state.currentlyHoveredPlottable ? colorPenHovered : colorPen;
        const ImDrawFlags drawFlags = isClosed ? ImDrawFlags_Closed : ImDrawFlags_None;
        
        const VINDEX(points) maxLengthPolyLine = 5000;
        VINDEX(points) currentIndex = 0;
        VINDEX(points) pointsLeft = points.size();
        while (pointsLeft > 0)
        {
            const VINDEX(points) pointsThisSection = min(maxLengthPolyLine, pointsLeft);
            
            ImVec2* imPoints = new ImVec2[pointsThisSection];
            VINDEX(points) cleanIndex = 0;
            for (VINDEX(points) j = 0; j < pointsThisSection; j++)
            {
                VINDEX(points) index = currentIndex + j;
                if (index != 0 && points[index] == points[index - 1])
                    continue; // skip repeating points
                imPoints[cleanIndex] = scaleHandler.scale(points[index]);
                cleanIndex++;
            }
            
            pointsLeft -= pointsThisSection;
            currentIndex += pointsThisSection;
            
            int usedSize = isClosed ? (int)cleanIndex - 1 : (int)cleanIndex; // don't draw first/last point twice
            draw->AddPolyline(imPoints, usedSize, colorUsed, drawFlags, lineThicknessScaled);
            delete[] imPoints;
        }
    }
    if (canvasOptions.drawTravel)
        draw->AddLine(scaleHandler.scale(currentPos), scaleHandler.scale(Vec2(0, 0)), COLOR_HSV((float)hue, 1.0f, 0.75f), travelThickness);
    
    // check what plottable is hovered
    const Vec2 hoveredPos = scaleHandler.scaleReverse(GetMousePos());
    const QuadTreeVec closestPointOnPlottable = state.quadTree.getClosestPoint(hoveredPos);
    state.currentlyHoveredPlottable = closestPointOnPlottable.plottable;
    if (hoveredPos.distance(closestPointOnPlottable.point) > 0.25)
    {
        state.currentlyHoveredPlottable = nullptr;
    }
}

void MainUI::TextRightBound(const string& text)
{
    NextColumn();
    float posX = GetCursorPosX() + GetColumnWidth() - CalcTextSize(text.c_str()).x - GetScrollX() - 2.0f * GetStyle().ItemSpacing.x;
    if (posX > GetCursorPosX())
        SetCursorPosX(posX);
    Text("%s", text.c_str());
}

void MainUI::TextCentered(const string& text)
{
    NextColumn();
    float posX = GetCursorPosX() + GetColumnWidth() * 0.5f - CalcTextSize(text.c_str()).x * 0.5f - GetScrollX() - GetStyle().ItemSpacing.x;
    if (posX > GetCursorPosX())
        SetCursorPosX(posX);
    Text("%s", text.c_str());
}
