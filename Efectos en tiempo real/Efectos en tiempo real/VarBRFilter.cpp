#include "VarBRFilter.h"
#define PI 3.14159265


VarBRFilter::VarBRFilter()
{
	numberOfFilters = 0;
	pToFilters = NULL;
	sampleCount = 0;
}

void VarBRFilter::filter(float * in, float * out, unsigned int len)
{
	for (unsigned int j = 0; j < len; j++) //Esto dsps se efectivizará.
	{
		calcSweepIndex();
		/*Para cada filtro proceso la muestra L Y R, y pongo a la salida lo que da.*/
		pToFilters[0].filter(in, aux, 1, sweepIndex);
		for (unsigned int i = 1; i < numberOfFilters; i++)
			pToFilters->filter(aux, aux, 1, sweepIndex);
		memcpy(out, aux, 2 * 1); //2 veces pk es estereo.
		sampleCount++;
	}
}

void VarBRFilter::recalcFiltersCoefs()
{
	calcFreqRangeForEachFilter();
	for (unsigned int i = 0; i < numberOfFilters; i++)
		pToFilters[i].setParameters(filterFrecRange[i], gb, Q);

}

void VarBRFilter::createFilters()
{
	if (pToFilters != NULL)
		delete pToFilters;
	pToFilters = new BandRejectIIROrder2[numberOfFilters];
	for (unsigned int i = 0; i < numberOfFilters; i++)
	{
		pToFilters[i].setSampleFreq(sampleFreq);
		pToFilters[i].setNmbrOfCoefsPreCalc(SWEEP_POINTS);
	}

}

void VarBRFilter::calcFreqRangeForEachFilter()
{
	for (unsigned int i = 0; i < numberOfFilters; i++)
	{
		for (unsigned int j = 0; j < SWEEP_POINTS; j++)
			filterFrecRange[i][j] = j * (sweepWidth*(filterMinFrecs[i + 1] - filterMinFrecs[i]) / (2.0f*(float)SWEEP_POINTS)) + filterMinFrecs[i]; //Esto es lineal, dsps debería ser log
	}
}
void VarBRFilter::calcSweepIndex()
{
	sweepIndex = (unsigned int)((float)(SWEEP_POINTS - 1)*(sin(2 * PI*(lfoFreq / sampleFreq) * (float)sampleCount) + 0.5f));
}


VarBRFilter::~VarBRFilter()
{
}


