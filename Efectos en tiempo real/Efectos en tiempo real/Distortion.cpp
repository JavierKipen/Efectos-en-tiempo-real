#include "Distortion.h"

#define DISTORTION_TYPE_INDEX 0

Distortion::Distortion(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	clippingGain = 1;
	paramNames = DISTORTION_SOFT_CLIPPING_PARAM_NAMES;
	paramValues = DISTORTION_SOFT_CLIPPING_DEFAULT_PARAM_VALUES;
}

bool Distortion::setParam(string paramName, string paramValue)
{
	bool retVal = false;
	float aux;
	if (paramName == "Type")
	{
		if (paramValue == "Soft_Clipping")
		{
			paramNames = DISTORTION_SOFT_CLIPPING_PARAM_NAMES;
			paramValues = DISTORTION_SOFT_CLIPPING_DEFAULT_PARAM_VALUES;
			paramValues[0] = paramValue;
			retVal = true;
		}
		else if (paramValue == "Hard_Clipping")
		{
			paramNames = DISTORTION_HARD_CLIPPING_PARAM_NAMES;
			paramValues = DISTORTION_HARD_CLIPPING_DEFAULT_PARAM_VALUES;
			paramValues[0] = paramValue;
			retVal = true;
		}
		if (paramValue == "Full_Wave_Rectifier" || paramValue == "Fuzz")
		{
			paramNames = DISTORTION_OTHER_PARAM_NAMES;
			paramValues = DISTORTION_OTHER_PARAM_NAMES;
			paramValues[0] = paramValue;
			retVal = true;
		}
		else
			ErrorMsg=DISTORTION_TYPE_ERROR_MSG;
	}
	else
	{
		aux = stof(paramValue);
		
		if (paramValues[DISTORTION_TYPE_INDEX] == "Hard_Clipping")
		{
			if (aux > 0 && aux < 1)
			{
				paramValues[1] = paramValue; retVal = true;
			}
			else
				ErrorMsg = DISTORTION_THRESHOLD_ERROR_MSG;
		}
		else if (paramValues[DISTORTION_TYPE_INDEX] == "Soft_Clipping")
		{
			if (aux > 0)
			{
				paramValues[1] = paramValue; retVal = true;
			}
			else
				ErrorMsg = DISTORTION_GAIN_ERROR_MSG;
		}
	}
	if (retVal == true)
		saveValues();
	return retVal;
}

bool Distortion::Action(const float * in, float * out, unsigned int len)
{
	float aux;
	for (unsigned int i = 0; i < 2 * len; i++) //Dos veces len por ser estereo.
		*out++ = applyDistortion(*in++);
	return true;
}

void Distortion::saveValues()
{
	if (paramValues[DISTORTION_TYPE_INDEX] == "Soft_Clipping")
		clippingGain = stof(paramValues[1]);
	if (paramValues[DISTORTION_TYPE_INDEX] == "Hard_Clipping")
		threshold = stof(paramValues[1]);
}
float Distortion::applyDistortion(float input)
{
	float retVal = 0;
	if (paramValues[DISTORTION_TYPE_INDEX] == "Hard_Clipping")
	{
		if (input < (-1)*threshold)
			retVal = (-1)*threshold;
		else if (input > threshold)
			retVal = threshold;
		else
			retVal = input;
	}
	else if (paramValues[DISTORTION_TYPE_INDEX] == "Soft_Clipping")
		retVal = SIGN(input)*(1 - exp(abs(clippingGain*input)));
	else if (paramValues[DISTORTION_TYPE_INDEX] == "Full_Wave_Rectifier")
		retVal = abs(input);
	else if (paramValues[DISTORTION_TYPE_INDEX] == "Fuzz")
		retVal = cubicAmplifier(input);

	return retVal;
}
float Distortion::cubicAmplifier(float input)
{
	float output, temp;
	if (input < 0.0)
	{
		temp = input + 1.0f;
		output = (temp * temp * temp) - 1.0f;
	}
	else
	{
		temp = input - 1.0f;
		output = (temp * temp * temp) + 1.0f;
	}

	return output;
}
Distortion::~Distortion()
{
}


