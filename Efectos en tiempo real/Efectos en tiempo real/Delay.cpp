#include "Delay.h"



Delay::Delay(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = DELAY_PARAMETERS;
	paramValues.resize(3, "0");
	paramValues[0]=DELAY_DEFAULT_TYPE; //Tipo del delay
	paramValues[1]=DELAY_DEFAULT_BASIC_DELAY_T; //
	paramValues[2] = DELAY_DEFAULT_BASIC_ATT; //
	prevSamplesNeeded = stof(paramValues[1]);
	prevSamplesNeeded = (unsigned int) ((stof(paramValues[1]) * (float)(sampleFreq))+1);
	basicAttenuation = stof(paramValues[2]) ;
	prevInputL.resize(prevSamplesNeeded,0);
	prevInputR.resize(prevSamplesNeeded,0);
	counter = 0;
}

bool Delay::setParam(string paramName, string paramValue)
{
	return false;
}

bool Delay::Action(const float * in, float * out, unsigned int len)
{
	float aux;
	if(paramValues[0]== "Basic" )
	for (unsigned int i = 0; i < len; i++) //Dos veces len por ser estereo.
	{
		*out++ = *in + basicAttenuation * prevInputL[counter];
		prevInputL[counter] = *in++;
		*out++ = *in + basicAttenuation * prevInputR[counter];
		prevInputR[counter] = *in++;
		counter = (++counter) % prevSamplesNeeded;
	}
	else if(paramValues[0] == "IIR Comb Filter")
	for (unsigned int i = 0; i < len; i++) //Dos veces len por ser estereo.
	{
		*out = *in + basicAttenuation * prevInputL[counter];
		prevInputL[counter] = *out;
		out++; in++;
		*out = *in + basicAttenuation * prevInputR[counter];
		prevInputR[counter] = *out;
		out++; in++;
		counter = (++counter) % prevSamplesNeeded;
	}
		
	return true;
}


Delay::~Delay()
{
}
