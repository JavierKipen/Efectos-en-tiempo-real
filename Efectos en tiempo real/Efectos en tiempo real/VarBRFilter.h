#pragma once
#include "BandRejectIIROrder2.h"
#include <cstring>

#define MAX_INPUT_LEN 1024

#define MAX_NMBR_OF_FILTERS 32

#define SWEEP_POINTS 100



class VarBRFilter
{
public:
	VarBRFilter();
	void filter(float *in, float *out, unsigned int len);
	void recalcFiltersCoefs();
	void createFilters();
	~VarBRFilter();
	unsigned int numberOfFilters; 
	float filterMinFrecs[MAX_NMBR_OF_FILTERS];  //Aca deberan ser puestas las frecs minimas del filtro, y en el último lugar la frecuencia del "siguiente filtro", así se sabe en donde mover el último.
	float fMin, fStep, gb, sampleFreq,Q, sweepWidth, lfoFreq;
private:
	void calcFreqRangeForEachFilter();
	void calcSweepIndex();
	unsigned int sweepIndex, sampleCount;
	BandRejectIIROrder2 * pToFilters;
	float aux[MAX_INPUT_LEN];
	float filterFrecRange[MAX_NMBR_OF_FILTERS][SWEEP_POINTS]; //Para cada filtro, las frecuencias en las que variará su notch ordenadas.

};

