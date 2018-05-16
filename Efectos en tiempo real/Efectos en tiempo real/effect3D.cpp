#include "effect3D.h"


effect3D::effect3D(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = EFFECT3D_PARAMETERS;
	paramValues.resize(paramNames.size(), "0");
	paramValues[0] = EFFECT3D_ANGLE_DEFAULT;
	paramValues[1] = EFFECT3D_ELEVATION_DEFAULT;

	angle = (float)(stoi(paramValues[0]) % 360);
	elevation = (float)(stoi(paramValues[1]) % 360);
	lastOutL = 0.0f;
	lastOutR = 0.0f;

	
}


bool effect3D::setParam(string paramName, string paramValue)
{
	bool retVal = true;

	if (paramName == "ANGLE")	// [1:20] ms
	{
		angle = (float)(stoi(paramValue) % 360);
		paramValues[0] = paramValue;
	}
	else if (paramName == "ELEVATION") // [1:20] ms
	{
		elevation = (float)(stoi(paramValue) % 360);
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

	
	for (unsigned int i = 0; i < len; i++)
	{
		leftInput[i] = *in++;
		rightInput[i] = *in++;
	}

	hsfilter(&rightInput[0], &rightOutput[0], len, angle, 'R');
	hsfilter(&leftInput[0], &leftOutput[0], len, 180 - angle, 'L');

	vector<float>::iterator itL = leftOutput.begin();
	vector<float>::iterator itR = rightOutput.begin();

	for (unsigned int i = 0; i <  len; i++)
	{
		*out++ = *itL++;
		*out++ = *itR++;
	}
	return true;
}

void effect3D::hsfilter( const float *  input, float * out, unsigned int len, float theta, char channel)
{
	const float * in = input;
	float Fs = (float)sampleFreq;
	float thetaO = 150.0f;
	float c = 334.0f;
	float a = 0.08f;
	float wo = c / a;
	float alphamin = 0.05f;
	float alpha = 1.0f + alphamin + (1.0f - alphamin)*cosf(theta * (float)PI / thetaO);
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

	for (unsigned int i = 0; i < len - 1; i++, tempIt++, in++)
		*tempIt = ( *in*(wo + alpha * Fs) + *(in - 1)*(wo - alpha * Fs) - *(tempIt - 1)*(wo - Fs)) / (wo + Fs);

	*lastIn = *(--in);

	/*------- IDT ---------------*/
	if (abs(theta) < 90.0f)
		gDelay = -(Fs / wo)*(cosf(theta*(float)PI / 180.0f) - 1.0f);
	else
		gDelay = (Fs / wo)* ((abs(theta) - 90.0f)*(float)PI / 180.0f + 1.0f);

	float gCoef = (1.0f - gDelay) / (1.0f + gDelay);

	tempIt = buffer.begin();
	*out++ = *tempIt++ * gCoef + *lastTempOut - *lastOut * gCoef;


	for (unsigned int i = 0; i < len - 1; i++, out++, tempIt++)
		*out = *tempIt * gCoef + *(tempIt - 1) - *(out - 1) * gCoef;


	*lastTempOut = *(--tempIt);
	*lastOut = *(--out);
}
effect3D::~effect3D()
{
}
