#include "effect3D.h"


effect3D::effect3D(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = EFFECT3D_PARAMETERS;
	paramValues.resize(paramNames.size(), "0");
	paramValues[0] = EFFECT3D_ANGLE_DEFAULT;
	paramValues[1] = EFFECT3D_ELEVATION_DEFAULT;

	angle = stoi(paramValues[0]) % 360;
	elevation = stoi(paramValues[1]) % 360;
	lastOutL = 0;
	lastOutR = 0;

	
}


bool effect3D::setParam(string paramName, string paramValue)
{
	bool retVal = true;

	if (paramName == "ANGLE")	// [1:20] ms
	{
		angle = stoi(paramValue) % 360;
		paramValues[0] = paramValue;
	}
	else if (paramName == "ELEVATION") // [1:20] ms
	{
		elevation = stoi(paramValue) % 360;
		paramValues[1] = paramValue;
	}
	return retVal;

}

bool effect3D::Action(const float * in, float * out, unsigned int len)
{
	vector<float> leftInput = vector<float>(len, 0);;
	vector<float> rightInput = vector<float>(len, 0);;

	vector<float> leftOutput = vector<float>(len, 0);
	vector<float> rightOutput = vector<float>(len, 0);
	
	for (int i = 0; i < len; i++)
	{
		leftInput[i] = *in++;
		rightInput[i] = *in++;
	}

	hsfilter(&leftInput[0], &leftOutput[0], len, angle, 'L');
	hsfilter(&rightInput[0], &rightOutput[0], len, 180-angle, 'R');

	vector<float>::iterator itL = leftOutput.begin();
	vector<float>::iterator itR = rightOutput.begin();

	for (int i = 0; i <  len; i++)
	{
		*out++ = *itL++;
		*out++ = *itR++;
	}
	return true;
}

void effect3D::hsfilter( const float const *  input, float * out, unsigned int len, float theta, char channel)
{
	const float * in = input;
	float Fs = sampleFreq;
	float thetaO = 150;
	float c = 334.0;
	float a = 0.08;
	float wo = c / a;
	float alphamin = 0.05;
	float alpha = 1 + alphamin + (1 - alphamin)*cosf(theta * PI / thetaO);
	float gDelay = 0;

	float * lastIn;
	float * lastTempOut;
	float * lastOut;

	if (channel == 'L')
	{
		lastIn = &lastInL;
		lastTempOut = &lastTempOutL;
		lastOut = &lastOutL;
	}
	else
	{
		lastIn = &lastInR;
		lastTempOut = &lastTempOutR;
		lastOut = &lastOutR;

	}

	vector<float> buffer = vector<float>(len, 0);
	vector<float>::iterator tempIt = buffer.begin();

	*tempIt++ = (*in++*(wo + alpha * Fs) + *lastIn * (wo - alpha * Fs) - *lastTempOut*(wo - Fs)) / (wo + Fs);
//*tempIt++ = *in++;
	for (unsigned long i = 0; i < len - 1; i++, tempIt++, in++)
		*tempIt = ( *in*(wo + alpha * Fs) + *(in - 1)*(wo - alpha * Fs) - *(tempIt - 1)*(wo - Fs) )/ (wo + Fs);
//*tempIt = *in;
	*lastIn = *--in;

	/*------- IDT ---------------*/
	if (abs(theta) < 90)
		gDelay = -(Fs / wo)*(cos(theta*PI / 180) - 1);
	else
		gDelay = (Fs / wo)* ((abs(theta) - 90)*PI / 180 + 1);

	float gCoef = (1 - gDelay) / (1 + gDelay);

	tempIt = buffer.begin();
	*out++ = *tempIt++ * gCoef + *lastTempOut - *lastOut * gCoef;
//*out++ = *tempIt++;

	for (unsigned long i = 0; i < len - 1; i++, out++, tempIt++)
		*out = *tempIt * gCoef + *(tempIt - 1) - *(out - 1) * gCoef;
//*out = *tempIt;

	*lastTempOut = *--tempIt;
	*lastOut = *--out;
}
effect3D::~effect3D()
{
}
