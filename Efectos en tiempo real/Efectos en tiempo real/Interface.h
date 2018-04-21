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
	bool handleFirstMenu(unsigned int optChoosed);
	void handleChooseEffect(unsigned int optChoosed);
	void handleChooseParam(unsigned int optChoosed);
	void handleSetParamValue(unsigned int optChoosed);
	void printMenu();
	AudioEffects * A;
	INTERFACE_STATE currState;
	vector<string> optionsToPrint;
	vector<string> valuesOfOptions;
	vector<string> stateTitle;
};

