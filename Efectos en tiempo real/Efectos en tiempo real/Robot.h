#pragma once
#include "Effect.h"
#include "FFT.h"

#define ROBOT_PARAM_NAMES {"Windowed"}
#define ROBOT_PARAM_VALUES {"Yes"}

#define ROBOT_DEFAULT_LEN 512
class Robot :
	public Effect
{
public:
	Robot(unsigned int sampleFreq,unsigned int N);
	bool Action(const float * in, float * out, unsigned int len);
	bool setParam(string paramName, string paramValue);
	~Robot();
private:
	unsigned int N;
	void windowBlocks();
	void deWindowBlocks();
	void robotizeBlocks();
	void savePreviousData();
	void robotize(complex<double> *a);
	void obtainOuputs();
	bool windowed;
	vector<complex<double>> inL, inR, HannWindow,prevInR,prevInL, OutL, OutR;
	vector<complex<double>> block1L, block2L, block1R, block2R;
};

