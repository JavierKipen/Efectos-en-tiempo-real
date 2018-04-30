#pragma once
#include "Effect.h"

#define REVERB_SCHROEDER_PARAM_NAMES {"Type", "G Factor", "Delay Time"}

#define REVERB_SCHROEDER_DEFAULT_G "3"
#define REVERB_SCHROEDER_DEFAULT_T_DELAY "0.5"

#define REVERB_DEFAULT_PARAM_NAMES REVERB_SCHROEDER_PARAM_NAMES
#define REVERB_DEFAULT_PARAM_VALUES {"Schroeder", REVERB_SCHROEDER_DEFAULT_G, REVERB_SCHROEDER_DEFAULT_T_DELAY}

#define REVERB_SCHROEDER_MAX_T_DELAY 1


class Reverb :public Effect
{
public:
	Reverb();
	~Reverb();
private:
	vector<float> prevInputL;
	vector<float> prevInputR;
	unsigned int nmbrOfTaps, maxSamplesNeeded;
};

