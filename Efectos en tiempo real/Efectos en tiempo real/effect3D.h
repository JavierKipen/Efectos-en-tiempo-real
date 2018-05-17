#pragma once
#include "Effect.h"
#include "portaudio.h"
#include <math.h>

#define EFFECT3D_ANGLE_DEFAULT "0"		// en grados

#define EFFECT3D_PARAMETERS {"ANGLE"}

#define PI 3.14159265

class effect3D : public Effect
{
public:
	effect3D(unsigned int sampleFreq);
	bool setParam(string paramName, string paramValue);
	bool Action(const float * in, float * out, unsigned int len);
	~effect3D();
private:
	void hsfilter(const float * input, float * out, unsigned int len, float theta, char channel);
	float lastInL, lastInR;
	float lastTempOutL, lastTempOutR;
	float lastOutL, lastOutR;
	float angle, elevation;

};

