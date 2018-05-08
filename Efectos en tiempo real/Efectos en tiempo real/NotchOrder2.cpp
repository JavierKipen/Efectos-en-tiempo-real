#include "NotchOrder2.h"
#define PI 3.14159265


NotchOrder2::NotchOrder2()
{
	
}

void NotchOrder2::setSampleFreq(float fsample)
{
	this->fsample = fsample;
}

void NotchOrder2::setParameters(float f0, float R)
{
	float wo= 2 * PI * f0/fsample;
	coefsX[0] = 1; //Coeficiente del filtro para la frecuencia i, que acompaña al término de z^0 de la entrada.
	coefsX[1] = (-2)*cos(wo);
	coefsX[2] = 1;
	coefsY[1] = 2 * R*cos(wo);
	coefsY[2] = (-R)*R;
	gainL = 1;
	gainR = 1;
}

void NotchOrder2::filter(float * in, float * out, unsigned int len)
{
	for (unsigned int j = 0; j < len; j++)
	{
		/*Se actualizan los valores previos de entrada, se calcula el de salida y se calcula el de salida, para L Y R.*/
		/*0 será el valor más nuevo, 2 el más viejo.*/
		XnL[2] = XnL[1];
		XnL[1] = XnL[0];
		XnL[0] = *in++;

		YnL[2] = YnL[1];
		YnL[1] = YnL[0];

		YnL[0] = gainL * (coefsX[0] * XnL[0] + coefsX[1] * XnL[1] + coefsX[2] * XnL[2] + coefsY[1] * YnL[1] + coefsY[2] * YnL[2]);
		*out++ = YnL[0];

		XnR[2] = XnR[1];
		XnR[1] = XnR[0];
		XnR[0] = *in++;

		YnR[2] = YnR[1];
		YnR[1] = YnR[0];

		YnR[0] = gainR * (coefsX[0] * XnR[0] + coefsX[1] * XnR[1] + coefsX[2] * XnR[2] + coefsY[1] * YnR[1] + coefsY[2] * YnR[2]);
		*out++ = YnR[0];
	}
}


NotchOrder2::~NotchOrder2()
{
}
