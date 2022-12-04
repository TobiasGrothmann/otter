#include "QueueDrawer.hpp"
#include "ofMain.h"

using namespace otter;
using namespace std;


void QueueDrawer::draw(const State& queueState)
{
    ofClear(255, 255, 255);
    drawGrid(queueState);
    drawQueue(queueState);
}

void QueueDrawer::drawGrid(const State& queueState)
{
    ofSetLineWidth(1);
    const Vec2 minOtterSpace = queueState.scaleHandler.scaleReverse(Vec2(queueState.windowTopLeft_screenSpace) +
                                                                    queueState.windowSize_screenSpace.onlyY());
    const Vec2 maxOtterSpace = queueState.scaleHandler.scaleReverse(Vec2(queueState.windowTopLeft_screenSpace) +
                                                                    queueState.windowSize_screenSpace.onlyX());
    
    const float step = 1.0f;
    const int highlightStep = 10;
    
    double x = floor(minOtterSpace.x);
    while (x <= maxOtterSpace.x)
    {
        x += step;
        const int xInt = (int)std::round(x);
        ofSetColor(xInt % highlightStep != 0 ? colorGrid : colorGridHighlight);
        ofDrawLine(queueState.scaleHandler.scale(Vec2(x, minOtterSpace.y)),
                   queueState.scaleHandler.scale(Vec2(x, maxOtterSpace.y)));
    }
    double y = floor(minOtterSpace.y);
    while (y <= maxOtterSpace.y)
    {
        y += step;
        const int yInt = (int)std::round(y);
        ofSetColor(yInt % highlightStep != 0 ? colorGrid : colorGridHighlight);
        ofDrawLine(queueState.scaleHandler.scale(Vec2(minOtterSpace.x, y)),
                   queueState.scaleHandler.scale(Vec2(maxOtterSpace.x, y)));
    }
    
    ofSetColor(colorGridZero);
    ofDrawLine(queueState.scaleHandler.scale(Vec2(0, minOtterSpace.y)),
               queueState.scaleHandler.scale(Vec2(0, maxOtterSpace.y)));
    ofDrawLine(queueState.scaleHandler.scale(Vec2(minOtterSpace.x, 0)),
               queueState.scaleHandler.scale(Vec2(maxOtterSpace.x, 0)));
}

void QueueDrawer::drawQueue(const State& queueState)
{
    ofSetColor(0, 0, 0, 110);
    
    double lineWidth = queueState.scaleHandler.scale(queueState.queue.settings.pen.lineDiameter);
    ofSetLineWidth(lineWidth);
    
    for (const PlottablePtr& plottable : queueState.queue.items)
    {
        const vector<Vec2> points = plottable->getPlottablePoints(SampleSettings(30.0));
        
        ofPolyline poly = ofPolyline();
        for (const Vec2& point : points)
        {
            ofVec2f pointScaled = queueState.scaleHandler.scale(point);
            poly.addVertex(pointScaled.x, pointScaled.y, 0.0);
        }
        
//        poly.getSmoothed(10).draw();
        poly.draw();
    }
}
