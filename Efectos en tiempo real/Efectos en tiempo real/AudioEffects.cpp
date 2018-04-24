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

vector<string> AudioEffects::getCurrParamValues()
{
	return currentEffect->getParamValues();
}

bool AudioEffects::setParam(string paramName, string paramValue)
{
	bool retVal=false;
	if (paramValue.size()!=0) //Evita valores que nunca van a tomar parámetros.
		retVal = currentEffect->setParam(paramName, paramValue);
	return retVal;
}

bool AudioEffects::Action(const float * in, float * out, unsigned int len)
{
	return currentEffect->Action(in,out,len);
}

vector<string> AudioEffects::getListOfEffects()
{
	return allEffects;
}





AudioEffects::~AudioEffects()
{
}

int audioEffectCallback(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo * timeInfo, PaStreamCallbackFlags statusFlags, void * userData)
{
	float * out = (float *)outputBuffer;
	const float * in = (const float *)inputBuffer;
	unsigned int i;
	(void)timeInfo; /* Prevent unused variable warnings. */
	(void)statusFlags;
	(void)userData;
	AudioEffects *audioEffects = (AudioEffects *)userData;
	audioEffects->Action(in, out, framesPerBuffer);

	return paContinue;
}
