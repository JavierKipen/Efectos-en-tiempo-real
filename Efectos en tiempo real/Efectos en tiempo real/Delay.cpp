#include "Delay.h"

#define MAX(x,y) (x>y ? x:y)

Delay::Delay(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = DELAY_PARAMETERS;
	paramValues = { DELAY_DEFAULT_TYPE ,DELAY_DEFAULT_TIME_D, DELAY_DEFAULT_FF, DELAY_DEFAULT_FB, DELAY_DEFAULT_BL };
	//Para saber cuantas muestras guardar, se para que escuche las repeticiones hasta que la señal de salida sea AMPLITUDE_RELATION veces menor que la de entrada
	maxSamplesNeeded =(unsigned int) (MAX_DELAY_TIME * (float)(sampleFreq) + 1);
	prevInputL.resize(maxSamplesNeeded,0);
	prevInputR.resize(maxSamplesNeeded,0);
	counter = 0;
	saveValues();
}

bool Delay::setParam(string paramName, string paramValue)
{
	bool retVal = false;
	float paramValuef = 0;
	if (paramName == "Type")
	{
		if (paramValue == "Universal")
		{
			paramValues[0] = paramValue; retVal = true;
		}
		else
			ErrorMsg = DELAY_TYPE_ERROR_MSG;
	}
	else 
	{
		paramValuef = stof(paramValue);
		if (paramName == "Delay Time" )
		{	
			if(paramValuef >= 0 && paramValuef <= MAX_DELAY_TIME && paramValuef > MIN_DELAY_TIME)
			{	paramValues[1] = paramValue; retVal = true;}
			else
				ErrorMsg = DELAY_TIME_D_ERROR_MSG;
		}
		else if (paramName == "Feed Forward Coef")
		{
			if (paramValuef >= -1 && paramValuef <= MAX_DELAY_GAIN)
			{
				paramValues[2] = paramValue; retVal = true;
			}
			else
				ErrorMsg = DELAY_FF_ERROR_MSG;
		}
		else if (paramName == "Feed Back Coef")
		{
			if (paramValuef >= -1 && paramValuef <= MAX_DELAY_GAIN)
			{
				paramValues[3] = paramValue; retVal = true;
			}
			else
				ErrorMsg = DELAY_FB_ERROR_MSG;
		}
		else if (paramName == "Follower Coef")
		{
			if (paramValuef >= -1 && paramValuef <= 1)
			{
				paramValues[4] = paramValue; retVal = true;
			}
			else
				ErrorMsg = DELAY_BL_ERROR_MSG;
		}
	}
	saveValues();
	return retVal;
}

bool Delay::Action(const float * in, float * out, unsigned int len)
{
	float aux;
	unsigned int delayedInputIndex=0;
	if(paramValues[0]== "Universal" )
	for (unsigned int i = 0; i < len; i++) //Dos veces len por ser estereo.
	{
		*out++ = bl * (*in) + (bl * fb + ff) * prevInputL[counter];
		prevInputL[counter] = bl * ((*in++) + fb * prevInputL[counter]);
		*out++ = bl * (*in) + (bl * fb + ff) * prevInputR[counter];
		prevInputR[counter] = bl * ((*in++) + fb * prevInputR[counter]);
		counter = (++counter) % nmbrOfTaps;
	}
		
	return true;
}


Delay::~Delay()
{
}

void Delay::saveValues()
{
	nmbrOfTaps = (unsigned int)(stof(paramValues[1])* (float)(sampleFreq))+1;
	ff = stof(paramValues[2]);
	fb = stof(paramValues[3]);
	bl = stof(paramValues[4]);

}
