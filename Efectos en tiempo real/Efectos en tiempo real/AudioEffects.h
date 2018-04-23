#pragma once
#include "Effect.h"
#include "Fuzz.h"

#define LIST_OF_EFFECTS {"Fuzz"}

class AudioEffects
{
public:
	AudioEffects();
	bool startPlaying();
	vector<string> getCurrParamNames();
	vector<double> getCurrParamValues();
	bool setParam(string paramName, double paramValue);
	bool Action(float * in, float * out, unsigned int len);
	vector<string> getListOfEffects();
	void pickNewEffect(string newEffect);
	string popErrorMsg();
	~AudioEffects();
private:
	Effect * currentEffect;
	vector<string> allEffects;
};

