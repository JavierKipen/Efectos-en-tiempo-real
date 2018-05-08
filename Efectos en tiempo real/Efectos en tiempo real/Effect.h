#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "portaudio.h"

#define GEN_MOD(x,y) ((x)-(floor(((x)/(y)))*y))
#define TRI_WAVE(x,y) (GEN_MOD((x),(y))/(y))

using namespace std;

class Effect
{
public:
	Effect();
	Effect(unsigned int sampleFreq);
	vector<string> getParamNames();
	vector<string> getParamValues();
	virtual bool setParam(string paramName, string paramValue)=0;//Setea un parametro del efecto, devuelve false si el valor no es válido (depende da cada parámetro).
	virtual bool Action(const float * in, float * out, unsigned int len)=0; //Define como actúa el efecto con un sample de muestras
	string popInputErrorMsg();
	virtual ~Effect();
protected:
	vector<string> paramNames;//Los parámetros que tendrá cada efecto.
	vector<string> paramValues;
	string ErrorMsg;
	unsigned int sampleFreq;
};

