#pragma once
#include <vector>
class PlaneReverb
{
public:
	PlaneReverb(float g, unsigned int Taps);
	void Action(const float *in, float *out, unsigned int len);
	~PlaneReverb();
private:
	float g;
	unsigned int Taps, counter;
	vector<float> memoryL, memoryR;
};

