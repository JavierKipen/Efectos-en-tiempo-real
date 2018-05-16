#pragma once
#include "sndfile.h"
#include "Effect.h"
#include "Distortion.h"
#include "Delay.h"
#include "Reverb.h"
#include "Flanger.h"
#include "Chorus.h"
#include "effect3D.h"
#include "Vibrato.h"
#include "Phaser.h"
#include "Robot.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <conio.h>

#define START_OK_MSG "Welcome to real time effects!\n The .wav file that you've entered as second argument, \n will be passed through the effect you added in the third argument.\n Press any key to start processing\n"
#define FINISHED_OK_MSG "The output file is in '.../Efectos en tiempo real/Output/' with the same name as the input.\n Press any key to exit.\n"

#define ARG_C_ERROR_MSG "The arguments to call this program must be 1 or 3.\n 1 - Real time effects\n 3 - WAV Processing \n "
#define WAV_FILENAME_ERROR_MSG "The second argument must be a .wav in the same folder as the solution.\n"
#define EFFECT_CHOSEN_ERROR_MSG "The third argument must be one of the effets.\n This are the possible effects:\nDelay\nReverb\nDistortion\nPhaser\nVibrato\nChorus\nFlanger\n3D\n"

#define ERROR_MSG "Press any key to end...\n"

#define FPS 30.0f

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

