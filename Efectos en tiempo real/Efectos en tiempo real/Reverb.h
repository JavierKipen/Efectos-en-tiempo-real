#pragma once
#include "Effect.h"
#include "BasicReverberator.h"

#define REVERB_SCHROEDER_PARAM_NAMES {"Type", "G Factor", "Delay Time"}
#define REVERB_SCHROEDER_MULTI_PARAM_NAMES {"Type","Nmbr of boxes", "G Factors", "Delay Times"}

#define REVERB_SCHROEDER_DEFAULT_G "0.3"
#define REVERB_SCHROEDER_DEFAULT_T_DELAY "0.3"

#define REVERB_SCHROEDER_MULTI_NMBR "5"
#define REVERB_SCHROEDER_MULTI_DEFAULT_G "0.7 -0.7 0.7 0.7 0.7"
#define REVERB_SCHROEDER_MULTI_DEFAULT_T_DELAY "0.1 0.068 0.06 0.0197 0.00585"
/*
#define REVERB_SCHROEDER_MULTI_NMBR "2"
#define REVERB_SCHROEDER_MULTI_DEFAULT_G "0.7 0.7"
#define REVERB_SCHROEDER_MULTI_DEFAULT_T_DELAY "0.1 0.1"*/

#define REVERB_DEFAULT_PARAM_NAMES REVERB_SCHROEDER_PARAM_NAMES
#define REVERB_DEFAULT_PARAM_VALUES {"Schroeder", REVERB_SCHROEDER_DEFAULT_G, REVERB_SCHROEDER_DEFAULT_T_DELAY}
#define REVERB_SCHROEDER_MULTI_PARAM_VALUES {"Multi_Schroeder", REVERB_SCHROEDER_MULTI_NMBR, REVERB_SCHROEDER_MULTI_DEFAULT_G, REVERB_SCHROEDER_MULTI_DEFAULT_T_DELAY}

#define REVERB_SCHROEDER_MAX_T_DELAY 0.5
#define REVERB_SCHROEDER_MAX_G 1
#define REVERB_SCHROEDER_MAX_BOXES 5

#define REVERB_TYPE_ERROR_MSG "The only type avaiable is Schroeder."
#define REVERB_G_ERROR_MSG "The parameter g must be from 0 to 1."
#define REVERB_T_DELAY_ERROR_MSG "The maximum delay is 0.5 secs."

class Reverb :public Effect
{
public:
	Reverb(unsigned int sampleFreq);
	bool Action(const float * in, float * out, unsigned int len);
	bool setParam(string paramName, string paramValue);
	~Reverb();
private:
	void computeMultipleSchroeder(const float * in, float * out, unsigned int len);
	void saveValues();
	BasicReverberator *Schroeder;
	unsigned int nmbrOfTaps, nmbrOfBoxes;
	vector<float> memoryL[REVERB_SCHROEDER_MAX_BOXES];
	vector<float> memoryR[REVERB_SCHROEDER_MAX_BOXES];
	unsigned int nmbrsOfTaps[REVERB_SCHROEDER_MAX_BOXES], counters[REVERB_SCHROEDER_MAX_BOXES];
	float schroederG, Gs[REVERB_SCHROEDER_MAX_BOXES];
};

