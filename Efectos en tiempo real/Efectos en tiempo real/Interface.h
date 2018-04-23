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
	bool interact(); //Interacts with the user, changing the effects and exiting.
	void attachAudioEffects(AudioEffects *A);
	
	~Interface();
private:
	bool isValid(int ch);
	bool handleFirstMenu(unsigned int optChosen);
	void handleChooseEffect(unsigned int optChosen);
	void handleChooseParam(unsigned int optChosen);
	void handleSetParamValue(double newValue);
	void printMenu();
	AudioEffects * A;
	INTERFACE_STATE currState;
	vector<string> optionsToPrint;
	vector<double> valuesOfOptions;
	vector<string> stateTitle;
	string valueErrorMsg;
};

