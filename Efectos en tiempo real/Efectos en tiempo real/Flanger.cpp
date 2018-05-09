#include "Flanger.h"



Flanger::Flanger(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = FLANGER_PARAMETERS;
	paramValues.resize(paramNames.size(), "0");
	paramValues[0] = FLANGER_DELAY_DEFAULT; //Tipo del delay
	paramValues[1] = FLANGER_WIDTH_DEFAULT; //
	paramValues[2] = FLANGER_DEPTH_DEFAULT; //
	paramValues[3] = FLANGER_FEEDBACK_DEFAULT;
	paramValues[4] = FLANGER_LFO_DEFAULT;
	paramValues[5] = FLANGER_INVERT_DEFAULT;


	paramDelay = stof(paramValues[0])/1000;	// /1000 for being in ms.
	paramWidth = stof(paramValues[1])/1000;
	paramDepth = stof(paramValues[2]);
	paramFB = stof(paramValues[3]);
	paramLFO = stof(paramValues[4]);
	paramInvert = stof(paramValues[5]);

	lfoPhase = 0.0;
	bufferSize = (int)(paramDelay + paramWidth)*sampleFreq + 1;
	if (bufferSize < 1) bufferSize = 1;

	prevInputL.resize(bufferSize, 0);
	prevInputR.resize(bufferSize, 0);

	writeTapR = 0;
	writeTapL = 0;
}


bool Flanger::setParam(string paramName, string paramValue)
{
	bool retVal = true;
	float value = stof(paramValue);

	if (paramName == "DELAY" && value > 0.001 && value < 0.02 )	// [1:20] ms
		paramDelay = value / 1000;
	else if (paramName == "WIDTH" && value > 0.001 && value < 0.02) // [1:20] ms
		paramWidth = value / 1000;
	else if (paramName == "DEPTH" && value > 0.0 && value < 1.0)
		paramDepth = value;
	else if (paramName == "FEEDBACK" && value > 0.0 && value < 1.0)
		paramFB = value;
	else if (paramName == "LFO" && value >= 0.1 && value <= 1)
		paramLFO = value;
	else if (paramName == "INVERT" && (value == -1.0 || value == 1.0))
		paramInvert = value;
	
	bufferSize = (int)(paramDelay + paramWidth)*sampleFreq + 1;
	if (bufferSize < 1) bufferSize = 1;

	prevInputL.clear();
	prevInputR.clear();

	prevInputL.resize(bufferSize, 0);
	prevInputR.resize(bufferSize, 0);

	lfoPhase = 0;
	writeTapR = 0;
	writeTapL = 0;

	return retVal;
}

bool Flanger::Action(const float * in, float * out, unsigned int len)
{
	float phaseR = lfoPhase;
	float phaseL = fmodf(lfoPhase + 0.25f, 1.0f);
	float delayTimeR = 0.0f, delayTimeL = 0.0f, readTapR, readTapL;
	float outR = 0.0f, outL = 0.0f;
	float fraction = 0.0f;
	float interp0, interp1;

	for (unsigned int i = 0; i < len; i++)
	{
		const float inR = *in++;
		const float inL = *in++;

		// --- RIGHT CHANNEL ---
		delayTimeR = paramDelay + paramWidth * (0.5 + 0.5*sinf(2 * PI * phaseR));
		readTapR = fmodf(writeTapR - delayTimeR * (float)sampleFreq + (float)bufferSize, (float)bufferSize);
		if (readTapR < 0) readTapR = 0;

		// LINEAR INTERPOLATION
		fraction = readTapR - floorf(readTapR);
		readTapR = floorf(readTapR);
		interp0 = prevInputR[(int)readTapR];
		interp1 = prevInputR[((int)readTapR + 1) % bufferSize];
		outR = interp0 + fraction * (interp1 - interp0);

		*out++ = inR + outR * paramDepth * paramInvert;
		prevInputR[readTapR] = inR + outR * paramFB;
		if (++readTapR >= bufferSize)
			readTapR -= bufferSize;
		phaseR += paramLFO / sampleFreq;
		if (phaseR >= 1.0f)
			phaseR -= 1.0f;

		// --- LEFT CHANNEL ---
		delayTimeL = paramDelay + paramWidth * (0.5 + 0.5*sinf(2 * PI * phaseL));
		readTapL = fmodf(writeTapL - delayTimeL * (float)sampleFreq + (float)bufferSize, (float)bufferSize);
		if (readTapL < 0) readTapL = 0;

		// LINEAR INTERPOLATION
		fraction = readTapL - floorf(readTapL);
		readTapL = floorf(readTapL);
		interp0 = prevInputL[(int)readTapL];
		interp1 = prevInputL[((int)readTapL + 1) % bufferSize];
		outL = interp0 + fraction * (interp1 - interp0);

		*out++ = inL + outL * paramDepth * paramInvert;
		prevInputL[readTapL] = inL + outL * paramFB;
		if (++readTapL >= bufferSize)
			readTapL -= bufferSize;
		phaseL += paramLFO / sampleFreq;
		if (phaseL >= 1.0f)
			phaseL -= 1.0f;

	}
	lfoPhase = phaseR;	

	return true;
}


Flanger::~Flanger()
{
}

/*
for (unsigned int i = 0; i < 2 * len; i++) //Dos veces len por ser estereo.
{
currentTap = oldestTap - 1 - tapDelay;
while (currentTap < 0)
{
currentTap += bufferSize;
}

*out++ =  prevInputL[(currentTap)];
prevInputL[oldestTap] = *in++;

*out++ =  prevInputR[(currentTap)];
prevInputR[oldestTap] = *in++;

oldestTap = (++oldestTap) % bufferSize;

tapDelay = abs(floor(delayPeak*sampleFreq*(0.5 + 0.5*sin(2 * PI * delayFreq * counter++))));

}


if (counter > (1 / delayFreq))
counter = 0;
*/