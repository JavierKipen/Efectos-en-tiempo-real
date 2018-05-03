#include "PlaneReverb.h"



PlaneReverb::PlaneReverb(float g, unsigned int Taps)
{
	this->g = g;
	this->Taps = Taps;
	memoryL.resize(Taps, 0);
	memoryR.resize(Taps, 0);
	counter = 0;
}

void PlaneReverb::Action(const float * in, float * out, unsigned int len)
{
	for (unsigned int i = 0; i < len; i++)
	{
		*out = (*in) + memoryL[counter] * g;
		memoryL[counter] = *out;
		out++; in++;
		*out = (*in) + memoryR[counter] * g;
		memoryR[counter] = *out;
		out++; in++;
		counter = (++counter) % Taps;
	}
}


PlaneReverb::~PlaneReverb()
{
}
