/*
 * GpuParser.h
 *
 *  Created on: Sep 18, 2015
 *      Author: tobiasnorlund
 */

#ifndef GPUPARSER_H_
#define GPUPARSER_H_

#include "Corpus.h"
#include "Dictionary.h"

class GpuParser {
public:
	GpuParser();
	GpuParser(model::Dictionary* dictionary);
	virtual ~GpuParser();

	void parse(model::Corpus& corpus, int k, int d, int epsilon, unsigned long max_cpu_mem, unsigned long max_gpu_mem, unsigned long max_shared_mem);

	model::Dictionary* getDictionary();
private:
	bool dictSpecified = false;
	model::Dictionary* dictionary;
};

#endif
