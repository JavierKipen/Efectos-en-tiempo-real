#pragma once
#include "Effect.h"
#include "portaudio.h"
#include <math.h>

#define FLANGER_DELAY_DEFAULT "15"		// Base length of the delay line in ms.	[1:20]
#define FLANGER_WIDTH_DEFAULT "10"	// Amplitude of the modulating signal in ms.	[1:20]
#define FLANGER_DEPTH_DEFAULT "0.5"		// Mix amount of delayed signal [0:1]
#define FLANGER_FEEDBACK_DEFAULT "0.4"	// Signal level of feedback signal
#define FLANGER_LFO_DEFAULT "0.6"		//Hz [0.1:1]
#define FLANGER_INVERT_DEFAULT "1"		// -1 or 1

#define FLANGER_PARAMETERS {"DELAY", "WIDTH", "DEPTH", "FEEDBACK", "LFO", "INVERT"}

#define PI 3.14159265

class Flanger : public Effect
{
public:
	Flanger(unsigned int sampleFreq);
	bool setParam(string paramName, string paramValue);
	bool Action(const float * in, float * out, unsigned int len);
	~Flanger();
private:
	vector<float> prevInputL, prevInputR;
	int bufferSize;
	float writeTapR, writeTapL;
	float paramDelay, paramWidth, paramDepth, paramFB, paramLFO, paramInvert;
	float lfoPhase;

};

