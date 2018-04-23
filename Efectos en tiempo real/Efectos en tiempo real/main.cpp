#include "Interface.h"

void main(void)
{
	AudioEffects audioEffects; //Este objeto se encargará de inicializar a los efectos, e intermediará entre la interfaz y los últimos para cambiar sus parámetros
	bool exit = false;
	Interface interfaceA;
	interfaceA.attachAudioEffects(&audioEffects);
	audioEffects.startPlaying();
	while (exit == false)
		exit = interfaceA.interact(); //Hasta que se decida terminar el programa se estará en este loop


}