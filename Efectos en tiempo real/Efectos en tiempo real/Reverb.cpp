#include "Reverb.h"

#define REVERB_TYPE_INDEX 0
#define REVERB_SCHR_G_INDEX 1
#define REVERB_SCHR_T_DELAY_INDEX 2
#define REVERB_SCHR_BOXES_INDEX 1
#define REVERB_MULTI_SCHR_G_INDEX 2
#define REVERB_MULTI_SCHR_T_DELAY_INDEX 3

Reverb::Reverb(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = REVERB_DEFAULT_PARAM_NAMES;
	paramValues = REVERB_DEFAULT_PARAM_VALUES;
	maxSamplesNeeded = (unsigned int)(REVERB_SCHROEDER_MAX_T_DELAY  * (float)(sampleFreq)+1);
	prevInputL.resize(maxSamplesNeeded, 0);
	prevInputR.resize(maxSamplesNeeded, 0);
	for (unsigned int i = 0; i < REVERB_SCHROEDER_MAX_BOXES; i++)
	{
		memoryL[i].resize(maxSamplesNeeded, 0);
		memoryR[i].resize(maxSamplesNeeded, 0);
	}
	saveValues();
	counter = 0;
}

bool Reverb::Action(const float * in, float * out, unsigned int len)
{
	float aux;
	unsigned int delayedInputIndex = 0;
	if (paramValues[REVERB_TYPE_INDEX] == "Schroeder")
	{
		for (unsigned int i = 0; i < len; i++) //Dos veces len por ser estereo.
		{
			*out++ = (-1) * schroederG * (*in) + prevInputL[counter] * (1 - schroederG * schroederG);
			prevInputL[counter] = (*in++) + prevInputL[counter] * schroederG;
			*out++ = (-1) * schroederG * (*in) + prevInputR[counter] * (1 - schroederG * schroederG);
			prevInputR[counter] = (*in++) + prevInputR[counter] * schroederG;
			counter = (++counter) % nmbrOfTaps;
		}
	}
	else if (paramValues[REVERB_TYPE_INDEX] == "Multi_Schroeder")
		computeMultipleSchroeder(in, out, len);

	return true;
}

bool Reverb::setParam(string paramName, string paramValue)
{
	bool retVal = false;
	if (paramName == "Type")
	{
		if (paramValue == "Schroeder")
		{	paramValues[REVERB_TYPE_INDEX] = paramValue; retVal = true;}
		else if (paramValue == "Multi_Schroeder")
		{
			paramNames = REVERB_SCHROEDER_MULTI_PARAM_NAMES;
			paramValues = REVERB_SCHROEDER_MULTI_PARAM_VALUES;
			saveValues();
			retVal = true;
		}
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

void Reverb::computeMultipleSchroeder(const float * in, float * out, unsigned int len)
{
	float auxL[REVERB_SCHROEDER_MAX_BOXES+1];
	float auxR[REVERB_SCHROEDER_MAX_BOXES+1];
	for (unsigned int i = 0; i < len; i++) //Dos veces len por ser estereo.
	{
		auxL[0] = (*in++);
		auxR[0] = (*in++);
		for (unsigned int nmbrOfBox = 0; nmbrOfBox < nmbrOfBoxes; nmbrOfBox++)
		{
			auxL[nmbrOfBox+1] = (-1) * Gs[nmbrOfBox] * auxL[nmbrOfBox] + memoryL[nmbrOfBox][counters[nmbrOfBox]] * (1 - Gs[nmbrOfBox]* Gs[nmbrOfBox]);
			memoryL[nmbrOfBox][counters[nmbrOfBox]] = auxL[nmbrOfBox] + memoryL[nmbrOfBox][counters[nmbrOfBox]] * Gs[nmbrOfBox];
			auxR[nmbrOfBox + 1] = (-1) * Gs[nmbrOfBox] * auxR[nmbrOfBox] + memoryR[nmbrOfBox][counters[nmbrOfBox]] * (1 - Gs[nmbrOfBox] * Gs[nmbrOfBox]);
			memoryR[nmbrOfBox][counters[nmbrOfBox]] = auxR[nmbrOfBox] + memoryR[nmbrOfBox][counters[nmbrOfBox]] * Gs[nmbrOfBox];
		}
		(*out++) = auxL[nmbrOfBoxes];
		(*out++) = auxR[nmbrOfBoxes];
		for(unsigned int j=0; j<nmbrOfBoxes; j++)
			counters[j] = (++counters[j]) % nmbrsOfTaps[j];
	}
}

void Reverb::saveValues()
{
	if(paramValues[REVERB_TYPE_INDEX]== "Schroeder")
	{
		nmbrOfTaps = unsigned int(stof(paramValues[REVERB_SCHR_T_DELAY_INDEX]) * (float)(sampleFreq)+1);
		schroederG = stof(paramValues[REVERB_SCHR_G_INDEX]);
	}
	else if (paramValues[REVERB_TYPE_INDEX] == "Multi_Schroeder")
	{
		nmbrOfBoxes = stof(paramValues[REVERB_SCHR_BOXES_INDEX]);
		string aux1, aux2;
		string::size_type szT,szG;
		nmbrsOfTaps[0] = unsigned int(stof(paramValues[REVERB_MULTI_SCHR_T_DELAY_INDEX], &szT) * (float)(sampleFreq)+1);
		Gs[0] = stof(paramValues[REVERB_MULTI_SCHR_G_INDEX], &szG);
		counters[0] = 0;
		aux1 = paramValues[REVERB_MULTI_SCHR_T_DELAY_INDEX];
		aux2 = paramValues[REVERB_MULTI_SCHR_G_INDEX];
		for (unsigned int i = 1; i < nmbrOfBoxes; i++)
		{
			aux1 = aux1.substr(szT);
			aux2 = aux2.substr(szG);
			nmbrsOfTaps[i] = unsigned int(stof(aux1, &szT) * (float)(sampleFreq)+1);
			Gs[i] = stof(aux2, &szG);
			counters[i] = 0;
		}
	}
	
}

Reverb::~Reverb()
{
}


