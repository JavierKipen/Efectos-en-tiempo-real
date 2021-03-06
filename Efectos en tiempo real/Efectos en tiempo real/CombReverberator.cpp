#include "CombReverberator.h"




CombReverberator::CombReverberator()
{
}

CombReverberator::CombReverberator(float g, unsigned int Taps)
{
	this->g = g;
	this->Taps = Taps;
	memoryL.resize(Taps, 0);
	memoryR.resize(Taps, 0);
	counter = 0;
}

void CombReverberator::setParams(float g, unsigned int Taps)
{
	this->g = g;
	this->Taps = Taps;
	memoryL.resize(Taps, 0);
	memoryR.resize(Taps, 0);
	counter = 0;
}

void CombReverberator::Action(const float * in, float * out, unsigned int len)
{
	for (unsigned int i = 0; i < len; i++)
	{
		*out = memoryL[counter];
		memoryL[counter] = (*out) * g + (*in);
		out++; in++;
		*out = memoryR[counter];
		memoryR[counter] = (*out) * g + (*in);
		out++; in++;
		counter = (++counter) % Taps;
	}
}

CombReverberator::~CombReverberator()
{
}
