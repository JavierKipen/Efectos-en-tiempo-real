#pragma once
#include "Effect.h"

#define REVERB_SCHROEDER_PARAM_NAMES {"Type", "G Factor", "Delay Time"}

#define REVERB_SCHROEDER_DEFAULT_G "0.3"
#define REVERB_SCHROEDER_DEFAULT_T_DELAY "0.3"

#define REVERB_DEFAULT_PARAM_NAMES REVERB_SCHROEDER_PARAM_NAMES
#define REVERB_DEFAULT_PARAM_VALUES {"Schroeder", REVERB_SCHROEDER_DEFAULT_G, REVERB_SCHROEDER_DEFAULT_T_DELAY}

#define REVERB_SCHROEDER_MAX_T_DELAY 2.5
#define REVERB_SCHROEDER_MAX_G 1

#define REVERB_TYPE_ERROR_MSG "The only type avaiable is Schroeder."
#define REVERB_G_ERROR_MSG "The parameter g must be from 0 to 1."
#define REVERB_T_DELAY_ERROR_MSG "The maximum delay is 2.5 secs."

class Reverb :public Effect
{
public:
	Reverb(unsigned int sampleFreq);
	bool Action(const float * in, float * out, unsigned int len);
	bool setParam(string paramName, string paramValue);
	~Reverb();
private:
	void saveValues();
	vector<float> prevInputL;
	vector<float> prevInputR;
	unsigned int nmbrOfTaps, maxSamplesNeeded, counter;
	float schroederG;
};

