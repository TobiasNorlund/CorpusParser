/*
 * OneBCorpus.h
 *
 *  Created on: Sep 22, 2015
 *      Author: tobiasnorlund
 */

#ifndef ONEBCORPUS_H_
#define ONEBCORPUS_H_

#include "Corpus.h"
#include <iostream>
#include <fstream>

namespace model {

class OneBCorpus: public model::Corpus {
public:
	OneBCorpus(std::string dir);
	virtual ~OneBCorpus();

	float getProgress();
	bool operator>>(std::string& val);
	void reset();

	std::string toString();

private:
	std::string dir;
	int current_file;
	std::ifstream* file;
	std::string next_word;
	unsigned long total_size;
	unsigned long total_read;

	void initFile(int ind);
	void closeFile();
};

} /* namespace model */
#endif /* ONEBCORPUS_H_ */
