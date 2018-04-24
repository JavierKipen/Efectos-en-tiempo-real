#include "Fuzz.h"





Fuzz::Fuzz(unsigned int sampleFreq)
{
	paramNames=FUZZ_PARAMS;
	paramValues = FUZZ_DEFAULT_VALUES;
	this->sampleFreq = sampleFreq;
}


bool Fuzz::setParam(string paramName, string paramValue)
{
	bool retVal=false;
	if (paramName=="Amplifications")
	{
		float aux = stof(paramValue);
		if (aux <= 0 || aux > MAX_AMPLIFICATIONS ||  aux !=floor(aux)) //El número de amplificaciones debe ser entero, positivo y menor a 5
			ErrorMsg = FUZZ_AMP_VALUE_ERROR_MSG;
		else
		{
			paramValues[0] = paramValue;
			retVal = true;
		}
	}
	return retVal;
}

bool Fuzz::Action(const float * in, float * out, unsigned int len)
{
	float aux;
	for (unsigned int i = 0; i < 2*len; i++) //Dos veces len por ser estereo.
	{
		aux = cubicAmplifier(*in++);
		for (unsigned int j = 1; j < (unsigned int)stof(paramValues[0]); j++)
			aux = cubicAmplifier(aux);
		*out++ = aux;
	}
	return true;
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
