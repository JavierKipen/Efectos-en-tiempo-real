#pragma once
#include "Effect.h"

#define DISTORTION_SOFT_CLIPPING_PARAM_NAMES {"Type", "Gain"}
#define DISTORTION_SOFT_CLIPPING_DEFAULT_PARAM_VALUES {"Soft_Clipping", "2.0"}
#define DISTORTION_HARD_CLIPPING_PARAM_NAMES {"Type", "Threshold"}
#define DISTORTION_HARD_CLIPPING_DEFAULT_PARAM_VALUES {"Hard_Clipping", "0.5"}
#define DISTORTION_OTHER_PARAM_NAMES {"Type"}

#define DISTORTION_TYPE_ERROR_MSG "The avaiable types are Soft_Clipping, Hard_Clipping, Fuzz and Full_Wave_Rectifier"
#define DISTORTION_THRESHOLD_ERROR_MSG "The threshold must go between 0 and 1"
#define DISTORTION_GAIN_ERROR_MSG "The gain must be positive"

class Distortion :
	public Effect
{
public:
	Distortion(unsigned int sampleFreq);
	bool setParam(string paramName, string paramValue);
	bool Action(const float * in, float * out, unsigned int len);
	~Distortion();
private:
	float applyDistortion(float input);
	float cubicAmplifier(float input);
	void saveValues();
	float clippingGain,threshold;
};

