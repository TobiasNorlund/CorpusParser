/*
 * CpuParser.h
 *
 *  Created on: Sep 21, 2015
 *      Author: tobiasnorlund
 */

#ifndef CPUPARSER_H_
#define CPUPARSER_H_

#include "Corpus.h"
#include "Dictionary.h"

class CpuParser {
public:
	CpuParser();
	CpuParser(model::Dictionary* dictionary);
	virtual ~CpuParser();

	void parse(model::Corpus& corpus, int k, int d, int epsilon, unsigned long max_cpu_mem, unsigned int max_words, std::string dump_path);

	model::Dictionary* getDictionary();
private:
	bool dictSpecified = false;
	model::Dictionary* dictionary;

};

#endif /* CPUPARSER_H_ */
