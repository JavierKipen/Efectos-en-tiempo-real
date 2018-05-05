#include "Chorus.h"

#define CHORUS_MODEL_INDEX 0
#define CHORUS_DELAY_T_INDEX 1
#define CHORUS_DEPTH_INDEX 2
#define CHORUS_LFO_FREQ_INDEX 3
#define CHORUS_LFO_TYPE_INDEX 4

Chorus::Chorus(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = CHORUS_PARAMETERS;
	paramValues = CHORUS_DEFAULT_PARAM_VALUES;
	unsigned int maxSamplesNeeded = (unsigned int)(CHORUS_MAX_DELAY_T *2 * (float)(sampleFreq)+1); //*2 por que con un depth de 1 tiene un desvío de max delay, para max delay.
	memoryL.resize(maxSamplesNeeded, 0);
	memoryR.resize(maxSamplesNeeded, 0);
	//LP_prevX.resize(CHORUS_MAX_LP_ORDER, 0);
	//LP_prevY.resize(CHORUS_MAX_LP_ORDER, 0);
	counter = 0;
	sampleCount = 0;
	saveValues();
	
}

bool Chorus::Action(const float * in, float * out, unsigned int len)
{
	unsigned int randomIndex;
	float aux;
	for (unsigned int i = 0; i < len; i++) //Dos veces len por ser estereo.
	{
		//randomIndex = getRandomIndex();
		/**out++ = bl * (*in) + (bl * fb + ff) * memoryL[randomIndex];
		memoryL[counter] = (*in++) + fb * memoryL[randomIndex];
		*out++ = bl * (*in) + (bl * fb + ff) * memoryR[randomIndex];
		memoryR[counter] = (*in++) + fb * memoryR[randomIndex];*/

		/**out++ = (*in) + fb * memoryL[randomIndex];
		memoryL[counter] = (*in++);
		*out++ =  (*in) + fb * memoryR[randomIndex];
		memoryR[counter] = (*in++) ;
		counter = (++counter) % maxTaps;*/
		/*aux = getRandomPrev(memoryL);
		*out++ = (bl * (*in)) + ((bl * fb + ff) * aux);
		memoryL[counter] = (*in++) + fb * aux;*/
		/*aux = getRandomPrev(memoryR);
		*out++ = bl * (*in) + (bl * fb + ff) * aux;
		memoryR[counter] = (*in++) + fb * aux;*/
		calculateEcho();
		if (paramValues[CHORUS_MODEL_INDEX] == "Basic")
		{
			*out++ = bl*((*in) + fb * echoL);
			memoryL[counter] = (*in++);
			*out++ = bl*((*in) + fb * echoR);
			memoryR[counter] = (*in++);
		}
		else if (paramValues[CHORUS_MODEL_INDEX] == "Full")
		{
			*out++ = (bl * (*in)) + ((bl * fb + ff) * echoL);
			memoryL[counter] = (*in++) + fb * echoL;
			*out++ = bl * (*in) + (bl * fb + ff) * echoR;
			memoryR[counter] = (*in++) + fb * echoR;
		}
		counter = (++counter) % maxTaps;
		sampleCount++;
	}

	return true;
}

