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
void BandRejectIIROrder2::setParameters(float * freqsNorm, float R)
{
	/*float wo;
	float damp= sqrt(1 - pow(gb, 2)) / gb;
	for (unsigned int i = 0; i < nmbrOfCoefsPreCalc; i++)
	{
		wo = 2 * PI * freqsNorm[i];
		e = 1 / (1 + damp * tan(wo / (Q * 2)));
		p = cos(wo);
		coefs[i][0] = e;
		coefs[i][1] = 2 * e * p;
		coefs[i][2] = (2 * e - 1);
	}*/ 
	float wo;
	for (unsigned int i = 0; i < nmbrOfCoefsPreCalc; i++)
	{
		wo = 2 * PI * freqsNorm[i];
		coefsX[i][0] = 1; //Coeficiente del filtro para la frecuencia i, que acompaña al término de z^0 de la entrada.
		coefsX[i][1] = (-2)*cos(wo);
		coefsX[i][2] = 1;
		coefsY[i][1] = 2*R*cos(wo);
		coefsY[i][2] = (-R)*R;
		gains[i] = 1; //Después esta ganancia se debe ajustar.
	}
	
}

void BandRejectIIROrder2::filter(float * in, float * out, unsigned int len, unsigned int i) 
{
	for (unsigned int j = 0; j < len; j++)
	{
		/*Se actualizan los valores previos de entrada, se calcula el de salida y se calcula el de salida, para L Y R.*/
		/*0 será el valor más nuevo, 2 el más viejo.*/
		memoryXL[2] = memoryXL[1];
		memoryXL[1] = memoryXL[0];
		memoryXL[0] = *in++;
		
		memoryYL[2] = memoryYL[1];
		memoryYL[1] = memoryYL[0];

		memoryYL[0] = gains[i]*(coefsX[i][0] * memoryXL[0] + coefsX[i][1] * memoryXL[1] + coefsX[i][2] * memoryXL[2]+ coefsY[i][1] * memoryYL[1] + coefsY[i][2] * memoryYL[2]);
		*out++ = memoryYL[0];

		memoryXR[2] = memoryXR[1];
		memoryXR[1] = memoryXR[0];
		memoryXR[0] = *in++;

		memoryYR[2] = memoryYR[1];
		memoryYR[1] = memoryYR[0];

		memoryYR[0] = gains[i] * (coefsX[i][0] * memoryXR[0] + coefsX[i][1] * memoryXR[1] + coefsX[i][2] * memoryXR[2] + coefsY[i][1] * memoryYR[1] + coefsY[i][2] * memoryYR[2]);
		*out++ = memoryYR[0];
	}
}
void BandRejectIIROrder2::reserveMemory()
{
	vector<float> aux;
	aux.resize(NUMER_OF_COEFS, 0.0f);
	coefsX.resize(nmbrOfCoefsPreCalc, aux);
	coefsY.resize(nmbrOfCoefsPreCalc, aux);
	gains.resize(nmbrOfCoefsPreCalc, 0);
}
BandRejectIIROrder2::~BandRejectIIROrder2()
{
}
