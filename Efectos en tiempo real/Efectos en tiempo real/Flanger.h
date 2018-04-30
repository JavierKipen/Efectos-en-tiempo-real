#pragma once
#include "Effect.h"
#include "portaudio.h"
#include <math.h>

#define FLANGER_ATTENUATION_DEFAULT "0.8" 
#define FLANGER_PEAK_DELAY_DEFAULT "10"	// not user controllable
#define FLANGER_FREQ_DEFAULT "1" //Hz
#define FLANGER_AVG_DELAY "2"	// not user controllable
#define FLANGER_INVERT_DEFAULT "0"

#define FLANGER_PARAMETERS {"ATTENUATION", "PEAK DELAY", "FREQUENCY", "INVERT"}

#define PI 3.14159265

class Flanger : public Effect
{
public:
	Flanger(unsigned int sampleFreq);
	bool setParam(string paramName, string paramValue);
	bool Action(const float * in, float * out, unsigned int len);
	~Flanger();
private:
	vector<float> prevInput;
	float attenuation, delayPeak, delayFreq;
	unsigned int delaySize, invertSum;
	float counter, delta;

};

