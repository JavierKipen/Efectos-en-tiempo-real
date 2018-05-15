#include "Flanger.h"


Flanger::Flanger(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = FLANGER_PARAMETERS;
	paramValues.resize(paramNames.size(), "0");
	paramValues[0] = FLANGER_DELAY_DEFAULT; //Tipo del delay
	paramValues[1] = FLANGER_WIDTH_DEFAULT; //
	paramValues[2] = FLANGER_FB_DEFAULT; //
	paramValues[3] = FLANGER_FREQ_DEFAULT;

	paramDelay = stof(paramValues[0]) / 1000.0f;
	paramWidth = stof(paramValues[1]) / 1000.0f;
	FB = stof(paramValues[2]);
	paramFreq = stof(paramValues[3]);
	BL = stof(FLANGER_BL_DEFAULT);
	FF = stof(FLANGER_FF_DEFAULT);

	bufferSize = (int)((paramDelay + paramWidth)*sampleFreq) + 1;
	if (bufferSize < 1) bufferSize = 1;

	prevInputL.resize(bufferSize, 0);
	prevInputR.resize(bufferSize, 0);

	counter = 0;
	writeTapR = 0;
	writeTapL = 0;
}

bool Flanger::Action(const float * in, float * out, unsigned int len)
{

	float phaseL, phaseR;
	float delayTimeR = 0.0f, delayTimeL = 0.0f, readTapR, readTapL;
	float outR = 0.0f, outL = 0.0f;
	float fraq = 0.0f, interp0, interp1;
	float xh;

	for (unsigned int i = 0; i < len; i++)
	{
		const float inL = *in++;
		const float inR = *in++;

		// --- LEFT CHANNEL ---
		phaseL = (paramFreq * (float)counter++) / sampleFreq;
		if (phaseL >= 1.0f)
		{
			phaseL = 0;
			counter = 0;
		}

		delayTimeL = paramDelay + paramWidth * (0.5f + 0.5f*sinf(2.0f * (float)PI * phaseL));
		readTapL = fmodf(writeTapL - delayTimeL * (float)sampleFreq + (float)bufferSize, (float)bufferSize);
		while (readTapL < 0) readTapL += bufferSize;

		// LINEAR INTERPOLATION
		fraq = readTapL - floorf(readTapL);
		readTapL = floorf(readTapL);
		interp0 = prevInputL[(int)readTapL];
		interp1 = prevInputL[((int)readTapL + 1) % bufferSize];
		outL = interp0 + fraq * (interp1 - interp0);

		xh = inL + FB * outL;

		*out++ = xh * BL + outL * FF;
		prevInputL[(int)writeTapL] = inL + outL * FB;
		if (++writeTapL >= bufferSize)
			writeTapL -= bufferSize;


		// --- RIGHT CHANNEL ---
		phaseR = fmodf(phaseL + 0.25f, 1.0f);

		delayTimeR = paramDelay + paramWidth * (0.5f + 0.5f*sinf(2.0f * (float)PI * phaseR));
		readTapR = fmodf(writeTapR - delayTimeR * (float)sampleFreq + (float)bufferSize, (float)bufferSize);
		while (readTapR < 0) readTapR += bufferSize;

		// LINEAR INTERPOLATION
		fraq = readTapR - floorf(readTapR);
		readTapR = floorf(readTapR);
		interp0 = prevInputR[(int)readTapR];
		interp1 = prevInputR[((int)readTapR + 1) % bufferSize];
		outR = interp0 + fraq * (interp1 - interp0);

		xh = inR + FB * outR;

		*out++ = xh * BL + outR * FF;
		prevInputR[(int)writeTapR] = inR + outR * FB;
		if (++writeTapR >= bufferSize)
			writeTapR -= bufferSize;

	}

	return true;
	
}



bool Flanger::setParam(string paramName, string paramValue)
{
	bool retVal = true;
	float value = stof(paramValue);

	if (paramName == "DELAY" && value >= 1.0f && value <= 20.0f)	// [1:20] ms
	{
		paramDelay = value / 1000.0f;
		paramValues[0] = paramValue;
	}
	else if (paramName == "WIDTH" && value >= 1.0f && value <= 2.0f) // [1:2] ms
	{
		paramWidth = value / 1000.0f;
		paramValues[1] = paramValue;
	}
	else if (paramName == "FEEDBACK" && value >= 0.0f && value <= 1.0f)
	{
		FB = value;
		paramValues[2] = paramValue;
	}
	else if (paramName == "FREQUENCY" && value >= 0.1f && value <= 1.0f)
	{
		paramFreq = value;
		paramValues[3] = paramValue;
	}
	
	bufferSize = (int)((paramDelay + paramWidth)*sampleFreq) + 1;
	if (bufferSize < 1) bufferSize = 1;

	prevInputL.resize(bufferSize, 0);
	prevInputR.resize(bufferSize, 0);

	counter = 0;
	writeTapR = 0;
	writeTapL = 0;

	return retVal;
}
Flanger::~Flanger()
{
}