bool Chorus::setParam(string paramName, string paramValue)
{
	bool retVal=false;
	
	if (paramName == "Model Type")
	{
		if(paramValue == "Basic" || paramValue=="Full")
		{
			paramValues[CHORUS_MODEL_INDEX] = paramValue; 
			retVal = true;
		}
		else
			ErrorMsg = "WIP";
	}
	else if (paramName == "LFO Type")
	{
		if (paramValue == "Sine" )
		{
			paramValues[CHORUS_LFO_TYPE_INDEX] = paramValue;
			retVal = true;
		}
		else
			ErrorMsg = "WIP";
	}
	else {
		float paramValuef = stof(paramValue);
		if (paramName == "Delay Time")
		{
			if (paramValuef >= CHORUS_MIN_DELAY_T && paramValuef <= CHORUS_MAX_DELAY_T)
			{
				paramValues[CHORUS_DELAY_T_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = "WIP";
		}
		else if (paramName == "Depth Factor")
		{
			if (paramValuef > 0 && paramValuef < 1)
			{
				paramValues[CHORUS_DEPTH_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = "WIP";
		}
		else if (paramName == "LFO Freq")
		{
			if (paramValuef >= CHORUS_MIN_LFO_FREQ && paramValuef <= CHORUS_MAX_LFO_FREQ)
			{
				paramValues[CHORUS_LFO_FREQ_INDEX] = paramValue; retVal = true;
			}
			else
				ErrorMsg = "WIP";
		}
	}
	
	if (retVal == true)
		saveValues();
	return retVal;
}

Chorus::~Chorus()
{
}
void Chorus::calculateEcho()
{
	float sampleIndex;
	if(paramValues[CHORUS_LFO_TYPE_INDEX] == "Sine")
		sampleIndex = sin(2 * 3.14159265 * lfoFreq * (float)sampleCount / sampleFreq)  * (float)maxDevOfTaps + maxDevOfTaps; //Calculo la desviación que tendrá para el sample dado
	sampleIndex = counter + sampleIndex;
	if(sampleIndex > maxTaps)
		sampleIndex = sampleIndex - maxTaps;
	unsigned int indexOfolderSample = floor(sampleIndex);
	unsigned int  indexOfnewerSample = floor(sampleIndex)+1;
	if (indexOfnewerSample == maxTaps)
		indexOfnewerSample = 0;
	float frac = sampleIndex - floor(sampleIndex);//Fracción del error
	echoL = memoryL[indexOfnewerSample] * frac + memoryL[indexOfolderSample] * (1 - frac);
	echoR = memoryR[indexOfnewerSample] * frac + memoryR[indexOfolderSample] * (1 - frac);
}/*
float Chorus::getRandomPrev(vector<float> &mem)
{
	float retVal;
	unsigned int nextMem;
	float aux = (((float)rand() / (float)RAND_MAX)*(2 * (float)maxDevOfTaps )) - (float)maxDevOfTaps; //Distribución uniforme  con centro en donde se está contando y con posible desviación de hasta maxdev para arriba y para abajo.
	aux = filterNoise(aux);
	aux = aux + maxDevOfTaps;
	float newDelay = ((float)counter + aux);
	if (newDelay > maxTaps)
		newDelay = newDelay - maxTaps;
	if (floor(newDelay) + 1 == nominalTaps)
		nextMem = 0;
	else
		nextMem = floor(newDelay) + 1;
	float frac = newDelay - floor(newDelay);
	retVal = mem[nextMem] * frac + mem[floor(newDelay)] * (1 - frac);
	return retVal;
}*/
/*
unsigned int Chorus::getRandomIndex()
{
	unsigned int retVal;
	float aux = (((float)rand() / (float)RAND_MAX)*(2 * (float)maxDevOfTaps )); //Distribución uniforme  con centro en donde se está contando y con posible desviación de hasta maxdev para arriba y para abajo.
	aux = filterNoise(aux) + maxDevOfTaps;
	int dev = (int)((float)counter + aux);
	retVal = dev % maxTaps;
	
	return retVal;
}*/
/*
float Chorus::filterNoise(float random)
{
	/*float retVal = totGain * (random + LP_PZRatio * LP_prevX[0] + LP_PZRatio * LP_prevY[0]); //Por ahora orden 1.
	LP_prevX[0] = random;
	LP_prevY[0] = retVal; //Este es orden 1.
	float retVal = totGain * (random + 2 * LP_PZRatio * LP_prevX[0] + LP_PZRatio * LP_PZRatio * LP_prevX[1] + 2 * LP_PZRatio * LP_prevY[0]- LP_PZRatio * LP_PZRatio * LP_prevY[1]); //Por ahora orden 1.
	LP_prevX[1] = LP_prevX[0];
	LP_prevX[0] = random;
	LP_prevY[1] = LP_prevY[0];
	LP_prevY[0] = retVal;
	return retVal;
}*/

void Chorus::saveValues()
{
	float aux = stof(paramValues[CHORUS_DELAY_T_INDEX])/1000.0; //Porque estaba en milisegundos.
	nominalTaps = (unsigned int)(aux * (float)(sampleFreq));
	maxDevOfTaps = (unsigned int)(aux * stof(paramValues[CHORUS_DEPTH_INDEX]) * (float)(sampleFreq));
	if (maxDevOfTaps == 0)
		maxDevOfTaps = 1;
	maxTaps = nominalTaps + maxDevOfTaps;
	lfoFreq = stof(CHORUS_DEFAULT_LFO_FREQ);
	if (paramValues[CHORUS_MODEL_INDEX] == "Basic")
	{
		bl = 0.5;
		fb = 1;
		ff = 1;
	}
	else
	{
		bl = 0.5;
		fb = 1;
	}
	//LP_PZRatio = stof(paramValues[CHORUS_RATIO_INDEX]);
	//totGain = (1 - LP_PZRatio) / (1 + LP_PZRatio); Orden 1
	//totGain = pow(((1 - LP_PZRatio) / (1 + LP_PZRatio)),2); //Orden 2
	
}
