#pragma once
#include "Effect.h"
#include "BasicReverberator.h"
#include "PlaneReverb.h"
#include "CombReverbLP.h"
#include "CompleteReverb.h"

#define REVERB_FULL_PARAM_NAMES {"Type", "G Factor", "A Factor", "Depth"}
#define REVERB_PLANE_PARAM_NAMES {"Type", "G Factor", "Delay Time"}
#define REVERB_LP_PARAM_NAMES {"Type", "G Factor", "Delay Time", "LP Factor"}

#define REVERB_DEFAULT_G "0.7"
#define REVERB_DEFAULT_T_DELAY "0.05"
#define REVERB_DEFAULT_A "0.9"
#define REVERB_DEFAULT_DEPTH "0.5"

#define REVERB_PLANE_DEFAULT_PARAM_VALUES {"Plane", REVERB_DEFAULT_G, REVERB_DEFAULT_T_DELAY}
#define REVERB_LP_DEFAULT_PARAM_VALUES {"LP", REVERB_DEFAULT_G, REVERB_DEFAULT_T_DELAY,REVERB_DEFAULT_A}
#define REVERB_FULL_DEFAULT_PARAM_VALUES {"Full",REVERB_DEFAULT_A, REVERB_DEFAULT_G, REVERB_DEFAULT_DEPTH }

#define REVERB_DEFAULT_PARAM_NAMES REVERB_PLANE_PARAM_NAMES
#define REVERB_DEFAULT_PARAM_VALUES REVERB_PLANE_DEFAULT_PARAM_VALUES

#define REVERB_SCHROEDER_MAX_T_DELAY 0.5
#define REVERB_SCHROEDER_MAX_G 1

#define REVERB_TYPE_ERROR_MSG "The only type avaiable is Schroeder."
#define REVERB_G_ERROR_MSG "The parameter g must be from 0 to 1."
#define REVERB_T_DELAY_ERROR_MSG "The maximum delay is 0.5 secs."
#define REVERB_FC_ERROR_MSG "The Cut frequency must be less than fs/2=22050"

class Reverb :public Effect
{
public:
	Reverb(unsigned int sampleFreq);
	bool Action(const float * in, float * out, unsigned int len);
	bool setParam(string paramName, string paramValue);
	~Reverb();
private:
	void saveValues();
	void updateReverb();
	CompleteReverb * Full;
	CombReverbLP * LP;
	PlaneReverb * plane;
	unsigned int nmbrOfTaps, nmbrOfBoxes;
	float g,a,depth;
};

