#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "portaudio.h"

using namespace std;

class Effect
{
public:
	Effect();
	vector<string> getParamNames();
	vector<double> getParamValues();
	virtual bool setParam(string paramName, double paramValue)=0;//Setea un parametro del efecto, devuelve false si el valor no es válido (depende da cada parámetro).
	virtual bool Action(float * in, float * out, unsigned int len)=0; //Define como actúa el efecto con un sample de muestras
	string popInputErrorMsg();
	virtual ~Effect();
protected:
	vector<string> paramNames;//Los parámetros que tendrá cada efecto.
	vector<double> paramValues;
	string ErrorMsg;
};

