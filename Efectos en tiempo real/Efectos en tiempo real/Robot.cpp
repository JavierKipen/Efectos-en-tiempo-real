#include "Robot.h"



Robot::Robot(unsigned int sampleFreq,unsigned int N)
{
	this->sampleFreq = sampleFreq;
	this->N = N;
	windowed = true;
	inL = vector<complex<float>>(N);
	inR = vector<complex<float>>(N);
	prevInL = vector<complex<float>>(N);
	prevInR = vector<complex<float>>(N);
	OutL = vector<complex<float>>(N);
	OutR = vector<complex<float>>(N);
	block1L = vector<complex<float>>(N);
	block2L = vector<complex<float>>(N );
	block1R = vector<complex<float>>(N);
	block2R = vector<complex<float>>(N);
	HannWindow = vector<complex<float>>(N);
	for (unsigned int i = 0; i < N; i++)
	{
		HannWindow[i] = 0.5*(1 - cos(2 * 3.14159265*(float)i / (float)(N - 1)));
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
		fft(inL.data(),inL.data(), len);
		for (auto& c : inL)
			c = abs(c);
		ifft(inL.data(),inL.data(), len);
		fft(inR.data(), inR.data(), len);
		for (auto& c : inR)
			c = abs(c);
		ifft(inR.data(), inR.data(), len);
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
void Robot::robotize(complex<float> *a)
{
	fft(a, a, N);
	for (unsigned int c=0; c< N;c++)
		a[c] = abs(a[c]);
	ifft(a, a, N);
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
