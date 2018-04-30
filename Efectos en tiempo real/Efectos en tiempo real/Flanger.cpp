#include "Flanger.h"



Flanger::Flanger(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = FLANGER_PARAMETERS;
	paramValues.resize(paramNames.size(), "0");
	paramValues[0] = FLANGER_ATTENUATION_DEFAULT; //Tipo del delay
	paramValues[1] = FLANGER_PEAK_DELAY_DEFAULT; //
	paramValues[2] = FLANGER_FREQ_DEFAULT; //
	attenuation = stof(paramValues[0]);
	delayPeak = stof(paramValues[1]);
	delayFreq = stof(paramValues[2]);
	delta = 1.0 / (10.0 * stof(paramValues[2]));
	counter = 0;
	delaySize = 0;
	prevInput.resize(0, 0);
}


bool Flanger::setParam(string paramName, string paramValue)
{
	bool retVal = false;
	
	return retVal;
}

bool Flanger::Action(const float * in, float * out, unsigned int len)
{
	if (delaySize != 0)
	{
		for (unsigned int i = 0; i < 2 * len; i++) //Dos veces len por ser estereo.
		{
			*out++ = *in + attenuation * prevInput[i%delaySize];
			prevInput[i%delaySize] = *in++;
		}
	}
	else
	{
		for (unsigned int i = 0; i < 2 * len; i++) //Dos veces len por ser estereo.
		{
			*out++ = *in++;
		}
	}
	counter += delta;
	if (counter > 1 / delayFreq)	counter = 0;

	delaySize = abs(floor(delayPeak*sin(2 * PI*delayFreq*counter)));
	prevInput.resize(2*delaySize, 0);
	return true;
}


Flanger::~Flanger()
{
}
