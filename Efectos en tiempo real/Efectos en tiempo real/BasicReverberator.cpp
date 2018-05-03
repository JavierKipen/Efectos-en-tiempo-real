#include "BasicReverberator.h"



BasicReverberator::BasicReverberator(float g, unsigned int Taps)
{
	this->g = g;
	this->Taps = Taps;
	memoryL.resize(Taps, 0);
	memoryR.resize(Taps, 0);
	counter = 0;
}

void BasicReverberator::Action(const float * in, float * out, unsigned int len)
{
	for (unsigned int i = 0; i < len; i++)
	{
		*out = memoryL[counter] - g * (*in);
		memoryL[counter] = (*out) * g + (*in);
		out++; in++;
		*out = memoryR[counter] - g * (*in);
		memoryR[counter] = (*out) * g + (*in);
		out++; in++;
		counter = (++counter) % Taps;
	}
}


BasicReverberator::~BasicReverberator()
{
}
