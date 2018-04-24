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
	vector<string> getParamValues();
	virtual bool setParam(string paramName, string paramValue)=0;//Setea un parametro del efecto, devuelve false si el valor no es v�lido (depende da cada par�metro).
	virtual bool Action(const float * in, float * out, unsigned int len)=0; //Define como act�a el efecto con un sample de muestras
	string popInputErrorMsg();
	virtual ~Effect();
protected:
	vector<string> paramNames;//Los par�metros que tendr� cada efecto.
	vector<string> paramValues;
	string ErrorMsg;
};

