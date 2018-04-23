#include "Interface.h"

#define FIRST_MENU_MSG "Welcome to Audio Effects. This are the avaiable options:"
#define CHOOSE_EFFECT_MSG "Choose Between the following effects:"
#define CHOOSE_PARAMETER_MSG "Welcome to Audio Effects. This are the avaiable options:"
#define SET_PARAM_VALUE_MSG "Welcome to Audio Effects. This are the avaiable options:"

#define CHANGE_EFFECT "Select new effect"
#define CHANGE_EFFECT_PARAMS "Change current effect settings"
#define EXIT "Exit"
#define CANCEL "Cancel"
#define FIRST_MENU_OPTIONS {CHANGE_EFFECT, CHANGE_EFFECT_PARAMS, EXIT}





Interface::Interface()
{
	currState = FIRST_MENU;
	optionsToPrint = FIRST_MENU_OPTIONS;
	stateTitle = { FIRST_MENU_MSG, CHOOSE_EFFECT_MSG , CHOOSE_PARAMETER_MSG, SET_PARAM_VALUE_MSG };
	printMenu();
	
}

bool Interface::interact()
{
	bool quit = false;
	if (currState != SET_PARAM_VALUE) //Salvo para el estado set param value, se tomara solo 1 caracter para indicar 1 opcion
	{
		int ch = _getch();
		if (isValid(ch))
		{
			switch (currState) //Dependiendo en el estado de la interface se ejecutaran distintas acciones
			{
			case FIRST_MENU:
				quit = handleFirstMenu(ch - (int)'1'); //Solo se puede salir en el primer men�.
				break;
			case CHOOSE_EFFECT:
				handleChooseEffect(ch - (int)'1'); 
				break;
			case CHOOSE_PARAM:
				handleChooseParam(ch - (int)'1');
				break;
			}
		}
	}
	else
	{
		double x; //En este estado solo se tomar�n valores para establecer par�metros.
		cin >> x;
		handleSetParamValue(x);
	}
	return quit;
}


void Interface::attachAudioEffects(AudioEffects * A)
{
	this->A = A;
}


bool Interface::isValid(int ch)
{
	return (ch - (int)'0') <= optionsToPrint.size() && (ch - (int)'1')>=0;
}

bool Interface::handleFirstMenu(unsigned int optChosen)
{
	bool retVal = false;
	if (optionsToPrint[optChosen]==CHANGE_EFFECT)
	{
		currState = CHOOSE_EFFECT;
		optionsToPrint = A->getListOfEffects();
		optionsToPrint.push_back(CANCEL);
	}
	else if (optionsToPrint[optChosen]==CHANGE_EFFECT_PARAMS)
	{
		currState = CHOOSE_PARAM;
		optionsToPrint = A->getCurrParamNames();
		optionsToPrint.push_back(CANCEL);
		valuesOfOptions = A->getCurrParamValues();
	}
	else if (optionsToPrint[optChosen]==EXIT)
		retVal = true;
	if (!retVal)
		printMenu();
	return retVal;

}

void Interface::handleChooseEffect(unsigned int optChosen)
{
	if (optionsToPrint[optChosen] != CANCEL) //En el caso que se haya escogido un efecto
		A->pickNewEffect(optionsToPrint[optChosen]);
	currState = FIRST_MENU; //Siempre se volver� al primer men�
	optionsToPrint = FIRST_MENU_OPTIONS;
	printMenu();
	
}

void Interface::handleChooseParam(unsigned int optChosen)
{
	string aux;
	double aux2;
	if (optionsToPrint[optChosen]==CANCEL)
	{
		currState = FIRST_MENU;
		optionsToPrint = FIRST_MENU_OPTIONS;
		valuesOfOptions.clear();
	}
	else
	{
		currState = SET_PARAM_VALUE;
		aux=optionsToPrint[optChosen];
		aux2= valuesOfOptions[optChosen];
		optionsToPrint.clear();
		optionsToPrint.push_back(aux);
		valuesOfOptions.clear();
		valuesOfOptions.push_back(aux2);
	}
	printMenu();
}

void Interface::handleSetParamValue(double newValue)
{
	if (A->setParam(optionsToPrint[0], newValue)) //En el caso que se ingres� un valor no erroneo
	{
		currState = CHOOSE_PARAM;
		optionsToPrint = A->getCurrParamNames();
		optionsToPrint.push_back(CANCEL);
		valuesOfOptions = A->getCurrParamValues();
	}
	else
		valueErrorMsg = A->popErrorMsg();
	printMenu();
}

void Interface::printMenu()
{
	system("cls");
	string aux;
	cout << stateTitle[(unsigned int)currState]<< endl;
	for (unsigned int i = 0; i < optionsToPrint.size(); i++)
	{
		aux = valuesOfOptions.size()>i ? to_string(i+1) + "-" + optionsToPrint[i] + "\t\t" + to_string(valuesOfOptions[i]) : to_string(i + 1) + "-" + optionsToPrint[i];
		cout << aux << endl;
	}
	if (valueErrorMsg.size() != 0)
	{
		cout << valueErrorMsg << endl;
		valueErrorMsg.clear();
	}
}

Interface::~Interface()
{
}