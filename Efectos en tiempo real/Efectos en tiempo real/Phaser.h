#pragma once
#include "Effect.h"
#include "VarBRFilter.h"
#include "NotchOrder2.h"

#define PHASER_DEFAULT_DEPTH "1"
#define PHASER_DEFAULT_SWEEP_WIDTH "0.4"
#define PHASER_DEFAULT_MIN_FREQ "220"
#define PHASER_DEFAULT_LFO_FREQ "2"
#define PHASER_DEFAULT_LFO_TYPE "Exp Triang"
#define PHASER_DEFAULT_R "0.9"


#define PHASER_PARAM_NAMES {"Depth", "Sweep Width", "Min Frequency", "LFO Freq","LFO Type", "Pole Ratio"}
#define PHASER_DEFAULT_PARAM_VALUES  {PHASER_DEFAULT_DEPTH,PHASER_DEFAULT_SWEEP_WIDTH,PHASER_DEFAULT_MIN_FREQ, PHASER_DEFAULT_LFO_FREQ,PHASER_DEFAULT_LFO_TYPE, PHASER_DEFAULT_R}

#define PHASER_MIN_LFO_FREQ 0.1
#define PHASER_MAX_LFO_FREQ 5

#define PHASER_DEPTH_ERROR_MSG "Depth must go between 0 and 1."
#define PHASER_SWEEP_WIDTH_ERROR_MSG "Sweep width must go between 0 and 1."
#define PHASER_MIN_FREQ_ERROR_MSG "This frequency must be audible! (more than 20Hz and less than 20KHz)."
#define PHASER_LFO_FREQ_ERROR_MSG "The LFO frequency must be less than 5Hz and more than 0.1Hz"
#define PHASER_LFO_TYPE_ERROR_MSG "The avaiable types are Sine, Triang and Exp Triang"
#define PHASER_R_ERROR_MSG "This value must be less than 1 and positive."



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
	float getNotchFreq();
	NotchOrder2 notchFilter;
	unsigned int sampleCount;
	float R, aux[MAX_INPUT_LEN],depth, sweepWidth,lfoFreq, fo;
};

