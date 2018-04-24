#include "Effect.h"

Effect::Effect()
{
}

Effect::Effect(unsigned int freqSample)
{
	this->sampleFreq = freqSample;
}

vector<string> Effect::getParamNames()
{
	return paramNames;
}

vector<string> Effect::getParamValues()
{
	return paramValues;
}

string Effect::popInputErrorMsg()
{
	string aux = ErrorMsg;
	ErrorMsg.clear();
	return aux;
}

Effect::~Effect()
{
}
