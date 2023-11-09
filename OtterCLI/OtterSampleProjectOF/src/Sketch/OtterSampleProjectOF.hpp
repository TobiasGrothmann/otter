#pragma once

#include <stdio.h>
#include "ofxGui.h"
#include "OtterSketchOF.hpp"
#include "Otter.hpp"


class OtterSampleProjectOF : public OtterSketchOF
{
public:
    
    virtual void setup() override;
    virtual otter::Queue create(const State& state) override;
    
    FLOAT exampleSliderValue;
};
