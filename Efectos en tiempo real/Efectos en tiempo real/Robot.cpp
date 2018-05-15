#include "Robot.h"



Robot::Robot(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	windowed = true;
	inL = vector<complex<float>>(ROBOT_DEFAULT_LEN);
	inR = vector<complex<float>>(ROBOT_DEFAULT_LEN);
	prevInL = vector<complex<float>>(ROBOT_DEFAULT_LEN);
	prevInR = vector<complex<float>>(ROBOT_DEFAULT_LEN);
	OutL = vector<complex<float>>(ROBOT_DEFAULT_LEN);
	OutR = vector<complex<float>>(ROBOT_DEFAULT_LEN);
	block1L = vector<complex<float>>(ROBOT_DEFAULT_LEN);
	block2L = vector<complex<float>>(ROBOT_DEFAULT_LEN );
	block1R = vector<complex<float>>(ROBOT_DEFAULT_LEN);
	block2R = vector<complex<float>>(ROBOT_DEFAULT_LEN);
	HannWindow = vector<complex<float>>(ROBOT_DEFAULT_LEN);
	for (unsigned int i = 0; i < ROBOT_DEFAULT_LEN; i++)
	{
		HannWindow[i] = 0.5*(1 - cos(2 * 3.14159265*(float)i / (float)(ROBOT_DEFAULT_LEN - 1)));
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
			if (i < ROBOT_DEFAULT_LEN / 2)
				block1L[i + (ROBOT_DEFAULT_LEN / 2)] = block2L[i];
			block2R[i] = *in++;
			inR[i] = block2R[i];
			if (i < ROBOT_DEFAULT_LEN / 2)
				block1R[i + (ROBOT_DEFAULT_LEN / 2)] = block2R[i];
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
	for (unsigned int i = 0; i < ROBOT_DEFAULT_LEN; i++)
	{
		block1L[i] = block1L[i] * HannWindow[i];
		block2L[i] = block2L[i] * HannWindow[i];
		block1R[i] = block1R[i] * HannWindow[i];
		block2R[i] = block2R[i] * HannWindow[i];
	}
}

void Robot::deWindowBlocks()
{
	for (unsigned int i = 0; i < ROBOT_DEFAULT_LEN; i++)
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
	for (unsigned int i = 0; i < ROBOT_DEFAULT_LEN/2; i++)
	{
		OutL[i] =  0.5 * block2L[i + (ROBOT_DEFAULT_LEN / 2)].real();
		OutR[i] =  0.5 * block2R[i + (ROBOT_DEFAULT_LEN / 2)].real();
		block1L[i] = inL[i + (ROBOT_DEFAULT_LEN / 2)];
		block1R[i] = inR[i + (ROBOT_DEFAULT_LEN / 2)];
	}
}
void Robot::robotize(complex<float> *a)
{
	fft(a, a, ROBOT_DEFAULT_LEN);
	for (unsigned int c=0; c< ROBOT_DEFAULT_LEN;c++)
		a[c] = abs(a[c]);
	ifft(a, a, ROBOT_DEFAULT_LEN);
}
void Robot::obtainOuputs()
{
	for (unsigned int i = 0; i < ROBOT_DEFAULT_LEN / 2; i++)
	{
		OutL[i] += 0.5 * block1L[i].real();
		OutR[i] += 0.5 * block1R[i].real();
	}
	for (unsigned int i = ROBOT_DEFAULT_LEN / 2; i < ROBOT_DEFAULT_LEN ; i++)
	{
		OutL[i] = 0.5 * block1L[i].real() + 0.5 * block2L[i- (ROBOT_DEFAULT_LEN / 2)].real();
		OutR[i] = 0.5 * block1R[i].real() + 0.5 * block2R[i-(ROBOT_DEFAULT_LEN / 2)].real();
	}
}
