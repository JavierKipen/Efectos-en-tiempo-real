#pragma once
#include "AudioEffects.h"
#include <list>
#include <conio.h>
#include <Windows.h>

typedef  enum { FIRST_MENU, CHOOSE_EFFECT, CHOOSE_PARAM, SET_PARAM_VALUE } INTERFACE_STATE;
class Interface
{
public:
	Interface();
	bool interact(); //Loop principal.
	void attachAudioEffects(AudioEffects *A);
	
	~Interface();
private:
	bool isValid(int ch); //Validación de los eventos
	bool handleFirstMenu(unsigned int optChosen); //Maneja los eventos válidos en el estado First Menu
	void handleChooseEffect(unsigned int optChosen);//Maneja los eventos válidos en el estado Choose Effect
	void handleChooseParam(unsigned int optChosen);//Maneja los eventos válidos en el estado Choose Param
	void handleSetParamValue(string newValue);//Maneja los eventos válidos en el estado Param Value
	void printMenu(); //Para cualquier estado, será la función que imprimirá en pantalla lo correspondiente.

	AudioEffects * A;
	INTERFACE_STATE currState;
	vector<string> optionsToPrint; //Cada estado tendrá opciones de acción en cada estado
	vector<string> valuesOfOptions; //Esas opciones pueden tener valores, en el caso que sean parámetros de efectos
	vector<string> stateTitle; //Títulos predefinidos para cada estado;
	string valueErrorMsg; //Mensaje de error al introducir un valor erróneo para un parámetro
};

