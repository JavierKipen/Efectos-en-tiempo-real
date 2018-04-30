#include "Reverb.h"



Reverb::Reverb()
{
	paramNames = REVERB_DEFAULT_PARAM_NAMES;
	paramValues = REVERB_DEFAULT_PARAM_VALUES;
	maxSamplesNeeded = (unsigned int)(REVERB_SCHROEDER_MAX_T_DELAY  * (float)(sampleFreq)+1);
	prevInputL.resize(maxSamplesNeeded, 0);
	prevInputR.resize(maxSamplesNeeded, 0);
}


Reverb::~Reverb()
{
}
