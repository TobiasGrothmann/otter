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
    else if (key == OF_KEY_CONTROL)
        state.controlDown = true;
    else if (key == OF_KEY_ALT)
        state.altDown = true;
    else if (key == OF_KEY_COMMAND)
        state.commandDown = true;
    else if (key == OF_KEY_SHIFT)
        state.shiftDown = true;
    
    if (state.commandDown && key == 'p')
    {
        cout << "sending to preview" << endl;
        state.queue.preview();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    if (key == OF_KEY_CONTROL)
        state.controlDown = false;
    else if (key == OF_KEY_ALT)
        state.altDown = false;
    else if (key == OF_KEY_COMMAND)
        state.commandDown = false;
    else if (key == OF_KEY_SHIFT)
        state.shiftDown = false;
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
