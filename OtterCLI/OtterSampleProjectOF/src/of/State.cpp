#include "State.hpp"

#include "ofMain.h"

using namespace otter;
using namespace std;


void State::setQueue(const Queue& newQueue)
{
    this->queue = Queue(newQueue);
    
    queueBounds = newQueue.getBounds();
    
    displayedBounds = Rectangle(queueBounds);
    double margin = displayedBounds.getSize().max() * 0.08;
    displayedBounds.bottomLeft -= margin;
    displayedBounds.topRight += margin;
    
    recomputeScale();
    iteration++;
}

void State::recomputeScale()
{
    const ofVec2f ofWindowSize = ofGetWindowSize();
    windowTopLeft_screenSpace = Vec2(0, 0);
    windowSize_screenSpace = Vec2(ofWindowSize.x, ofWindowSize.y);
    
    scaleHandler.setScale(windowSize_screenSpace,
                          windowTopLeft_screenSpace,
                          displayedBounds);
}

void State::SetMousePos(const Vec2& mousePos_screenSpace)
{
    mousePos = scaleHandler.scaleReverse(mousePos_screenSpace);
}

void State::SetMouseClick(const otter::Vec2& mouseClick_screenSpace)
{
    Vec2 pos = scaleHandler.scaleReverse(mouseClick_screenSpace);
    mouseClick = pos;
    mousePos = pos;
}

void State::update()
{
    time = ofGetElapsedTimef();
}
