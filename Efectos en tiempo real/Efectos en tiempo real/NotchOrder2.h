#pragma once
#include <math.h>
class NotchOrder2
{
public:
	NotchOrder2();
	void setSampleFreq(float fsample);
	void setParameters(float f0, float R);
	void filter(float *in, float *out, unsigned int len);
	~NotchOrder2();
private:
	float fsample,coefsX[3], coefsY[3],XnL[3],YnL[3], XnR[3], YnR[3],gainL,gainR;
};

