#include "ofMain.h"
#include "ofApp.h"

int main( ){
    // can be OF_WINDOW or OF_FULLSCREEN
	ofSetupOpenGL(1024, 768, OF_WINDOW);

    ofRunApp(new ofApp());
}
