#pragma once
#include <vector>
#include <math.h>

using namespace std;
class CombReverbLP
{
public:
	CombReverbLP(float g, unsigned int Taps, float a); //fc Normalizada!!!
	void Action(const float *in, float *out, unsigned int len);
	~CombReverbLP();
private:
	float g,A;
	unsigned int Taps, counter;
	vector<float> memoryL, memoryR;
};

