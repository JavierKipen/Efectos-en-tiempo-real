#include "BandRejectIIROrder2.h"

#define PI 3.14159265


BandRejectIIROrder2::BandRejectIIROrder2()
{
	this->sampleFreq = sampleFreq;
	for (unsigned int j = 0; j < NUMER_OF_COEFS; j++)
	{
		memoryXL[j] = 0;
		memoryXR[j] = 0;
		memoryYL[j] = 0;
		memoryYR[j] = 0;
	}
}
void BandRejectIIROrder2::setSampleFreq(float sf)
{
	this->sampleFreq = sf;
}
void BandRejectIIROrder2::setNmbrOfCoefsPreCalc(unsigned int n)
{
	nmbrOfCoefsPreCalc = n;
	reserveMemory();
}
void BandRejectIIROrder2::setParameters(float * freqsNorm, float gb, float Q)
{
	float wo;
	float damp= sqrt(1 - pow(gb, 2)) / gb;
	for (unsigned int i = 0; i < nmbrOfCoefsPreCalc; i++)
	{
		wo = 2 * PI * freqsNorm[i];
		e = 1 / (1 + damp * tan(wo / (Q * 2)));
		p = cos(wo);
		coefs[i][0] = e;
		coefs[i][1] = 2 * e * p;
		coefs[i][2] = (2 * e - 1);
	}
	
}

void BandRejectIIROrder2::filter(float * in, float * out, unsigned int len, unsigned int i) 
{
	for (unsigned int j = 0; j < len; j++)
	{
		/*Se actualizan los valores previos de entrada, se calcula el de salida y se calcula el de salida, para L Y R.*/
		memoryXL[0] = memoryXL[1];
		memoryXL[1] = memoryXL[2];
		memoryXL[2] = *in++;

		memoryYL[0] = memoryYL[1];
		memoryYL[1] = memoryYL[2];

		memoryYL[2] = coefs[i][0] * memoryXL[2] - coefs[i][1] * memoryXL[1] + coefs[i][0] * memoryXL[0]+ coefs[i][1] * memoryYL[1]-coefs[i][2] * memoryYL[0];
		*out++ = memoryYL[2];

		memoryXR[0] = memoryXR[1];
		memoryXR[1] = memoryXR[2];
		memoryXR[2] = *in++;

		memoryYR[0] = memoryYR[1];
		memoryYR[1] = memoryYR[2];

		memoryYR[2] = coefs[i][0] * memoryXR[2] - coefs[i][1] * memoryXR[1] + coefs[i][0] * memoryXR[0] + coefs[i][1] * memoryYR[1] - coefs[i][2] * memoryYR[0];
		*out++ = memoryYR[2];
	}
}
void BandRejectIIROrder2::reserveMemory()
{
	vector<float> aux;
	aux.resize(NUMER_OF_COEFS, 0.0f);
	coefs.resize(nmbrOfCoefsPreCalc, aux);
}
BandRejectIIROrder2::~BandRejectIIROrder2()
{
}
