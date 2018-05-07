#pragma once
#include "Effect.h"
#include "VarBRFilter.h"

#define PHASER_DEFAULT_N_FILTERS "2"
#define PHASER_DEFAULT_GB "0.1"
#define PHASER_DEFAULT_DEPTH "0.5"
#define PHASER_DEFAULT_SWEEP_WIDTH "0.5"
#define PHASER_DEFAULT_Q "15"
#define PHASER_DEFAULT_LFO_FREQ "1"

#define PHASER_PARAM_NAMES {"Depth", "Sweep Width", "LFO Freq","Number Of Filters","Q", "GB"}
#define PHASER_DEFAULT_PARAM_VALUES  {PHASER_DEFAULT_DEPTH,PHASER_DEFAULT_SWEEP_WIDTH,PHASER_DEFAULT_LFO_FREQ,PHASER_DEFAULT_N_FILTERS,PHASER_DEFAULT_Q,PHASER_DEFAULT_GB}

class Phaser :
	public Effect
{
public:
	Phaser(unsigned int sampleFreq);
	bool Action(const float * in, float * out, unsigned int len);
	~Phaser();
private:
	void saveValues();
	VarBRFilter varBRFilter;
	void updateFilters();
	unsigned int numberOfFilters, prevNmbrOfFilters;
	float gb, Q, aux[MAX_INPUT_LEN],depth, sweepWidth,lfoFreq;
	float filterMinFrecs[MAX_NMBR_OF_FILTERS];
};

