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
	virtual vector<string> getParamNames();
	virtual vector<double> getParamValues();
	virtual bool setParam(string paramName, double paramValue);
	virtual bool Action(float * in, float * out, unsigned int len);
	virtual ~Effect();
};

