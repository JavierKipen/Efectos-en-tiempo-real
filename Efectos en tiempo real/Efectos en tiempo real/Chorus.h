#pragma once
#include "Effect.h"
#include <vector>

#define CHORUS_DEFAULT_DELAY_T "15" //En milisegundos
#define CHORUS_DEFAULT_DEPTH_F "0.25"
#define CHORUS_DEFAULT_LFO_FREQ "0.5"
#define CHORUS_DEFAULT_LFO_TYPE "Sine"

#define CHORUS_MAX_DELAY_T 30.0
#define CHORUS_MIN_DELAY_T 1.0
#define CHORUS_MIN_LFO_FREQ 0.1
#define CHORUS_MAX_LFO_FREQ 3

#define CHORUS_PARAMETERS {"Delay Time", "Depth Factor", "LFO Freq", "LFO Type"}
#define CHORUS_DEFAULT_PARAM_VALUES {CHORUS_DEFAULT_DELAY_T , CHORUS_DEFAULT_DEPTH_F, CHORUS_DEFAULT_LFO_FREQ, CHORUS_DEFAULT_LFO_TYPE }

#define CHORUS_DELAY_T_ERROR_MSG "The maximum delay time is 30ms, and the minimum is 1ms."
#define CHORUS_DEPTH_F_MSG "Depth must be positive and less than 1."
#define CHORUS_LFO_FREQ_MSG "The LFO frequency must go between 0.1Hz and 5Hz."
#define CHORUS_LFO_TYPE_MSG "The avaiable LFO Types are Sine and Triang"

class Chorus :
	public Effect
{
public:
	Chorus(unsigned int sampleFreq);
	bool Action(const float * in, float * out, unsigned int len);
	bool setParam(string paramName, string paramValue);
	~Chorus();
private:
	void calculateEcho();
	void saveValues();
	int counter;
	long unsigned int sampleCount;
	unsigned int nominalTaps,maxDevOfTaps, maxTaps;
	float fb, ff, bl,LP_PZRatio, totGain, echoL, echoR, lfoFreq;
	vector<float> memoryR, memoryL;// LP_prevX, LP_prevY;
};

