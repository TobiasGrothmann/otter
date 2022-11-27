#include "ofApp.h"

#include "QueueDrawer.hpp"

using namespace otter;
using namespace std;


//--------------------------------------------------------------
void ofApp::setup()
{
    this->sketch = Sketch();
    createQueue();
}

//--------------------------------------------------------------
void ofApp::update()
{
    
}

//--------------------------------------------------------------
void ofApp::draw()
{
    queueDrawer.draw(queueState);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == ' ')
        createQueue();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    queueState.SetMousePos(Vec2(x, y));
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    queueState.SetMouseClick(Vec2(x, y));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    queueState.SetMouseClick(Vec2(x, y));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    queueState.SetMouseClick(Vec2(x, y));
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
    queueState.SetMousePos(Vec2(x, y));
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    queueState.recomputeScale();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}


void ofApp::createQueue()
{
    this->queueState.setQueue(this->sketch.create(queueState));
}
