#pragma once
#include "Effect.h"
#include "portaudio.h"
#include <math.h>

#define FLANGER_BL_DEFAULT "0.7"
#define FLANGER_FF_DEFAULT "0.7"
#define FLANGER_FB_DEFAULT "0.7"
#define FLANGER_DELAY_DEFAULT "1"	//ms [0]
#define FLANGER_WIDTH_DEFAULT "1"	//ms [0-2ms]
#define FLANGER_FREQ_DEFAULT "1"	//Hz [0.1-1Hz]

#define FLANGER_PARAMETERS {"DELAY", "WIDTH", "FEEDBACK", "FREQUENCY"}


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
	unsigned int bufferSize, counter;
	float writeTapR, writeTapL;
	float paramDelay, paramWidth, FB, BL, FF, paramFreq;

};

