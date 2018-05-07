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
	filterMinFrecs[2] = 600;
	filterMinFrecs[3] = 800;
	filterMinFrecs[4] = 1000;
	filterMinFrecs[5] = 1200;
	saveValues();
}

bool Phaser::Action(const float * in, float * out, unsigned int len)
{
	varBRFilter.filter((float *)in, out, len);
	/*for (unsigned int i = 0; i < 2 * len; i++)
		out[i] = in[i] * (1.0f - (depth / 2.0f)) + out[i] * (depth / 2.0f);*/
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