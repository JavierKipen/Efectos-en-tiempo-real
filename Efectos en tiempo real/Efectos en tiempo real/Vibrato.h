#pragma once
#include "Effect.h"

#define VIBRATO_DEFAULT_LFO_FREQ "5"
#define VIBRATO_DEFAULT_MAX_DELAY "2" //En milisegundos
#define VIBRATO_DEFAULT_LFO_TYPE "Sine"

#define VIBRATO_PARAM_NAMES {"LFO Freq", "Max Delay", "LFO Type"}
#define VIBRATO_DEFAULT_PARAM_VALUES  {VIBRATO_DEFAULT_LFO_FREQ, VIBRATO_DEFAULT_MAX_DELAY,VIBRATO_DEFAULT_LFO_TYPE}

#define MAXIMUM_MAX_DELAY 5
#define MINIMUM_MAX_DELAY 0.2

#define MAX_LFO_FREQ 15
#define MIN_LFO_FREQ 0.1

#define VIBRATO_LFO_FREQ_ERROR_MSG "The LFO Freq must be between 0.1Hz and 15Hz."
#define VIBRATO_MAX_DELAY_ERROR_MSG "The time delay must go between 0.2ms and 5ms"
#define VIBRATO_LFO_TYPE_ERROR_MSG "The avaiable LFO Types are Sine and Triang"

class Vibrato :
	public Effect
{
public:
	Vibrato(unsigned int sampleFreq);
	bool Action(const float * in, float * out, unsigned int len);
	bool setParam(string paramName, string paramValue);
	~Vibrato();
private:
	float triangularWaveGen();
	void saveValues();
	unsigned int maxSampleDev,counter;
	long unsigned int sampleCount;
	float lfoFreq;
	vector<float> memoryR, memoryL;
};

