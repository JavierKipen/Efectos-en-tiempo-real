#include "Vibrato.h"

#define VIBRATO_LFO_FREQ_INDEX 0
#define VIBRATO_MAX_DELAY_INDEX 1
#define VIBRATO_LFO_TYPE_INDEX 2

#define GEN_MOD(x,y) ((x)-(floor(((x)/(y)))*y))


Vibrato::Vibrato(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	unsigned int maxMemoryNeeded = (unsigned int)((MAXIMUM_MAX_DELAY/1000.0f) *(float)sampleFreq + 1);
	memoryL.resize(maxMemoryNeeded, 0);
	memoryR.resize(maxMemoryNeeded, 0);
	paramNames = VIBRATO_PARAM_NAMES;
	paramValues = VIBRATO_DEFAULT_PARAM_VALUES;
	saveValues();
}

bool Vibrato::Action(const float * in, float * out, unsigned int len)
{
	float sampleIndex, frac;
	unsigned int indexOfolderSample, indexOfnewerSample;
	for (unsigned int i = 0; i < len; i++) //Dos veces len por ser estereo.
	{
		if(paramValues[VIBRATO_LFO_TYPE_INDEX]=="Sine")
			sampleIndex = (sin(2 * 3.14159265 * lfoFreq * (float)sampleCount / sampleFreq)  * (float)(maxSampleDev-1) + (float)(maxSampleDev-1)) / 2.0; //Calculo el retardo que tendrá para cada muestra. El mismo irá entre 0 y max sampleDev-1 asi no se pisa con la muestra más vieja.
		else if (paramValues[VIBRATO_LFO_TYPE_INDEX] == "Triang")
			sampleIndex = triangularWaveGen();
		sampleIndex = (float)counter + sampleIndex;
		if (sampleIndex > maxSampleDev)
			sampleIndex = GEN_MOD((float)sampleIndex, maxSampleDev);
		indexOfolderSample = floor(sampleIndex);
		indexOfnewerSample = floor(sampleIndex) + 1;
		if (indexOfnewerSample == maxSampleDev)
			indexOfnewerSample = 0;
		frac = sampleIndex - floor(sampleIndex);//Fracción del error
		*out++ = memoryL[indexOfnewerSample] * frac + memoryL[indexOfolderSample] * (1 - frac);
		memoryL[counter] = *in++;
		*out++ = memoryR[indexOfnewerSample] * frac + memoryR[indexOfolderSample] * (1 - frac);
		memoryR[counter] = *in++;
		counter = (++counter) % maxSampleDev;
		sampleCount++;
	}
	return true;
}

bool Vibrato::setParam(string paramName, string paramValue)
{
	bool retVal = false;
	if (paramName == "LFO Type")
	{
		if (paramValue == "Sine" || paramValue == "Triang")
		{
			paramValues[VIBRATO_LFO_TYPE_INDEX] = paramValue; retVal = true;
		}
		else
			ErrorMsg = "WIP";
	}
	else
	{
		float paramValuef = stof(paramValue);
		if (paramName == "Max Delay")
		{
			if (paramValuef >= MINIMUM_MAX_DELAY && paramValuef <= MAXIMUM_MAX_DELAY)
			{
				paramValues[VIBRATO_MAX_DELAY_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = "WIP";
		}
		else if (paramName == "LFO Freq")
		{
			if (paramValuef >= MIN_LFO_FREQ && paramValuef <= MAX_LFO_FREQ)
			{
				paramValues[VIBRATO_LFO_FREQ_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = "WIP";
		}
	}
	if (retVal == true)
		saveValues();
	return retVal;
}


Vibrato::~Vibrato()
{
}

float Vibrato::triangularWaveGen()
{
	float retVal;
	float waveT = (1.0f / lfoFreq) * sampleFreq;
	float normalizedRamp = (GEN_MOD((float)sampleCount, waveT) / waveT); //Esto es una rampa.
	float normalizedTri = abs(normalizedRamp - 0.5f);
	return normalizedTri * 2*(float)(maxSampleDev - 1);
}

void Vibrato::saveValues()
{
	maxSampleDev = unsigned int((stof(paramValues[VIBRATO_MAX_DELAY_INDEX]) / 1000.0f)*(float)sampleFreq + 1);
	lfoFreq = stof(paramValues[VIBRATO_LFO_FREQ_INDEX]);
}
