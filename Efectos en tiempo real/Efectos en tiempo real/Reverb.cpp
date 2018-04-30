#include "Reverb.h"

#define REVERB_TYPE_INDEX 0
#define REVERB_SCHR_G_INDEX 1
#define REVERB_SCHR_T_DELAY_INDEX 2

Reverb::Reverb(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = REVERB_DEFAULT_PARAM_NAMES;
	paramValues = REVERB_DEFAULT_PARAM_VALUES;
	maxSamplesNeeded = (unsigned int)(REVERB_SCHROEDER_MAX_T_DELAY  * (float)(sampleFreq)+1);
	prevInputL.resize(maxSamplesNeeded, 0);
	prevInputR.resize(maxSamplesNeeded, 0);
	saveValues();
	counter = 0;
}

bool Reverb::Action(const float * in, float * out, unsigned int len)
{
	float aux;
	unsigned int delayedInputIndex = 0;
	if (paramValues[REVERB_TYPE_INDEX] == "Schroeder")
		for (unsigned int i = 0; i < len; i++) //Dos veces len por ser estereo.
		{
			*out = (-1) * schroederG * (*in) + prevInputL[counter];
			prevInputL[counter] = (*in++) + schroederG * (*out++);
			*out = (-1) * schroederG * (*in) + prevInputR[counter];
			prevInputR[counter] = (*in++) + schroederG * (*out++);
			counter = (++counter) % nmbrOfTaps;
		}
	return true;
}

bool Reverb::setParam(string paramName, string paramValue)
{
	bool retVal = false;
	if (paramName == "Type")
	{
		if (paramValue == "Schroeder")
		{	paramValues[REVERB_TYPE_INDEX] = paramValue; retVal = true;}
		else
			ErrorMsg = REVERB_TYPE_ERROR_MSG;
	}
	else if (paramName == "G Factor")
	{
		if (stof(paramValue) < REVERB_SCHROEDER_MAX_G && stof(paramValue) > 0)
		{	paramValues[REVERB_SCHR_G_INDEX] = paramValue; saveValues(); retVal = true;}
		else
			ErrorMsg = REVERB_G_ERROR_MSG;
	}
	else if (paramName == "Delay Time")
	{
		if (stof(paramValue) <= REVERB_SCHROEDER_MAX_T_DELAY && stof(paramValue) > 0)
		{	paramValues[REVERB_SCHR_T_DELAY_INDEX] = paramValue; saveValues(); retVal = true;}
		else
			ErrorMsg = REVERB_T_DELAY_ERROR_MSG;
	}
	
	return retVal;
}

void Reverb::saveValues()
{
	nmbrOfTaps = unsigned int(stof(paramValues[REVERB_SCHR_T_DELAY_INDEX]) * (float)(sampleFreq)+1);
	schroederG = stof(paramValues[REVERB_SCHR_G_INDEX]);
}

Reverb::~Reverb()
{
}


