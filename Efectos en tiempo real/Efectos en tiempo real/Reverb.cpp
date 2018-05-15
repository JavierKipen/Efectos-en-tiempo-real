#include "Reverb.h"

#define REVERB_TYPE_INDEX 0
#define REVERB_G_INDEX 1
#define REVERB_T_DELAY_INDEX 2
#define REVERB_FC_INDEX 3

Reverb::Reverb(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = REVERB_DEFAULT_PARAM_NAMES;
	paramValues = REVERB_DEFAULT_PARAM_VALUES;
	saveValues();
	Full = NULL;
	plane = new PlaneReverb(g,nmbrOfTaps);
	LP = NULL;
}

bool Reverb::Action(const float * in, float * out, unsigned int len)
{
	float aux;
	unsigned int delayedInputIndex = 0;
	if (paramValues[REVERB_TYPE_INDEX] == "Full")
		Full->Action(in, out, len);
	else if (paramValues[REVERB_TYPE_INDEX] == "Plane")
		plane->Action(in, out, len);
	else if (paramValues[REVERB_TYPE_INDEX] == "LP")
		LP->Action(in, out, len);
	return true;
}

bool Reverb::setParam(string paramName, string paramValue)
{
	bool retVal = false;
	if (paramName == "Type")
	{
		if (paramValue == "Full")
		{	
			paramNames = REVERB_FULL_PARAM_NAMES;
			paramValues = REVERB_FULL_DEFAULT_PARAM_VALUES;
			retVal = true;
		}
		else if (paramValue == "Plane")
		{
			paramNames = REVERB_PLANE_PARAM_NAMES;
			paramValues = REVERB_PLANE_DEFAULT_PARAM_VALUES;
			retVal = true;
		}
		else if (paramValue == "LP")
		{
			paramNames = REVERB_LP_PARAM_NAMES;
			paramValues = REVERB_LP_DEFAULT_PARAM_VALUES;
			retVal = true;
		}
	}
	else if (paramName == "G Factor")
	{
		if (stof(paramValue) < REVERB_SCHROEDER_MAX_G && stof(paramValue) > 0)
		{	
			paramValues[REVERB_G_INDEX] = paramValue; 
			retVal = true;
		}
		else
			ErrorMsg = REVERB_G_ERROR_MSG;
	}
	else if (paramName == "A Factor")
	{
		if (stof(paramValue) < REVERB_SCHROEDER_MAX_G && stof(paramValue) > 0)
		{
			paramValues[2] = paramValue;
			retVal = true;
		}
		else
			ErrorMsg = REVERB_G_ERROR_MSG;
	}
	else if (paramName == "Depth")
	{
		if (stof(paramValue) < REVERB_SCHROEDER_MAX_G && stof(paramValue) > 0)
		{
			paramValues[3] = paramValue;
			retVal = true;
		}
		else
			ErrorMsg = REVERB_G_ERROR_MSG;
	}
	else if (paramName == "Delay Time")
	{
		if (stof(paramValue) <= REVERB_SCHROEDER_MAX_T_DELAY && stof(paramValue) > 0)
		{
			paramValues[REVERB_T_DELAY_INDEX] = paramValue; 
			retVal = true;
		}
		else
			ErrorMsg = REVERB_T_DELAY_ERROR_MSG;
	}
	else if (paramName == "LP Factor")
	{
		if (stof(paramValue) < 1 && stof(paramValue) > 0)
		{
			paramValues[REVERB_FC_INDEX] = paramValue;
			retVal = true;
		}
		else
			ErrorMsg = REVERB_FC_ERROR_MSG;
	}
	if (retVal == true)
	{
		saveValues();
		updateReverb();
	}
	
	return retVal;
}

void Reverb::updateReverb()
{
	if (paramValues[REVERB_TYPE_INDEX] == "Full")
	{	if(Full != NULL)
			delete Full;
		Full = new CompleteReverb(a,g, depth,(float) sampleFreq);
	}
	else if (paramValues[REVERB_TYPE_INDEX] == "Plane")
	{	
		if (plane != NULL)
			delete plane;
		plane = new PlaneReverb(g, nmbrOfTaps);
	}
	else if (paramValues[REVERB_TYPE_INDEX] == "LP")
	{	
		if (LP != NULL)
		delete LP; 
		LP = new CombReverbLP(g, nmbrOfTaps,a);
	}
}
void Reverb::saveValues()
{
	nmbrOfTaps = unsigned int(stof(paramValues[REVERB_T_DELAY_INDEX]) * (float)(sampleFreq)+1);
	g = stof(paramValues[REVERB_G_INDEX]);
	if (paramValues[REVERB_TYPE_INDEX] == "LP")
		a = stof(paramValues[REVERB_FC_INDEX]);
	if (paramValues[REVERB_TYPE_INDEX] == "Full")
	{
		a = stof(paramValues[2]);
		depth = stof(paramValues[3]);
	}
}

Reverb::~Reverb()
{
}


