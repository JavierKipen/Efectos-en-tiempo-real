#include "AudioEffects.h"



AudioEffects::AudioEffects()
{
	currentEffect = new Fuzz;
	allEffects = LIST_OF_EFFECTS;
}

bool AudioEffects::startPlaying()
{
	return false;
}
void AudioEffects::pickNewEffect(string newEffect)
{
	delete currentEffect;
	if (newEffect == "Fuzz")
		currentEffect = new Fuzz;
}

string AudioEffects::popErrorMsg()
{
	return currentEffect->popInputErrorMsg();
}

vector<string> AudioEffects::getCurrParamNames()
{
	return currentEffect->getParamNames();
}

vector<double> AudioEffects::getCurrParamValues()
{
	return currentEffect->getParamValues();
}

bool AudioEffects::setParam(string paramName, double paramValue)
{
	bool retVal=false;
	if (paramValue != INFINITY && paramValue != -INFINITY && paramValue != NAN) //Evita valores que nunca van a tomar parámetros.
		retVal = currentEffect->setParam(paramName, paramValue);
	return retVal;
}

bool AudioEffects::Action(float * in, float * out, unsigned int len)
{
	return false;
}

vector<string> AudioEffects::getListOfEffects()
{
	return allEffects;
}





AudioEffects::~AudioEffects()
{
}

