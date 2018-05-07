#pragma once
#include <math.h>
#include <vector>

using namespace std;
//Band Reject IIR por bilineal, creado por https://www.dsprelated.com/showcode/173.php. Modificado para que sea estereo
#define NUMER_OF_COEFS 3
class BandRejectIIROrder2
{
public:
	BandRejectIIROrder2();
	void setSampleFreq(float sf);
	void setNmbrOfCoefsPreCalc(unsigned int n);
	void setParameters(float * freqsNorm, float gb, float Q);
	void filter(float *in, float *out, unsigned int len, unsigned int i); //i indica que coeficientes utilizará.
	~BandRejectIIROrder2();
private:
	void reserveMemory();
	unsigned int nmbrOfCoefsPreCalc;
	float sampleFreq, e,p;
	vector<vector<float>> coefs; //Cada vector contendra un vector con los coeficientes para el filtro de la iésima frecuencia.
	float memoryXL[NUMER_OF_COEFS], memoryXR[NUMER_OF_COEFS];
	float memoryYL[NUMER_OF_COEFS], memoryYR[NUMER_OF_COEFS];
};

