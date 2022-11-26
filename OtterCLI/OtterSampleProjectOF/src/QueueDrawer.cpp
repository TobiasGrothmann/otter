#include "QueueDrawer.hpp"
#include "ofMain.h"

using namespace otter;
using namespace std;


void QueueDrawer::draw(const Queue& q)
{
    const ofVec2f ofWindowSize = ofGetWindowSize();
    const Vec2 topLeft = Vec2(0, 0);
    const Vec2 windowSize = Vec2(ofWindowSize.x, ofWindowSize.y);
    
    // TODO: compute state only once
    Rectangle drawingBounds = q.getBounds();
    drawingBounds.scaleAt(drawingBounds.getCenter(), 1.05);
    
    scaleHandler.setScale(windowSize, topLeft, drawingBounds);
    
    ofClear(255, 255, 255);
    drawGrid(q, topLeft, windowSize);
    drawQueue(q);
}

void QueueDrawer::drawGrid(const Queue& q, const Vec2 topLeft, const Vec2& windowSize)
{
    const Vec2 minOtterSpace = scaleHandler.scaleReverse(Vec2(topLeft) + Vec2(0.0, windowSize.y));
    const Vec2 maxOtterSpace = scaleHandler.scaleReverse(Vec2(topLeft) + Vec2(windowSize.x, 0.0));
    
    const float step = 1.0f;
    const int highlightStep = 10;
    
    double x = floor(minOtterSpace.x);
    while (x <= maxOtterSpace.x)
    {
        x += step;
        const int xInt = (int)std::round(x);
        ofSetColor(xInt % highlightStep != 0 ? colorGrid : colorGridHighlight);
        ofDrawLine(scaleHandler.scale(Vec2(x, minOtterSpace.y)),
                   scaleHandler.scale(Vec2(x, maxOtterSpace.y)));
    }
    double y = floor(minOtterSpace.y);
    while (y <= maxOtterSpace.y)
    {
        y += step;
        const int yInt = (int)std::round(y);
        ofSetColor(yInt % highlightStep != 0 ? colorGrid : colorGridHighlight);
        ofDrawLine(scaleHandler.scale(Vec2(minOtterSpace.x, y)),
                   scaleHandler.scale(Vec2(maxOtterSpace.x, y)));
    }
    
    ofSetColor(colorGridZero);
    ofDrawLine(scaleHandler.scale(Vec2(0, minOtterSpace.y)),
               scaleHandler.scale(Vec2(0, maxOtterSpace.y)));
    ofDrawLine(scaleHandler.scale(Vec2(minOtterSpace.x, 0)),
               scaleHandler.scale(Vec2(maxOtterSpace.x, 0)));
}

void QueueDrawer::drawQueue(const Queue& q)
{
    ofSetColor(0, 0, 0);
    
    for (const PlottablePtr& plottable : q.items)
    {
        const vector<Vec2> points = plottable->getPlottablePoints(SampleSettings(30.0));
        
        ofPolyline poly = ofPolyline();
        for (const Vec2& point : points)
        {
            ofVec2f pointScaled = scaleHandler.scale(point);
            poly.addVertex(pointScaled.x, pointScaled.y, 0.0);
        }
        
        //poly.getSmoothed(1).draw();
        poly.draw();
    }
}
