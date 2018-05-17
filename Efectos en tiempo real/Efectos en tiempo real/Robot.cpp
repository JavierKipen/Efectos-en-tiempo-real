#include "Robot.h"

using namespace ASSD;

Robot::Robot(unsigned int sampleFreq,unsigned int N)
{
	this->sampleFreq = sampleFreq;
	this->N = N;
	paramNames = ROBOT_PARAM_NAMES;
	paramValues = ROBOT_PARAM_VALUES;
	windowed = true;
	inL = vector<complex<double>>(N);
	inR = vector<complex<double>>(N);
	prevInL = vector<complex<double>>(N);
	prevInR = vector<complex<double>>(N);
	OutL = vector<complex<double>>(N);
	OutR = vector<complex<double>>(N);
	block1L = vector<complex<double>>(N);
	block2L = vector<complex<double>>(N );
	block1R = vector<complex<double>>(N);
	block2R = vector<complex<double>>(N);
	HannWindow = vector<complex<double>>(N);
	for (unsigned int i = 0; i < N; i++)
	{
		HannWindow[i] = 0.5*(1 - cos(2 * 3.14159265*(double)i / (double)(N - 1)));
	}
}

bool Robot::Action(const float * in, float * out, unsigned int len)
{
	if (!windowed)
	{
		for (unsigned int i = 0; i < len; i++)
		{
			inL[i] = *in++;
			inR[i] = *in++;
		}
		FFT(inL.data(),inL.data(), len);
		for (auto& c : inL)
			c = abs(c);
		IFFT(inL.data(),inL.data(), len);
		FFT(inR.data(), inR.data(), len);
		for (auto& c : inR)
			c = abs(c);
		IFFT(inR.data(), inR.data(), len);
		for (unsigned int i = 0; i < len; i++)
		{
			(*out++)=inL[i].real();
			(*out++) = inR[i].real();
		}
	}
	else
	{
		/*Primer overlap*/
		for (unsigned int i = 0; i < len; i++)
		{
			block2L[i] = *in++;
			inL[i] = block2L[i];
			if (i < N / 2)
				block1L[i + (N / 2)] = block2L[i];
			block2R[i] = *in++;
			inR[i] = block2R[i];
			if (i < N / 2)
				block1R[i + (N / 2)] = block2R[i];
		}
		windowBlocks();
		robotizeBlocks();
		//deWindowBlocks();
		obtainOuputs();
		for (unsigned int i = 0; i < len; i++)
		{
			(*out++) = OutL[i].real();
			(*out++) = OutR[i].real();
		}
		savePreviousData();
	}
	return false;
}

bool Robot::setParam(string paramName, string paramValue)
{
	bool retVal = false;
	if (paramName == "Windowed")
	{
		if (paramValue == "Yes")
		{
			windowed = true;
			paramValues[0] = paramValue;
			retVal = true;
		}
		else if (paramValue == "No")
		{
			windowed = false;
			paramValues[0] = paramValue;
			retVal = true;
		}
	}
	return true;
}


Robot::~Robot()
{
}

void Robot::windowBlocks()
{
	for (unsigned int i = 0; i < N; i++)
	{
		block1L[i] = block1L[i] * HannWindow[i];
		block2L[i] = block2L[i] * HannWindow[i];
		block1R[i] = block1R[i] * HannWindow[i];
		block2R[i] = block2R[i] * HannWindow[i];
	}
}

void Robot::deWindowBlocks()
{
	for (unsigned int i = 0; i < N; i++)
	{
		block1L[i] = block1L[i] / HannWindow[i];
		block2L[i] = block2L[i] / HannWindow[i];
		block1R[i] = block1R[i] / HannWindow[i];
		block2R[i] = block2R[i] / HannWindow[i];
	}
}

void Robot::robotizeBlocks()
{
	robotize(block1L.data());
	robotize(block2L.data());
	robotize(block1R.data());
	robotize(block2R.data());
}
void Robot::savePreviousData()
{
	for (unsigned int i = 0; i < N/2; i++)
	{
		OutL[i] =  0.5 * block2L[i + (N / 2)].real();
		OutR[i] =  0.5 * block2R[i + (N / 2)].real();
		block1L[i] = inL[i + (N / 2)];
		block1R[i] = inR[i + (N / 2)];
	}
}
void Robot::robotize(complex<double> *a)
{
	FFT(a, a, N);
	for (unsigned int c=0; c< N;c++)
		a[c] = abs(a[c]);
	IFFT(a, a, N);
}
void Robot::obtainOuputs()
{
	for (unsigned int i = 0; i < N / 2; i++)
	{
		OutL[i] += 0.5 * block1L[i].real();
		OutR[i] += 0.5 * block1R[i].real();
	}
	for (unsigned int i = N / 2; i < N ; i++)
	{
		OutL[i] = 0.5 * block1L[i].real() + 0.5 * block2L[i- (N / 2)].real();
		OutR[i] = 0.5 * block1R[i].real() + 0.5 * block2R[i-(N / 2)].real();
	}
}
