#include "QueueState.hpp"

#include "ofMain.h"

using namespace otter;
using namespace std;


void QueueState::setQueue(const Queue& newQueue)
{
    this->queue = Queue(newQueue);
    
    queueBounds = newQueue.getBounds();
    
    displayedBounds = Rectangle(queueBounds);
    double margin = displayedBounds.getSize().max() * 0.08;
    displayedBounds.bottomLeft -= margin;
    displayedBounds.topRight += margin;
    
    recomputeScale();
}

void QueueState::recomputeScale()
{
    const ofVec2f ofWindowSize = ofGetWindowSize();
    windowTopLeft_screenSpace = Vec2(0, 0);
    windowSize_screenSpace = Vec2(ofWindowSize.x, ofWindowSize.y);
    
    scaleHandler.setScale(windowSize_screenSpace,
                          windowTopLeft_screenSpace,
                          displayedBounds);
}

void QueueState::SetMousePos(const Vec2& mousePos_screenSpace)
{
    mousePos_otterSpace = scaleHandler.scaleReverse(mousePos_screenSpace);
}

void QueueState::SetMouseClick(const otter::Vec2& mouseClick_screenSpace)
{
    mouseClick_otterSpace = scaleHandler.scaleReverse(mouseClick_screenSpace);
}
