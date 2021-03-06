#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "portaudio.h"

#define GEN_MOD(x,y) ((x)-(floor(((x)/(y)))*y)) //La funci�n m�dulo m�s general
#define RAMP_WAVE(x,y) (GEN_MOD((x),(y))/(y)) //RAMP
#define TRI_WAVE(x,y) (abs(2*RAMP_WAVE(x,y)-1))
#define SIGN(x) ((x > 0) ? 1.0f : ((x < 0) ? -1.0f : 0))

#define MAX_AUDIBLE_FREQ 20000.0f
#define MIN_AUDIBLE_FREQ 20.0f

#define MAX_INPUT_LEN 2048

using namespace std;

class Effect
{
public:
	Effect();
	Effect(unsigned int sampleFreq);
	vector<string> getParamNames();
	vector<string> getParamValues();
	virtual bool setParam(string paramName, string paramValue)=0;//Setea un parametro del efecto, devuelve false si el valor no es v�lido (depende da cada par�metro).
	virtual bool Action(const float * in, float * out, unsigned int len)=0; //Define como act�a el efecto con un sample de muestras
	string popInputErrorMsg();
	virtual ~Effect();
protected:
	vector<string> paramNames;//Los par�metros que tendr� cada efecto.
	vector<string> paramValues;
	string ErrorMsg;
	unsigned int sampleFreq;
};

