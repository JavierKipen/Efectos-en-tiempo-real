#pragma once
#include "Effect.h"
#include "VarBRFilter.h"
#include "NotchOrder2.h"

#define PHASER_DEFAULT_N_FILTERS "5"
#define PHASER_DEFAULT_GB "0.5"
#define PHASER_DEFAULT_DEPTH "1"
#define PHASER_DEFAULT_SWEEP_WIDTH "1"
#define PHASER_DEFAULT_Q "2"
#define PHASER_DEFAULT_LFO_FREQ "1"

#define PHASER_PARAM_NAMES {"Depth", "Sweep Width", "LFO Freq","Number Of Filters","Q", "GB"}
#define PHASER_DEFAULT_PARAM_VALUES  {PHASER_DEFAULT_DEPTH,PHASER_DEFAULT_SWEEP_WIDTH,PHASER_DEFAULT_LFO_FREQ,PHASER_DEFAULT_N_FILTERS,PHASER_DEFAULT_Q,PHASER_DEFAULT_GB}

class Phaser :
	public Effect
{
public:
	Phaser(unsigned int sampleFreq);
	bool Action(const float * in, float * out, unsigned int len);
	bool setParam(string paramName, string paramValue);
	~Phaser();
private:
	void saveValues();
	vector<NotchOrder2> NotchFilter;
	VarBRFilter varBRFilter;
	void updateFilters();
	unsigned int numberOfFilters, prevNmbrOfFilters, sampleCount;
	float gb, Q, aux[MAX_INPUT_LEN],depth, sweepWidth,lfoFreq, fo;
	float filterMinFrecs[MAX_NMBR_OF_FILTERS];
};

