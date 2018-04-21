#include "Interface.h"

#define FIRST_MENU_MSG "Welcome to Audio Effects. This are the avaiable options"
#define CHANGE_EFFECT "Select new effect"
#define CHANGE_EFFECT_PARAMS "Change current effect settings"
#define EXIT "Exit"
#define CANCEL "Cancel"
#define FIRST_MENU_OPTIONS {CHANGE_EFFECT, CHANGE_EFFECT_PARAMS, EXIT}




Interface::Interface()
{
	currState = FIRST_MENU;
	optionsToPrint = FIRST_MENU_OPTIONS;
	printMenu();
}

bool Interface::interact()
{
	bool quit = false;
	int ch = getch();	
	if (isValid(ch))
	{
		switch (currState)
		{
		case FIRST_MENU:
			quit = handleFirstMenu(ch - (int)'1'); //Solo se puede salir en el primer menú.
			break;
		case CHOOSE_EFFECT:
			handleChooseEffect(ch - (int)'1');
			break;
		case CHOOSE_PARAM:
			handleChooseParam(ch - (int)'1');
			break;
		case SET_PARAM_VALUE:
			handleSetParamValue(ch - (int)'1');
			break;
		}
	}
	return quit;
}


void Interface::attachAudioEffects(AudioEffects * A)
{
	this->A = A;
}


bool Interface::isValid(int ch)
{
	return (ch - (int)'1') <= optionsToPrint.size();
}

bool Interface::handleFirstMenu(unsigned int optChosen)
{
	bool retVal = false;
	if (strcmp(optionsToPrint[optChosen].c_str(), CHANGE_EFFECT))
	{
		currState = FIRST_MENU;
		optionsToPrint = LIST_OF_EFFECTS;
		optionsToPrint.push_back(CANCEL);
	}
	else if (strcmp(optionsToPrint[optChosen].c_str(), CHANGE_EFFECT_PARAMS))
	{
		currState = FIRST_MENU;
		optionsToPrint = A->getCurrParamNames();
		optionsToPrint.push_back(CANCEL);
	}
	else if (strcmp(optionsToPrint[optChosen].c_str(), EXIT))
		retVal = true;
	if (!retVal)
		printMenu();
	return retVal;

}

void Interface::handleChooseEffect(unsigned int optChoosed)
{
	
}

void Interface::handleChooseParam(unsigned int optChosen)
{
	string aux;
	if (strcmp(optionsToPrint[optChosen].c_str(), CANCEL))
	{
		currState = FIRST_MENU;
		optionsToPrint = FIRST_MENU_OPTIONS;
	}
	else
	{
		currState = SET_PARAM_VALUE;
		aux=optionsToPrint[optChosen];
		optionsToPrint.clear();
		optionsToPrint.push_back(aux);
	}
	printMenu();
}

void Interface::handleSetParamValue(unsigned int optChoosed)
{
}

void Interface::printMenu()
{
	system("cls");
}

Interface::~Interface()
{
}