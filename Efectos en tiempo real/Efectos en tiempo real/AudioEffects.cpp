#include "AudioEffects.h"



AudioEffects::AudioEffects()
{
	currentEffect = new Delay(DEFAULT_SAMPLE_RATE);
	allEffects = LIST_OF_EFFECTS;
	initOk = false;
	err = Pa_Initialize();
	if (err == paNoError)
		initPortAudio(DEFAULT_SAMPLE_RATE,DEFAULT_FRAMES_PER_BUFFER);
}
void AudioEffects::initPortAudio(unsigned int sampleRate, unsigned int framesPerBuffer)
{
	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	if (inputParameters.device != paNoDevice)
	{
		inputParameters.channelCount = 2;       /* stereo input */
		inputParameters.sampleFormat = PA_SAMPLE_TYPE;
		inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
		inputParameters.hostApiSpecificStreamInfo = NULL;
		outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
		if (outputParameters.device != paNoDevice)
		{
			outputParameters.channelCount = 2;       /* stereo output */
			outputParameters.sampleFormat = PA_SAMPLE_TYPE;
			outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
			outputParameters.hostApiSpecificStreamInfo = NULL;
			err = Pa_OpenStream(&stream, &inputParameters,&outputParameters, sampleRate, framesPerBuffer,
				0, /* paClipOff, */  /* we won't output out of range samples so don't bother clipping them */
				audioEffectCallback,
				this);
			if (err == paNoError)
				initOk = true;
		}
		else
			fprintf(stderr, "Error: No default output device.\n");
	}
	else
		fprintf(stderr, "Error: No default input device.\n");
}


bool AudioEffects::startPlaying()
{
	err = Pa_StartStream(stream);
	return err == paNoError;
}
void AudioEffects::pickNewEffect(string newEffect)
{
	delete currentEffect;
	//Pa_CloseStream(stream);
	if (newEffect == "Fuzz")
		currentEffect = new Fuzz(DEFAULT_SAMPLE_RATE);
	else if (newEffect == "Delay")
		currentEffect = new Delay(DEFAULT_SAMPLE_RATE);
	//Pa_StartStream(stream);
}

string AudioEffects::popErrorMsg()
{
	return currentEffect->popInputErrorMsg();
}

bool AudioEffects::getInitOk()
{
	return initOk;
}

vector<string> AudioEffects::getCurrParamNames()
{
	return currentEffect->getParamNames();
}

vector<string> AudioEffects::getCurrParamValues()
{
	return currentEffect->getParamValues();
}

bool AudioEffects::setParam(string paramName, string paramValue)
{
	bool retVal=false;
	if (paramValue.size()!=0) //Evita valores que nunca van a tomar parámetros.
		retVal = currentEffect->setParam(paramName, paramValue);
	return retVal;
}

bool AudioEffects::Action(const float * in, float * out, unsigned int len)
{
	return currentEffect->Action(in,out,len);
}

vector<string> AudioEffects::getListOfEffects()
{
	return allEffects;
}





AudioEffects::~AudioEffects()
{
}


int audioEffectCallback(const void * inputBuffer, void * outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo * timeInfo, PaStreamCallbackFlags statusFlags, void * userData)
{
	float * out = (float *)outputBuffer;
	const float * in = (const float *)inputBuffer;
	unsigned int i;
	(void)timeInfo; /* Prevent unused variable warnings. */
	(void)statusFlags;
	(void)userData;
	AudioEffects *audioEffects = (AudioEffects *)userData;
	audioEffects->Action(in, out, framesPerBuffer);

	return paContinue;
}
