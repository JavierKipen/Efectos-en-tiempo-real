#include "Effect.h"



Effect::Effect()
{
}

vector<string> Effect::getParamNames()
{
	return paramNames;
}

vector<double> Effect::getParamValues()
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
