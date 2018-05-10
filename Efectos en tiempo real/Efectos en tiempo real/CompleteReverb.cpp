#include "CompleteReverb.h"



CompleteReverb::CompleteReverb(float a, float g, float depth, float fsample)
{
	A.resize(NMBR_OF_COMB_FILTERS, a);
	G.resize(NMBR_OF_AP_FILTERS, g);
	CombTaps.resize(NMBR_OF_COMB_FILTERS, 0);
	APTaps.resize(NMBR_OF_AP_FILTERS, 0);
	this->depth = depth;
	vector<float> aux = COMB_DELAY_TIMES;
	for (unsigned int i = 0; i < NMBR_OF_COMB_FILTERS; i++)
	{
		CombTaps[i] = (unsigned int)(aux[i] * fsample);
		combs[i].setParams(A[i], CombTaps[i]);
	}

	aux = AP_DELAY_TIMES;
	for (unsigned int i = 0; i < NMBR_OF_AP_FILTERS; i++)
	{
		APTaps[i] = (unsigned int)(aux[i] * fsample);
		APs[i].setParameters(G[i], APTaps[i]);
	}

}

void CompleteReverb::Action(const float * in, float * out, unsigned int len)
{
	for (unsigned int j = 0; j < 2*len; j++)
		APBuffer[j] = 0;
	for (unsigned int i = 0; i < NMBR_OF_COMB_FILTERS; i++)
	{
		combs[i].Action((float *)in, combBuffer, len);
		for (unsigned int j = 0; j < 2 * len; j++)
			APBuffer[j] += (1 / (float)NMBR_OF_COMB_FILTERS) * combBuffer[j];
	}
	for (unsigned int i = 0; i < NMBR_OF_AP_FILTERS; i++)
		APs[i].Action(APBuffer, APBuffer, len);
	for (unsigned int i = 0; i < 2 * len; i++)
		out[i] = in[i] + depth * APBuffer[i];
}


CompleteReverb::~CompleteReverb()
{
}
