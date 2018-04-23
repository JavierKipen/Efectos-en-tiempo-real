#pragma once
#include "Effect.h"
#include "portaudio.h"

#define DEFAULT_AMPLIFICATIONS 3
#define MAX_AMPLIFICATIONS 5

#define FUZZ_AMP_VALUE_ERROR_MSG "The value of amplifications must be a positive integer between 1 and 5"
#define FUZZ_PARAMS {"Amplifications"}
#define FUZZ_DEFAULT_VALUES {DEFAULT_AMPLIFICATIONS}

class Fuzz :
	public Effect
{
public:
	Fuzz();
	bool setParam(string paramName, double paramValue);
	bool Action(float * in, float * out, unsigned int len);
	~Fuzz();
private:
	float cubicAmplifier(float);
};

