#pragma once
#include <vector>
using namespace std;
class CombReverberator
{
public:
	CombReverberator();
	CombReverberator(float g, unsigned int Taps);
	void setParams(float g, unsigned int Taps);
	void Action(const float *in, float *out, unsigned int len);
	~CombReverberator();
private:
	float g;
	unsigned int Taps, counter;
	vector<float> memoryL, memoryR;
};

