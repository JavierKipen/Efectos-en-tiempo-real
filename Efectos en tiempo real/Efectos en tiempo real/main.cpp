#include "Interface.h"
#include <time.h>  

void main(void)
{
	srand(time(NULL));
	AudioEffects audioEffects; //Este objeto se encargar� de inicializar a los efectos, e intermediar� entre la interfaz y los �ltimos para cambiar sus par�metros
	bool exit = false;
	Interface interfaceA;
	interfaceA.attachAudioEffects(&audioEffects);
	if (audioEffects.getInitOk() == true && audioEffects.startPlaying()) //Si se inicializaron bien las cosas de port audio
	{
		while (exit == false)
			exit = interfaceA.interact(); //Hasta que se decida terminar el programa se estar� en este loop
	}
	


}