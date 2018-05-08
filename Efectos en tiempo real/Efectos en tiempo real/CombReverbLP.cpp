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
	prevVal[0] = 0;
	prevVal[1] = 0;
	prevInp[0] = 0;
	prevInp[1] = 0;
	calcCoefs();
}

void CombReverbLP::Action(const float * in, float * out, unsigned int len)
{
	for (unsigned int i = 0; i < len; i++)
	{
		*out = (*in) + g*(coefX[0] * memoryL[counter] + coefX[1] * prevInp[0] + coefY[1] * prevVal[0]);
		prevInp[0] =memoryL[counter];
		prevVal[0] = *out;
		memoryL[counter] = *out;
		out++; in++;
		*out = (*in) + g * (coefX[0] * memoryR[counter] + coefX[1] * prevInp[1] + coefY[1] * prevVal[1]);
		prevInp[1] = memoryR[counter];
		prevVal[1] = *out;
		memoryR[counter] = *out;
		out++; in++;
		counter = (++counter) % Taps;
	}
}
void CombReverbLP::setA(float a)
{
	A = a;
	calcCoefs();
}

void CombReverbLP::calcCoefs()
{
	/*Se calculan los parametros del pasabajos, normalizados.*/
	float gain = (1 - A) / (1 + A);
	coefX[0] = 1*gain; //Se multiplica para normalizar.
	coefX[1] = A* gain;
	coefY[1] = -A;
}

CombReverbLP::~CombReverbLP()
{
}
