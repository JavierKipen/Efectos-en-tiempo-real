#pragma once
#include <vector>
#include <math.h>

using namespace std;
class CombReverbLP
{
public:
	CombReverbLP(float g, unsigned int Taps, float a); //fc Normalizada!!!
	void Action(const float *in, float *out, unsigned int len);
	void setA(float a);
	~CombReverbLP();
private:
	void calcCoefs();
	float g,A,coefX[2], coefY[2],prevVal[2],prevInp[2];
	unsigned int Taps, counter;
	vector<float> memoryL, memoryR;
};

