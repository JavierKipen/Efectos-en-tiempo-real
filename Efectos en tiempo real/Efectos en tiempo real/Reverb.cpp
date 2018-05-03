#include "Reverb.h"

#define REVERB_TYPE_INDEX 0
#define REVERB_SCHR_G_INDEX 1
#define REVERB_SCHR_T_DELAY_INDEX 2

Reverb::Reverb(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = REVERB_DEFAULT_PARAM_NAMES;
	paramValues = REVERB_DEFAULT_PARAM_VALUES;
	saveValues();
	Schroeder = new BasicReverberator(schroederG, nmbrOfTaps);
}

bool Reverb::Action(const float * in, float * out, unsigned int len)
{
	float aux;
	unsigned int delayedInputIndex = 0;
	if (paramValues[REVERB_TYPE_INDEX] == "Schroeder")
		Schroeder->Action(in, out, len);

	return true;
}

bool Reverb::setParam(string paramName, string paramValue)
{
	bool retVal = false;
	if (paramName == "Type")
	{
		if (paramValue == "Schroeder")
		{	paramValues[REVERB_TYPE_INDEX] = paramValue; retVal = true;}
	}
	else if (paramName == "G Factor")
	{
		if (stof(paramValue) < REVERB_SCHROEDER_MAX_G && stof(paramValue) > 0)
		{	
			delete Schroeder;
			paramValues[REVERB_SCHR_G_INDEX] = paramValue; 
			saveValues(); 
			Schroeder = new BasicReverberator(schroederG, nmbrOfTaps);
			retVal = true;
		}
		else
			ErrorMsg = REVERB_G_ERROR_MSG;
	}
	else if (paramName == "Delay Time")
	{
		if (stof(paramValue) <= REVERB_SCHROEDER_MAX_T_DELAY && stof(paramValue) > 0)
		{
			delete Schroeder;
			paramValues[REVERB_SCHR_T_DELAY_INDEX] = paramValue; 
			saveValues(); 
			Schroeder = new BasicReverberator(schroederG, nmbrOfTaps);
			retVal = true;
		}
		else
			ErrorMsg = REVERB_T_DELAY_ERROR_MSG;
	}
	
	return retVal;
}


void Reverb::saveValues()
{
	if(paramValues[REVERB_TYPE_INDEX]== "Schroeder")
	{
		nmbrOfTaps = unsigned int(stof(paramValues[REVERB_SCHR_T_DELAY_INDEX]) * (float)(sampleFreq)+1);
		schroederG = stof(paramValues[REVERB_SCHR_G_INDEX]);
	}
	
}

Reverb::~Reverb()
{
}


