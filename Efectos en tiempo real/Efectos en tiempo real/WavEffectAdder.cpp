#include "WavEffectAdder.h"

#define DEFAULT_SAMPLE_RATE 44100

WavEffectAdder::WavEffectAdder(int argc, char *argv[])
{
	initOkay = false;
	currEffect = NULL;
	if (argc == 3) //En el caso que se haya escrito el programa, la localización del archivo de audio y el efecto a poner se sigue.
	{
		sfInfoR.format = 0;sfInfoR.samplerate = DEFAULT_SAMPLE_RATE;sfInfoR.channels = 2;
		sfInfoW.format = 0; sfInfoW.samplerate = DEFAULT_SAMPLE_RATE; sfInfoW.channels = 2;
		p2ReadFile=sf_open(argv[1], SFM_READ, &sfInfoR);
		p2WriteFile = sf_open(("Output/" + (string)argv[1]).c_str(), SFM_WRITE, &sfInfoR);
		if (p2ReadFile != NULL && p2WriteFile != NULL && chooseEffect((string)argv[2]))
		{
			initOkay = true;
		}
	}
}

bool WavEffectAdder::initOk()
{
	return initOkay;
}

void WavEffectAdder::applyEffects()
{
	sf_count_t count, test;
	do {
		count = sf_read_float(p2ReadFile, bufferInput, MAX_INPUT_LEN);
		currEffect->Action(bufferInput, bufferOutput, count/2);
		test=sf_write_float(p2WriteFile, bufferOutput, count);
	} while (count == MAX_INPUT_LEN);
}


WavEffectAdder::~WavEffectAdder()
{
	if(p2WriteFile!=NULL)
		sf_close(p2WriteFile);
	if (p2ReadFile != NULL)
		sf_close(p2ReadFile);
}

bool WavEffectAdder::chooseEffect(string effect)
{
	bool retVal = false;
	if (effect == "Delay")
		currEffect = new Delay(DEFAULT_SAMPLE_RATE);
	if (effect == "Distortion")		//Se escoge el nuevo efecto
		currEffect = new Distortion(DEFAULT_SAMPLE_RATE);
	else if (effect == "Reverb")
		currEffect = new Reverb(DEFAULT_SAMPLE_RATE);
	else if (effect == "Flanger")
		currEffect = new Flanger(DEFAULT_SAMPLE_RATE);
	else if (effect == "Chorus")
		currEffect = new Chorus(DEFAULT_SAMPLE_RATE);
	else if (effect == "Vibrato")
		currEffect = new Vibrato(DEFAULT_SAMPLE_RATE);
	else if (effect == "Phaser")
		currEffect = new Phaser(DEFAULT_SAMPLE_RATE);

	if (currEffect != NULL)
		retVal = true;
	else
		cout << "The efect chosen was wrong, it muss be one of this: " << endl;
	return retVal;
}
