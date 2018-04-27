#pragma once
#include "Effect.h"


#define DELAY_PARAMETERS {"Type", "Delay Time", "Feed Forward Coef", "Feed Back Coef","Follower Coef" }

#define DELAY_DEFAULT_TYPE "Universal"
#define DELAY_DEFAULT_FF "0.5" //
#define DELAY_DEFAULT_FB "0.5" //
#define DELAY_DEFAULT_TIME_D "0.25" //En segundos
#define DELAY_DEFAULT_BL "1"

#define DELAY_TYPE_ERROR_MSG "WIP"
#define DELAY_TIME_D_ERROR_MSG "WIP"
#define DELAY_FF_ERROR_MSG "WIP"
#define DELAY_FB_ERROR_MSG "WIP"
#define DELAY_BL_ERROR_MSG "WIP"

//Datos para calcular la memoria necesaria
#define MAX_DELAY_TIME 1.0
#define MAX_DELAY_GAIN 0.5
#define AMPLITUDE_RELATION (1/10.0)  //Es el factor en el cual se torna inaudible la salida para una entrada de voz

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
	unsigned int prevSamplesNeeded, counter, nmbrOfTaps;
};

