#include "Chorus.h"

#define CHORUS_DELAY_T_INDEX 0
#define CHORUS_DEPTH_INDEX 1
#define CHORUS_LFO_FREQ_INDEX 2
#define CHORUS_LFO_TYPE_INDEX 3



Chorus::Chorus(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = CHORUS_PARAMETERS;
	paramValues = CHORUS_DEFAULT_PARAM_VALUES;
	unsigned int maxSamplesNeeded = (unsigned int)(CHORUS_MAX_DELAY_T *2 * (float)(sampleFreq)+1); //*2 por que con un depth de 1 tiene un desvío de max delay, para max delay.
	memoryL.resize(maxSamplesNeeded, 0);
	memoryR.resize(maxSamplesNeeded, 0);
	counter = 0;
	sampleCount = 0;
	saveValues();
	bl = 0.5;
	fb = 1;
	ff = 1;
	
}

bool Chorus::Action(const float * in, float * out, unsigned int len)
{
	unsigned int randomIndex;
	float aux;
	for (unsigned int i = 0; i < len; i++) //Dos veces len por ser estereo.
	{
		calculateEcho();
		*out++ = bl*((*in) + fb * echoL);
		memoryL[counter] = (*in++);
		*out++ = bl*((*in) + fb * echoR);
		memoryR[counter] = (*in++);
		counter = (++counter) % maxTaps;
		sampleCount++;
	}

	return true;
}

bool Chorus::setParam(string paramName, string paramValue)
{
	bool retVal=false;
	if (paramName == "LFO Type")
	{
		if (paramValue == "Sine" || paramValue == "Triang")
		{
			paramValues[CHORUS_LFO_TYPE_INDEX] = paramValue;
			retVal = true;
		}
		else
			ErrorMsg = CHORUS_LFO_TYPE_MSG ;
	}
	else {
		float paramValuef = stof(paramValue);
		if (paramName == "Delay Time")
		{
			if (paramValuef >= CHORUS_MIN_DELAY_T && paramValuef <= CHORUS_MAX_DELAY_T)
			{
				paramValues[CHORUS_DELAY_T_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = CHORUS_DELAY_T_ERROR_MSG;
		}
		else if (paramName == "Depth Factor")
		{
			if (paramValuef > 0 && paramValuef < 1)
			{
				paramValues[CHORUS_DEPTH_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = CHORUS_DEPTH_F_MSG;
		}
		else if (paramName == "LFO Freq")
		{
			if (paramValuef >= CHORUS_MIN_LFO_FREQ && paramValuef <= CHORUS_MAX_LFO_FREQ)
			{
				paramValues[CHORUS_LFO_FREQ_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = CHORUS_LFO_FREQ_MSG "The LFO frequency must go between 0.1Hz and 5Hz.";
		}
	}
	
	if (retVal == true)
		saveValues();
	return retVal;
}

void Chorus::calculateEcho()
{
	float sampleIndex;
	if (paramValues[CHORUS_LFO_TYPE_INDEX] == "Sine")
		sampleIndex = sin(2 * 3.14159265 * lfoFreq * (float)sampleCount / sampleFreq)  * (float)maxDevOfTaps + maxDevOfTaps; //Calculo la desviación que tendrá para el sample dado
	else if (paramValues[CHORUS_LFO_TYPE_INDEX] == "Triang")
		sampleIndex = TRI_WAVE((float)sampleCount, (float)sampleFreq/lfoFreq)* 2 * (float)maxDevOfTaps;
	sampleIndex = counter + sampleIndex;
	if(sampleIndex > maxTaps)
		sampleIndex = sampleIndex - maxTaps;
	unsigned int indexOfolderSample = floor(sampleIndex);
	unsigned int  indexOfnewerSample = floor(sampleIndex)+1;
	if (indexOfnewerSample == maxTaps)
		indexOfnewerSample = 0;
	float frac = sampleIndex - floor(sampleIndex);//Fracción del error
	echoL = memoryL[indexOfnewerSample] * frac + memoryL[indexOfolderSample] * (1 - frac);
	echoR = memoryR[indexOfnewerSample] * frac + memoryR[indexOfolderSample] * (1 - frac);
}

void Chorus::saveValues()
{
	float aux = stof(paramValues[CHORUS_DELAY_T_INDEX])/1000.0; //Porque estaba en milisegundos.
	nominalTaps = (unsigned int)(aux * (float)(sampleFreq));
	maxDevOfTaps = (unsigned int)(aux * stof(paramValues[CHORUS_DEPTH_INDEX]) * (float)(sampleFreq));
	if (maxDevOfTaps == 0)
		maxDevOfTaps = 1;
	maxTaps = nominalTaps + maxDevOfTaps;
	lfoFreq = stof(CHORUS_DEFAULT_LFO_FREQ);
	
}

Chorus::~Chorus()
{
}