#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "OtterSketchOF.hpp"
#include "OtterSampleProjectOF.hpp"
#include "QueueDrawer.hpp"

#include "Otter.hpp"


class ofApp : public ofBaseApp
{
public:
    ofApp();
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    ofxPanel gui;
    
    void paramIntChanged(int& param);
    void paramFloatChanged(float& param);
    void paramBoolChanged(bool& param);
    
    
    OtterSampleProjectOF sketch;
    void createQueue();
    
    State state;
    QueueDrawer queueDrawer;
};
