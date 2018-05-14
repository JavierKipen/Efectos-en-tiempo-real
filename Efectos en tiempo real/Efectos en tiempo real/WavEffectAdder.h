#pragma once
#include "sndfile.h"
#include "Effect.h"
#include "Distortion.h"
#include "Delay.h"
#include "Reverb.h"
#include "Flanger.h"
#include "Chorus.h"
#include "Vibrato.h"
#include "Phaser.h"
#include <iostream>
class WavEffectAdder
{
public:
	WavEffectAdder(int argc, char *argv[]);
	bool initOk();
	void applyEffects();
	~WavEffectAdder();
private:
	bool chooseEffect(string effect);
	Effect *currEffect;
	SF_INFO sfInfoR, sfInfoW;
	SNDFILE *p2ReadFile, *p2WriteFile;
	float bufferInput[MAX_INPUT_LEN], bufferOutput[MAX_INPUT_LEN];
	bool initOkay;
};

