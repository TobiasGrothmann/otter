#include "ScaleHandler.hpp"

using namespace std;
using namespace otter;


void ScaleHandler::setScale(const Vec2& inCanvasSize, const Vec2& inCanvasPos, const Rectangle& inQueueBounds)
{
    canvasSize_screen = inCanvasSize;
    canvasPos_screen = inCanvasPos;
    queueBounds = inQueueBounds;
    
    const Vec2 queueSize = queueBounds.getSize();
    if (queueSize.x / queueSize.y > canvasSize_screen.x / canvasSize_screen.y) // higher aspect ratio of queue than canvas
    {
        queueScale = canvasSize_screen.x / queueSize.x;
        const double pixelSizeAtVertEdge = canvasSize_screen.y - queueSize.y * queueScale;
        queueOffset = Vec2(0.0, -pixelSizeAtVertEdge * 0.5);
    }
    else
    {
        queueScale = canvasSize_screen.y / queueSize.y;
        const double pixelSizeAtHorEdge = canvasSize_screen.x - queueSize.x * queueScale;
        queueOffset = Vec2(pixelSizeAtHorEdge * 0.5, 0.0);
    }
    
    zoomCenter_screen = canvasSize_screen * Vec2(0.8, 0.5);
}

void ScaleHandler::addScroll(const Vec2& offset)
{
    scrolling_screen += offset / (1.0 + zoom);
}

void ScaleHandler::resetScroll()
{
    scrolling_screen = Vec2(0, 0);
}

void ScaleHandler::addZoom(double offset)
{
    zoom = Math::clamp(zoom + offset, 0.0, 8.0);
}

void ScaleHandler::resetZoom()
{
    zoom = 0.0;
}

ofVec2f ScaleHandler::scale(const Vec2& otterPoint) const
{
    // other
    const Vec2 movedToShowOrigin = otterPoint - queueBounds.bottomLeft;
    const Vec2 screenSpace = movedToShowOrigin * queueScale;
    const Vec2 offset = screenSpace + Vec2(canvasPos_screen.x, -canvasPos_screen.y);
    const Vec2 flipped = Vec2(offset.x, canvasSize_screen.y - offset.y);
    const Vec2 centered = flipped + queueOffset;
    // scrolling
    const Vec2 scrolled = centered + scrolling_screen;
    // zoom
    const Vec2 zoomed = (scrolled - zoomCenter_screen) * (1.0 + zoom) + zoomCenter_screen;
    return ofVec2f(zoomed.x, zoomed.y);
}

double ScaleHandler::scale(double distance) const
{
    return distance * (1.0 + zoom) * queueScale;
}

Vec2 ScaleHandler::scaleReverse(const Vec2& screenPoint) const
{
    // reverse zooming
    const Vec2 zoomed = (screenPoint - zoomCenter_screen) / (1.0 + zoom) + zoomCenter_screen;
    // reverse scrolling
    const Vec2 withoutScrolling = zoomed - scrolling_screen;
    // reverse other
    const Vec2 atCenter = withoutScrolling - queueOffset;
    const Vec2 flipped = Vec2(atCenter.x, canvasSize_screen.y - atCenter.y);
    const Vec2 offset = flipped - Vec2(canvasPos_screen.x, -canvasPos_screen.y);
    const Vec2 otterPoint = offset / queueScale;
    const Vec2 movedToOriginalPlace = otterPoint + queueBounds.bottomLeft;
    return movedToOriginalPlace;
}

double ScaleHandler::scaleReverse(double distance) const
{
    return distance / (1.0 + zoom) / queueScale;
}
