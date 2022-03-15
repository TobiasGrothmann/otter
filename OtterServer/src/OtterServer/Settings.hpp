#pragma once
#include "stl.hpp"

namespace otter {

const std::string dataDir = std::string(std::getenv("HOME")) + std::string("/otterServerData");
const std::string dataLeftAfterPausePath = dataDir + std::string("/leftAfterPause.q");

//										y1   y2 x   z
const std::vector<char> dirPins   	=	{18, 4, 11, 16};
const std::vector<char> stepPins 	=	{15, 3, 7,  20};
const std::vector<char> enablePins	=	{14, 2, 8,  21};

const std::vector<char> ms1Pins	=	{22, 10, 12, 13};
const std::vector<char> ms2Pins	=	{27, 9,  6,  19};
const std::vector<char> ms3Pins	=	{17, 25, 5,  26};

const std::vector<std::vector<char>> dirVal	=
{
	{1, 0}, // y1
	{1, 0}, // y2
	{1, 0}, // x
	{0, 1}  // z
};

/*
	MS1		MS2		MS3		Microstep Resolution
	--------------------------------------------
	Low		Low		Low		Full step
	High	Low		Low		Half step
	Low		High	Low		Quarter step
	High	High	Low		Eighth step
	High	High	High	Sixteenth step
*/
const std::vector<std::vector<char>> msValues =
{
	{1, 1, 1}, // y1
	{1, 1, 1}, // y2
	{1, 1, 1}, // x
	{1, 1, 0}  // z
};
const int ms = 16;
const int headMs = 8;


const float distPerStepAxis = 0.0139935599999f / ms; // cm
const float distPerStepHead = 0.8f / (200.0f * headMs); // cm (200 steps per revolution, 8mm travel per revolution)

const float headTravelToTouch = 0.6f; // cm
const float maxExtraHeadTravel = 0.25f; // cm

}
