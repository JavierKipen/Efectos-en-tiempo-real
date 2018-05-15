#pragma once
#include <vector>
#include "Effect.h"
#include "BasicReverberator.h"
#include "CombReverberator.h"
#define NMBR_OF_COMB_FILTERS 4
#define COMB_DELAY_TIMES {0.03f, 0.0367f, 0.0433f, 0.05f}
#define NMBR_OF_AP_FILTERS 2
#define AP_DELAY_TIMES {0.02f, 0.03f}

#define MAX_LEN 10000

using namespace std;
class CompleteReverb
{
public:
	CompleteReverb(float a, float g, float depth, float fsample);
	void Action(const float *in, float *out, unsigned int len);
	~CompleteReverb();
private:
	float depth, combBuffer[MAX_LEN], APBuffer[MAX_LEN];
	vector<float> A, G;
	vector<unsigned int> CombTaps, APTaps;
	CombReverberator combs[NMBR_OF_COMB_FILTERS];
	BasicReverberator APs[NMBR_OF_AP_FILTERS];
};

