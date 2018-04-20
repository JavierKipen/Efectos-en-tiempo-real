#pragma once
#include "AudioEffects.h"
class Interface
{
public:
	Interface();
	bool interact(); //Interacts with the user, changing the effects and exiting.
	void attachAudioEffects(AudioEffects *A);
	~Interface();
private:
	void printMenu();
	AudioEffects * A;
};

