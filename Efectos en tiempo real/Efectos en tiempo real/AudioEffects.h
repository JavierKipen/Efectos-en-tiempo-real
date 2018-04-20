#pragma once
#include "Effect.h"

class AudioEffects
{
public:
	AudioEffects();
	bool startPlaying();
	vector<string> getCurrParamNames();
	vector<double> getCurrParamValues();
	bool setParam(string paramName, double paramValue);
	bool Action(float * in, float * out, unsigned int len);
	~AudioEffects();
private:
	Effect * currentEffect;
};

