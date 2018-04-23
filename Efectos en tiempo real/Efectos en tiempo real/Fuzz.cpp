#include "Fuzz.h"





Fuzz::Fuzz()
{
	paramNames=FUZZ_PARAMS;
	paramValues = FUZZ_DEFAULT_VALUES;
}


bool Fuzz::setParam(string paramName, double paramValue)
{
	bool retVal=false;
	if (paramName=="Amplifications")
	{
		if (paramValue <= 0 || paramValue > MAX_AMPLIFICATIONS ||  paramValue !=floor(paramValue))
			ErrorMsg = FUZZ_AMP_VALUE_ERROR_MSG;
		else
		{
			paramValues[0] = (unsigned int)paramValue;
			retVal = true;
		}
	}
	return retVal;
}

bool Fuzz::Action(float * in, float * out, unsigned int len)
{
	float aux;
	for (unsigned int i = 0; i < len; i++)
	{
		aux = cubicAmplifier(*in++);
		for (unsigned int j = 1; j < paramValues[1]; j++)
			aux = cubicAmplifier(aux);
		*out++ = aux;
	}
	return false;
}
float Fuzz::cubicAmplifier(float input)
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


Fuzz::~Fuzz()
{
}
