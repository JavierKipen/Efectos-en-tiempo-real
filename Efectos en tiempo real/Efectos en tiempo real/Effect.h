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
	virtual bool setParam(string paramName, double paramValue)=0;
	virtual bool Action(float * in, float * out, unsigned int len)=0;
	string popInputErrorMsg();
	virtual ~Effect();
protected:
	vector<string> paramNames;
	vector<double> paramValues;
	string ErrorMsg;
};

