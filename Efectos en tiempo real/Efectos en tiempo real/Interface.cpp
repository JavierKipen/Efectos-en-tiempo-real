#include "Interface.h"



Interface::Interface()
{
}

bool Interface::interact()
{
	printMenu();
	return false;
}


void Interface::attachAudioEffects(AudioEffects * A)
{
	this->A = A;
}


void Interface::printMenu()
{
}

Interface::~Interface()
{
}