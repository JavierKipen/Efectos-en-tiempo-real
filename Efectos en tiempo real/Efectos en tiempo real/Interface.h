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
	bool isValid(int ch); //Validaci�n de los eventos
	bool handleFirstMenu(unsigned int optChosen); //Maneja los eventos v�lidos en el estado First Menu
	void handleChooseEffect(unsigned int optChosen);//Maneja los eventos v�lidos en el estado Choose Effect
	void handleChooseParam(unsigned int optChosen);//Maneja los eventos v�lidos en el estado Choose Param
	void handleSetParamValue(string newValue);//Maneja los eventos v�lidos en el estado Param Value
	void printMenu(); //Para cualquier estado, ser� la funci�n que imprimir� en pantalla lo correspondiente.

	AudioEffects * A;
	INTERFACE_STATE currState;
	vector<string> optionsToPrint; //Cada estado tendr� opciones de acci�n en cada estado
	vector<string> valuesOfOptions; //Esas opciones pueden tener valores, en el caso que sean par�metros de efectos
	vector<string> stateTitle; //T�tulos predefinidos para cada estado;
	string valueErrorMsg; //Mensaje de error al introducir un valor err�neo para un par�metro
};

