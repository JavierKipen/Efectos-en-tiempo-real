#pragma once
#include "Effect.h"
#include <vector>

#define CHORUS_DEFAULT_DELAY_T "0.005"
#define CHORUS_DEFAULT_DEPTH_F "0.1"
#define CHORUS_DEFAULT_LP_RATIO "0.85"
#define CHORUS_DEFAULT_LP_ORDER "2"

#define CHORUS_MAX_DELAY_T 0.03
#define CHORUS_MAX_LP_ORDER 5

#define CHORUS_PARAMETERS {"Delay Time", "Depth Factor", "LP P/Z Ratio", "LP Order"}
#define CHORUS_DEFAULT_PARAM_VALUES {CHORUS_DEFAULT_DELAY_T , CHORUS_DEFAULT_DEPTH_F, CHORUS_DEFAULT_LP_RATIO, CHORUS_DEFAULT_LP_ORDER }

class Chorus :
	public Effect
{
public:
	Chorus(unsigned int sampleFreq);
	bool Action(const float * in, float * out, unsigned int len);
	bool setParam(string paramName, string paramValue);
	~Chorus();
private:
	unsigned int getRandomIndex();
	float getRandomPrev(vector<float> &mem);
	float filterNoise(float random);
	void saveValues();
	int counter;
	unsigned int nominalTaps,maxDevOfTaps, maxTaps;
	float fb, ff, bl,LP_PZRatio, totGain;
	vector<float> memoryR, memoryL, LP_prevX, LP_prevY;
};

