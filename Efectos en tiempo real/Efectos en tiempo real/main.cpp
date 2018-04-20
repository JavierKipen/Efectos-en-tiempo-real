#include "Interface.h"

void main(void)
{
	AudioEffects audioEffects;
	bool exit = false;
	Interface interface;
	interface.attachAudioEffects(&audioEffects);
	audioEffects.startPlaying();
	while (exit == false)
		exit = interface.interact();


}