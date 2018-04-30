#pragma once
#include "Effect.h"
#include "portaudio.h"
#include <math.h>

#define FLANGER_ATTENUATION_DEFAULT "0.5"
#define FLANGER_PEAK_DELAY_DEFAULT "10"
#define FLANGER_FREQ_DEFAULT "1" //Hz

#define FLANGER_PARAMETERS {"ATTENUATION", "PEAK DELAY", "FREQUENCY"}

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
	unsigned int delaySize;
	float counter, delta;

};

