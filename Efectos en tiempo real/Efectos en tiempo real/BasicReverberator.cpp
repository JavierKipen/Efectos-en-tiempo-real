#include "BasicReverberator.h"



BasicReverberator::BasicReverberator()
{
}

BasicReverberator::BasicReverberator(float g, unsigned int Taps)
{
	this->g = g;
	this->Taps = Taps;
	memoryL.resize(Taps, 0);
	memoryR.resize(Taps, 0);
	counter = 0;
}

void BasicReverberator::setParameters(float g, unsigned int Taps)
{
	this->g = g;
	this->Taps = Taps;
	memoryL.resize(Taps, 0);
	memoryR.resize(Taps, 0);
	counter = 0;
}

void BasicReverberator::Action(const float * in, float * out, unsigned int len)
{
	float aux;
	for (unsigned int i = 0; i < len; i++)
	{
		aux = (*in);
		*out = memoryL[counter] + g * aux;
		memoryL[counter] = aux  - (*out) * g ;
		out++; in++;
		aux = (*in);
		*out = memoryR[counter] + g * aux;
		memoryR[counter] = aux - (*out) * g ;
		out++; in++;
		counter = (++counter) % Taps;
	}
}


BasicReverberator::~BasicReverberator()
{
}
