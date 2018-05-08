#pragma once
#include "Effect.h"


#define DELAY_PARAMETERS {"Delay Time", "Feed Forward Coef", "Feed Back Coef","Follower Coef" }

#define DELAY_DEFAULT_FF "0.1" //
#define DELAY_DEFAULT_FB "0.5" //
#define DELAY_DEFAULT_TIME_D "0.75" //En segundos
#define DELAY_DEFAULT_BL "1"

#define DELAY_TIME_D_ERROR_MSG "The maximum delay time is 2.5 secs, and the minimum is 0.02."
#define DELAY_FF_ERROR_MSG "This factor has to be less than 1 and more than -1."
#define DELAY_FB_ERROR_MSG "This factor has to be less than 1 and more than -1."
#define DELAY_BL_ERROR_MSG "This factor has to be between -1 and 1."

//Datos para calcular la memoria necesaria
#define MAX_DELAY_TIME 2.5
#define MIN_DELAY_TIME 0.01

class Delay :
	public Effect
{
public:
	Delay(unsigned int sampleFreq);
	bool setParam(string paramName, string paramValue);
	bool Action(const float * in, float * out, unsigned int len);
	~Delay();
private:
	void saveValues(); //De los vectores de paramValues, guarda todos los valores de los factores
	vector<float> prevInputL;
	vector<float> prevInputR;
	float ff, fb, bl;
	unsigned int prevSamplesNeeded, counter, nmbrOfTaps, maxSamplesNeeded;
};

