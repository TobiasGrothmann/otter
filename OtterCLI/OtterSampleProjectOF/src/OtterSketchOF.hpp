#pragma once

#include <stdio.h>

#include "ofxGui.h"

#include "State.hpp"

#define QUOTED(a) #a

#define FLOAT ofxFloatSlider*
#define INT ofxIntSlider*
#define BOOL ofxToggle*

#define FLOAT_SETUP(param, value, min, max) { param = new ofxFloatSlider(); paramsFloat.push_back(param->setup(QUOTED(param), value, min, max)); }
#define INT_SETUP(param, value, min, max) { param = new ofxIntSlider(); paramsInt.push_back(param->setup(QUOTED(param), value, min, max)); }
#define BOOL_SETUP(param, value) { param = new ofxToggle(); paramsBool.push_back(param->setup(QUOTED(param), value)); }

#define GET(param) *param


class OtterSketchOF
{
public:
    
    virtual void setup();
    virtual otter::Queue create(const State& state) = 0;
    
    bool updateEveryFrame = false;
    bool updateOnMouseMove = false;
    bool updateOnMouseClick = false;
    bool updateOnParamChange = false;
    
    std::vector<FLOAT> paramsFloat = {};
    std::vector<INT> paramsInt = {};
    std::vector<BOOL> paramsBool = {};
};
