#pragma once
#include "Effect.h"


#define DELAY_PARAMETERS {"Type", "Delay Time", "Feed Forward Coef", "Feed Back Coef","Follower Coef" }

#define DELAY_DEFAULT_TYPE "Universal"
#define DELAY_DEFAULT_FF "0.1" //
#define DELAY_DEFAULT_FB "0.5" //
#define DELAY_DEFAULT_TIME_D "0.75" //En segundos
#define DELAY_DEFAULT_BL "1"

#define DELAY_TYPE_ERROR_MSG "The only possible type is Basic."
#define DELAY_TIME_D_ERROR_MSG "The maximum delay time is 2.5 secs, and the minimum is 0.02."
#define DELAY_FF_ERROR_MSG "This factor has to be equal or less than 0.99, and positive."
#define DELAY_FB_ERROR_MSG "This factor has to be equal or less than 0.99, and positive."
#define DELAY_BL_ERROR_MSG "This factor has to be equal or less than 1, and positive."

//Datos para calcular la memoria necesaria
#define MAX_DELAY_TIME 2.5
#define MAX_DELAY_GAIN 0.99
#define MIN_DELAY_TIME 0.02

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

