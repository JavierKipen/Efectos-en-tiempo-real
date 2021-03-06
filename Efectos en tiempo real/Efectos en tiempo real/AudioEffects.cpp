#include "AudioEffects.h"


#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

AudioEffects::AudioEffects()
{
	currentEffect = new Delay(DEFAULT_SAMPLE_RATE);
	warnBoutNumeric = false;
	allEffects = LIST_OF_EFFECTS;
	initOk = false;
	prevEffectWasRobot = false;
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
void AudioEffects::stopPlaying()
{
	Pa_StopStream(stream);
	while ((err = Pa_IsStreamActive(stream)) != 0);
	delete currentEffect;
}
void AudioEffects::pickNewEffect(string newEffect)
{
	unsigned int sampleRate = DEFAULT_SAMPLE_RATE, framesPerBuffer = DEFAULT_FRAMES_PER_BUFFER;
	Pa_StopStream(stream);
	while ((err = Pa_IsStreamActive(stream)) != 0);
	delete currentEffect;
	if (newEffect == "Distortion")		//Se escoge el nuevo efecto
		currentEffect = new Distortion(DEFAULT_SAMPLE_RATE);
	else if (newEffect == "Delay")
		currentEffect = new Delay(DEFAULT_SAMPLE_RATE);
	else if (newEffect == "Reverb")
		currentEffect = new Reverb(DEFAULT_SAMPLE_RATE);
	else if (newEffect == "Flanger")
		currentEffect = new Flanger(DEFAULT_SAMPLE_RATE);
	else if (newEffect == "Chorus")
		currentEffect = new Chorus(DEFAULT_SAMPLE_RATE);
	else if (newEffect == "Vibrato")
		currentEffect = new Vibrato(DEFAULT_SAMPLE_RATE);
	else if (newEffect == "Phaser")
		currentEffect = new Phaser(DEFAULT_SAMPLE_RATE);
	else if (newEffect == "Robot")
		currentEffect = new Robot(DEFAULT_SAMPLE_RATE, ROBOT_DEFAULT_LEN);
	else if (newEffect == "effect3D")
		currentEffect = new effect3D(DEFAULT_SAMPLE_RATE);
	//Pa_OpenStream(&stream, &inputParameters, &outputParameters, sampleRate, framesPerBuffer,0,audioEffectCallback,this);
	if((newEffect != "Robot" && !prevEffectWasRobot) ||(newEffect == "Robot" && prevEffectWasRobot))
		Pa_StartStream(stream);//Se vuelve a usar el stream
	else 
	{
		Pa_CloseStream(stream);
		if (err == paNoError)
		{
			if (newEffect == "Robot" && !prevEffectWasRobot)
			{
				initPortAudio(DEFAULT_SAMPLE_RATE, ROBOT_DEFAULT_LEN);
				prevEffectWasRobot = true;
			}
			else if (newEffect != "Robot" && prevEffectWasRobot)
			{
				initPortAudio(DEFAULT_SAMPLE_RATE, DEFAULT_FRAMES_PER_BUFFER);
				prevEffectWasRobot = false;
			}
		}
		Pa_StartStream(stream);
	}
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
	Pa_StopStream(stream);
	while ((err = Pa_IsStreamActive(stream)) != 0);
	try {
		if (paramValue.size() != 0) //Evita valores que nunca van a tomar parámetros.
			retVal = currentEffect->setParam(paramName, paramValue);
	}
	catch (std::invalid_argument a)
	{
		warnBoutNumeric = true;
	}
	Pa_StartStream(stream);
	return retVal;
}

bool AudioEffects::Action(const float * in, float * out, unsigned int len)
{
	return currentEffect->Action(in,out,len);
	/*for (unsigned int i = 0; i < len; i++) //Dos veces len por ser estereo.
	{
		*out = 2*(*in++);
		if (*out > 1.0)
			*out = 1.0;
		out++;
		*out = 2 * (*in++);
		if (*out > 1.0)
			*out = 1.0;
		out++;
	}
	return true;*/
}
bool AudioEffects::popNumericalError()
{
	bool aux = warnBoutNumeric;
	warnBoutNumeric = false;
	return aux;
}
vector<string> AudioEffects::getListOfEffects()
{
	return allEffects;
}





AudioEffects::~AudioEffects()
{
	Pa_Terminate();
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
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	audioEffects->Action(in, out, framesPerBuffer);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();
	return paContinue;
}
