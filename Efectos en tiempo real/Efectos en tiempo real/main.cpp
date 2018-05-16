#include "Interface.h"
#include "WavEffectAdder.h"
#include <time.h>  

int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));
	if (argc == 1) //En el caso que se llame al programa sin argumentos, se corren los efectos en tiempo real.
	{
		AudioEffects audioEffects; //Este objeto se encargará de inicializar a los efectos, e intermediará entre la interfaz y los últimos para cambiar sus parámetros
		bool exit = false;
		Interface interfaceA;
		interfaceA.attachAudioEffects(&audioEffects);
		if (audioEffects.getInitOk() == true && audioEffects.startPlaying()) //Si se inicializaron bien las cosas de port audio
		{
			while (exit == false)
				exit = interfaceA.interact(); //Hasta que se decida terminar el programa se estará en este loop
		}
	}
	else
	{
		WavEffectAdder WavManager(argc, argv);
		if (WavManager.initOk())
			WavManager.applyEffects();
	}
	
	

	return 0;
}