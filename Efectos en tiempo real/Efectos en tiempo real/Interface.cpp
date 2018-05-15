#include "Interface.h"

#define FIRST_MENU_MSG "Welcome to Audio Effects. This are the avaiable options: \n"
#define CHOOSE_EFFECT_MSG "Choose Between the following effects: \n"
#define CHOOSE_PARAMETER_MSG "Choose a parameter to modify: \n"
#define SET_PARAM_VALUE_MSG "Enter a new value for the parameter chosen: \n"

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
{/*En cada pantalla el usuario eligirá con un número una acción a utilizar, salvo cuando fija un valor nuevo de un parámetro
 que ingresa el número que quiera.*/
	bool quit = false;
	if (currState != SET_PARAM_VALUE) //Salvo para el estado set param value, se tomara solo 1 caracter para indicar 1 opcion
	{
		int ch = _getch(); //Se obtiene el caracter seleccionado
		if (isValid(ch)) 
		{
			switch (currState) //Dependiendo en el estado de la interface se ejecutaran distintas acciones
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
			}
		}
	}
	else
	{
		string x; //En este estado solo se tomarán valores para establecer parámetros.
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
	return (ch - (int)'0') <= optionsToPrint.size() && (ch - (int)'1')>=0; //El número que puso el usuario tiene que ser una de las opciones que figura en pantalla
}

bool Interface::handleFirstMenu(unsigned int optChosen)
{
	bool retVal = false;
	if (optionsToPrint[optChosen]==CHANGE_EFFECT) 
	{/*Si se escoge cambiar el efecto actual, se cambia de estado al mismo y se obtienen los efectos posibles.*/
		currState = CHOOSE_EFFECT;
		optionsToPrint = A->getListOfEffects();
		optionsToPrint.push_back(CANCEL);
	}
	else if (optionsToPrint[optChosen]==CHANGE_EFFECT_PARAMS)
	{/*Si se escoge cambiar un parámetro del efecto actual, se cambia de estado a la FSM y se obtienen los parametros con sus valores actuales*/
		currState = CHOOSE_PARAM;
		optionsToPrint = A->getCurrParamNames();
		optionsToPrint.push_back(CANCEL);
		valuesOfOptions = A->getCurrParamValues();
	}
	else if (optionsToPrint[optChosen] == EXIT)
	{
		retVal = true;
		A->stopPlaying(); //En el caso de salir se le pide a los efectos de audio que corten el stream
	}
	if (!retVal)
		printMenu(); //En caso de que no se haya escogido cerrar el programa, se limpiará la consola.
	return retVal;

}

void Interface::handleChooseEffect(unsigned int optChosen)
{
	if (optionsToPrint[optChosen] != CANCEL) //En el caso que se haya escogido un efecto
		A->pickNewEffect(optionsToPrint[optChosen]);
	currState = FIRST_MENU; //Siempre se volverá al primer menú
	optionsToPrint = FIRST_MENU_OPTIONS;
	printMenu(); 
}

void Interface::handleChooseParam(unsigned int optChosen)
{
	string aux, aux2;
	if (optionsToPrint[optChosen]==CANCEL)
	{//Si se tocó cancelar, se vuelve al menú principal.
		currState = FIRST_MENU;
		optionsToPrint = FIRST_MENU_OPTIONS;
		valuesOfOptions.clear(); //Se borran los valores de opciones, ya que estos solo viven con los parametros de los efectos
	}
	else
	{/*Al escogerse un parámetro se guarda como opcion el mismo y su valor, así en la siguiente pantalla se ven.*/
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

void Interface::handleSetParamValue(string newValue)
{
	if (A->setParam(optionsToPrint[0], newValue)) //En el caso que se ingresó un valor no erroneo
	{
		currState = CHOOSE_PARAM; //Se vuelve a la pantalla de escoger un parámetro
		optionsToPrint = A->getCurrParamNames();
		optionsToPrint.push_back(CANCEL);
		valuesOfOptions = A->getCurrParamValues();
	}
	else
		valueErrorMsg = A->popErrorMsg(); //Si fue erroneo, se obtiene el mensaje que indica como debe ser el input.
	printMenu();
}

void Interface::printMenu()
{
	system("cls"); //Limpia la consola
	string aux;

	cout << stateTitle[(unsigned int)currState]<< endl; //Se imprime el título de cada estado

	for (unsigned int i = 0; i < optionsToPrint.size(); i++)
	{/*Se imprimen todas las opciones, junto con sus valores si es que existen.*/
		aux = valuesOfOptions.size()>i ? to_string(i+1) + "-" + optionsToPrint[i] + "\t\t" + valuesOfOptions[i] : to_string(i + 1) + "-" + optionsToPrint[i];
		cout << aux << endl;
	}
	if (valueErrorMsg.size() != 0)
	{ //En el caso de que haya un mensaje de error se escribe al final.
		cout << valueErrorMsg << endl;
		valueErrorMsg.clear();
	}
	else if (A!=NULL && A->popNumericalError())
		cout << "The input to this parameter must be numerical! " << endl;
}

Interface::~Interface()
{
}