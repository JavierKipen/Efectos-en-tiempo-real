#pragma once
#include <vector>

using namespace std;

class BasicReverberator
{
public:
	BasicReverberator();
	BasicReverberator(float g, unsigned int Taps);
	void setParameters(float g, unsigned int Taps);
	void Action(const float *in, float *out, unsigned int len);
	~BasicReverberator();
private:
	float g;
	unsigned int Taps,counter;
	vector<float> memoryL, memoryR;
};

