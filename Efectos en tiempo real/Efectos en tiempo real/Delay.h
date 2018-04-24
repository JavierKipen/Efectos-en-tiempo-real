#pragma once
#include "Effect.h"

#define DELAY_DEFAULT_BASIC_DELAY_T "0.5" //Segundos
#define DELAY_DEFAULT_BASIC_ATT "0.5" //Segundos
#define DELAY_DEFAULT_TYPE "IIR Comb Filter"

#define DELAY_PARAMETERS {"Type", "Time Delay", "Attenuation"}

class Delay :
	public Effect
{
public:
	Delay(unsigned int sampleFreq);
	bool setParam(string paramName, string paramValue);
	bool Action(const float * in, float * out, unsigned int len);
	~Delay();
private:
	vector<float> prevInputL;
	vector<float> prevInputR;
	unsigned int prevSamplesNeeded, counter;
	float basicAttenuation;
};

