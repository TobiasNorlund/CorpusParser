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
	OneBCorpus(std::string filepath);
	virtual ~OneBCorpus();

	std::string nextWord();
	float getProgress();

	bool hasMore();
	bool operator>>(std::string& val);

	std::string toString();

private:
	std::ifstream file;
	std::string next_word;
	unsigned long total_size;
	unsigned long total_read;
};

} /* namespace model */
#endif /* ONEBCORPUS_H_ */
