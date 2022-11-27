#include "ofApp.h"

#include "QueueDrawer.hpp"

using namespace otter;
using namespace std;


//--------------------------------------------------------------
void ofApp::setup()
{
    this->sketch = Sketch();
    this->sketch.setup();
    createQueue();
}

//--------------------------------------------------------------
void ofApp::update()
{
    state.update();
    if (sketch.updateEveryFrame)
        createQueue();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    state.update();
    queueDrawer.draw(state);
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
    state.SetMousePos(Vec2(x, y));
    if (sketch.updateOnMouseMove)
        createQueue();
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    state.SetMouseClick(Vec2(x, y));
    if (sketch.updateOnMouseClick)
        createQueue();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    state.SetMouseClick(Vec2(x, y));
    if (sketch.updateOnMouseClick)
        createQueue();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    state.SetMouseClick(Vec2(x, y));
    if (sketch.updateOnMouseClick)
        createQueue();
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
    state.SetMousePos(Vec2(x, y));
    if (sketch.updateOnMouseMove)
        createQueue();
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
    if (sketch.updateOnMouseMove)
        createQueue();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    state.recomputeScale();
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
    this->state.setQueue(this->sketch.create(state));
}
