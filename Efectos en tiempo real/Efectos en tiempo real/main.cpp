#include "Interface.h"

void main(void)
{
	AudioEffects audioEffects; //Este objeto se encargar� de inicializar a los efectos, e intermediar� entre la interfaz y los �ltimos para cambiar sus par�metros
	bool exit = false;
	Interface interfaceA;
	interfaceA.attachAudioEffects(&audioEffects);
	audioEffects.startPlaying();
	while (exit == false)
		exit = interfaceA.interact(); //Hasta que se decida terminar el programa se estar� en este loop


}