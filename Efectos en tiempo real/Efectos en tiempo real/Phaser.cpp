#include "Phaser.h"

#define PHASER_DEPTH_INDEX 0
#define PHASER_SWEEP_INDEX 1
#define PHASER_LFO_FREQ_INDEX 2
#define PHASER_N_FILTERS_INDEX 3
#define PHASER_Q_INDEX 4
#define PHASER_GB_INDEX 5



Phaser::Phaser(unsigned int sampleFreq)
{
	this->sampleFreq = sampleFreq;
	paramNames = PHASER_PARAM_NAMES;
	paramValues = PHASER_DEFAULT_PARAM_VALUES;
	filterMinFrecs[0] = 200;
	filterMinFrecs[1] = 400;
	filterMinFrecs[2] = 800;
	filterMinFrecs[3] = 1600;
	filterMinFrecs[4] = 3200;
	saveValues();
	NotchFilter = vector<NotchOrder2>(5);
	for (auto& filter : NotchFilter)
		filter.setSampleFreq(sampleFreq);
}

bool Phaser::Action(const float * in, float * out, unsigned int len)
{
	//fo = 1080;
	sweepWidth = 200;
	float faux, aux[2];
	for (unsigned int i = 0; i <  len; i++)
	{
		faux = filterMinFrecs[0] + sweepWidth * sin(2 * 3.14159265*(float)sampleCount*lfoFreq / sampleFreq);
		NotchFilter[0].setParameters(faux, 0.99);
		NotchFilter[0].filter((float *)in, aux, 1);
		for (int i = 1; i < 5; i++)
		{
			faux = filterMinFrecs[i] + sweepWidth * sin(2 * 3.14159265*(float)sampleCount*lfoFreq / sampleFreq);
			NotchFilter[i].setParameters(faux, 0.99);
			NotchFilter[i].filter(aux, aux, 1);
		}
		/*faux = fo + sweepWidth * sin(2 * 3.14159265*(float)sampleCount*lfoFreq / sampleFreq);
		NotchFilter.setParameters(faux, 0.99);
		NotchFilter.filter((float *)in, aux, 1);*/
		*out++ = *in++ - aux[0];
		*out++ = *in++ - aux[1];
		sampleCount++;
	}
	return true;
}

bool Phaser::setParam(string paramName, string paramValue)
{
	return false;
}



void Phaser::updateFilters()
{
	varBRFilter.sampleFreq = (float)sampleFreq;
	varBRFilter.numberOfFilters = numberOfFilters;
	if (prevNmbrOfFilters != numberOfFilters) //En el caso que se cambio el número de filtros, se crearan los filtros necesarios.
		varBRFilter.createFilters();
	varBRFilter.gb = gb;
	varBRFilter.Q = Q;
	varBRFilter.sweepWidth = sweepWidth;
	varBRFilter.lfoFreq = lfoFreq;
	memcpy(varBRFilter.filterMinFrecs, filterMinFrecs, MAX_NMBR_OF_FILTERS);
	varBRFilter.recalcFiltersCoefs();
}


void Phaser::saveValues()
{
	prevNmbrOfFilters = numberOfFilters;
	depth = stof(paramValues[PHASER_DEPTH_INDEX]);
	sweepWidth = stof(paramValues[PHASER_SWEEP_INDEX]);
	lfoFreq = stof(paramValues[PHASER_LFO_FREQ_INDEX]);
	numberOfFilters = stoi(paramValues[PHASER_N_FILTERS_INDEX]);
	Q = stof(paramValues[PHASER_Q_INDEX]);
	gb = stof(paramValues[PHASER_GB_INDEX]);
	
	updateFilters();
}

Phaser::~Phaser()
{
}