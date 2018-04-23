#pragma once
#include "Effect.h"
#include "Fuzz.h"

#define LIST_OF_EFFECTS {"Fuzz"}

class AudioEffects
{
public:
	AudioEffects();
	bool startPlaying();
	vector<string> getCurrParamNames(); //Devuelve los nombres de los par�metros del efecto actual.
	vector<double> getCurrParamValues();//Devuelve los valores de los par�metros del efecto actual.
	bool setParam(string paramName, double paramValue); //Setea un parametro del efecto actual, devuelve false si el valor no es v�lido (depende da cada par�metro).
	bool Action(float * in, float * out, unsigned int len); //Hace actuar al efecto con las muestras obtenidas
	vector<string> getListOfEffects(); //Devuelve todos los efectos reconocidos
	void pickNewEffect(string newEffect); //Escoge un nuevo efecto actual. El string que se pasa como argumento tiene que ser alguno de los que devuelve getlistofeffects
	string popErrorMsg();
	~AudioEffects();
private:
	Effect * currentEffect;
	vector<string> allEffects;
};

