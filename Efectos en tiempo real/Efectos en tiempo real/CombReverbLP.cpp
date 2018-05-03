#include "CombReverbLP.h"

#define PI 3.141592


CombReverbLP::CombReverbLP(float g, unsigned int Taps, float a)
{
	this->g = g;
	this->Taps = Taps;
	memoryL.resize(Taps, 0);
	memoryR.resize(Taps, 0);
	counter = 0;
	A = a;
}

void CombReverbLP::Action(const float * in, float * out, unsigned int len)
{
	unsigned int secondaryCounter;
	if (counter == 0)
		secondaryCounter = (Taps - 1);
	else
		secondaryCounter = counter - 1;
	for (unsigned int i = 0; i < len; i++)
	{
		*out = (*in) + (g / (1 + A) ) * (memoryL[counter] + A * memoryL[secondaryCounter]);
		memoryL[counter] = *out;
		out++; in++;
		*out = (*in) + (g / (1 + A)) * (memoryR[counter] + A * memoryR[secondaryCounter]);
		memoryR[counter] = *out;
		out++; in++;
		counter = (++counter) % Taps;
		if (counter == 0)
			secondaryCounter = (Taps - 1);
		else
			secondaryCounter = counter - 1;
	}
}


CombReverbLP::~CombReverbLP()
{
}
