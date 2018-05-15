#include "Phaser.h"

#define PHASER_DEPTH_INDEX 0
#define PHASER_SWEEP_INDEX 1
#define PHASER_MIN_FREQ_INDEX 2
#define PHASER_LFO_FREQ_INDEX 3
#define PHASER_LFO_TYPE_INDEX 4
#define PHASER_R_INDEX 5

#define PI 3.141592625



Phaser::Phaser(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = PHASER_PARAM_NAMES;
	paramValues = PHASER_DEFAULT_PARAM_VALUES;
	saveValues();
	notchFilter.setSampleFreq(sampleFreq);
}

bool Phaser::Action(const float * in, float * out, unsigned int len)
{
	float faux, aux[2];
	for (unsigned int i = 0; i <  len; i++)
	{
		faux = getNotchFreq();
		notchFilter.setParameters(faux, R);
		notchFilter.filter((float *)in, aux, 1);
		*out++ = (*in++ *(1-depth) - depth * aux[0])*2;
		*out++ = (*in++ *(1 - depth) - depth * aux[1]) * 2;
		sampleCount++;
	}
	return true;
}

bool Phaser::setParam(string paramName, string paramValue)
{
	bool retVal = false;
	if (paramName == "LFO Type")
	{
		if (paramValue == "Sine" || paramValue == "Triang" || paramValue=="Exp Triang")
		{
			paramValues[PHASER_LFO_TYPE_INDEX] = paramValue;
			retVal = true;
		}
		else
			ErrorMsg = PHASER_LFO_TYPE_ERROR_MSG;
	}
	else {
		float paramValuef = stof(paramValue);
		if (paramName == "Depth")
		{
			if (paramValuef >= 0 && paramValuef <= 1)
			{
				paramValues[PHASER_DEPTH_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = PHASER_DEPTH_ERROR_MSG;
		}
		else if (paramName == "Sweep Width")
		{
			if (paramValuef >= 0 && paramValuef <= 1)
			{
				paramValues[PHASER_SWEEP_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = PHASER_SWEEP_WIDTH_ERROR_MSG;
		}
		else if (paramName == "LFO Freq")
		{
			if (paramValuef >= PHASER_MIN_LFO_FREQ && paramValuef <= PHASER_MAX_LFO_FREQ)
			{
				paramValues[PHASER_LFO_FREQ_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = PHASER_LFO_FREQ_ERROR_MSG;
		}
		else if (paramName == "Pole Ratio")
		{
			if (paramValuef > 0 && paramValuef < 1)
			{
				paramValues[PHASER_R_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = PHASER_R_ERROR_MSG;
		}
		else if (paramName == "Min Frequency")
		{
			if (paramValuef > MIN_AUDIBLE_FREQ && paramValuef < MAX_AUDIBLE_FREQ)
			{
				paramValues[PHASER_MIN_FREQ_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = PHASER_MIN_FREQ_ERROR_MSG;
		}
	}

	if (retVal == true)
		saveValues();
	return retVal;
}
float Phaser::getNotchFreq()
{
	float retVal, sweepRange,W;
	if (paramValues[PHASER_LFO_TYPE_INDEX] == "Exp Triang")
	{
		W = MAX_AUDIBLE_FREQ  / fo;
		retVal = fo * pow(W, sweepWidth *TRI_WAVE((float)sampleCount, (float)sampleFreq / lfoFreq));
	}
	else if (paramValues[PHASER_LFO_TYPE_INDEX] == "Sine")
	{/*En este caso, fo es la frecuencia mínima, y el 1 de sweep viene dado cuando la frecuencia máxima llega a 20KHz*/
		sweepRange = sweepWidth * (MAX_AUDIBLE_FREQ - fo);
		retVal= fo + sweepRange * (1+sin(2 * PI*(float)sampleCount*lfoFreq / (float)sampleFreq))/2.0f;
	}
	else if (paramValues[PHASER_LFO_TYPE_INDEX] == "Triang")
	{
		sweepRange = sweepWidth * (MAX_AUDIBLE_FREQ - fo);
		retVal = fo + sweepRange * TRI_WAVE((float)sampleCount, (float)sampleFreq/lfoFreq);
	}
	return retVal;
}
void Phaser::saveValues()
{

	depth = 0.25f+stof(paramValues[PHASER_DEPTH_INDEX])/4.0f;
	sweepWidth = stof(paramValues[PHASER_SWEEP_INDEX]);
	fo= stof(paramValues[PHASER_MIN_FREQ_INDEX]);
	lfoFreq = stof(paramValues[PHASER_LFO_FREQ_INDEX]);
	R = stof(paramValues[PHASER_R_INDEX]);
	
}

Phaser::~Phaser()
{
}