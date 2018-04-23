#include "Interface.h"

void main(void)
{
	AudioEffects audioEffects;
	bool exit = false;
	Interface interfaceA;
	interfaceA.attachAudioEffects(&audioEffects);
	audioEffects.startPlaying();
	while (exit == false)
		exit = interfaceA.interact();


}