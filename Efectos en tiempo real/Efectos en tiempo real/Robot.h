#pragma once
#include "Effect.h"
#include "FFT.h"

#define ROBOT_DEFAULT_LEN 512
class Robot :
	public Effect
{
public:
	Robot(unsigned int sampleFreq);
	bool Action(const float * in, float * out, unsigned int len);
	bool setParam(string paramName, string paramValue);
	~Robot();
private:
	void windowBlocks();
	void deWindowBlocks();
	void robotizeBlocks();
	void savePreviousData();
	void robotize(complex<float> *a);
	void obtainOuputs();
	bool windowed;
	vector<complex<float>> inL, inR, HannWindow,prevInR,prevInL, OutL, OutR;
	vector<complex<float>> block1L, block2L, block1R, block2R;
};

