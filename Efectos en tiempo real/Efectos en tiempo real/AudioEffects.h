#pragma once
#include "Effect.h"
#include "Fuzz.h"
#include "Delay.h"
#include "Reverb.h"
#include "Flanger.h"
#include "Chorus.h"
#include "Vibrato.h"

#define LIST_OF_EFFECTS {"Fuzz", "Delay", "Reverb", "Flanger", "Chorus","Vibrato"}

#define DEFAULT_SAMPLE_RATE         (44100)
#define PA_SAMPLE_TYPE      paFloat32
#define DEFAULT_FRAMES_PER_BUFFER   (64)

class AudioEffects
{
public:
	AudioEffects();
	bool startPlaying();
	void stopPlaying(); //Termina el stream.
	vector<string> getCurrParamNames(); //Devuelve los nombres de los parámetros del efecto actual.
	vector<string> getCurrParamValues();//Devuelve los valores de los parámetros del efecto actual.
	bool setParam(string paramName, string paramValue); //Setea un parametro del efecto actual, devuelve false si el valor no es válido (depende da cada parámetro).
	bool Action(const float * in, float * out, unsigned int len); //Hace actuar al efecto con las muestras obtenidas
	vector<string> getListOfEffects(); //Devuelve todos los efectos reconocidos
	void pickNewEffect(string newEffect); //Escoge un nuevo efecto actual. El string que se pasa como argumento tiene que ser alguno de los que devuelve getlistofeffects
	string popErrorMsg();
	bool getInitOk();
	~AudioEffects();
private:
	void initPortAudio(unsigned int sampleRate, unsigned int framesPerBuffer);
	bool initOk;
	Effect * currentEffect;
	vector<string> allEffects;
	PaStreamParameters inputParameters, outputParameters;
	PaStream *stream;
	PaError err;
};

int audioEffectCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData);
