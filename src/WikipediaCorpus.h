/*
 * WikipediaCorpus.h
 *
 *  Created on: Oct 1, 2015
 *      Author: tobiasnorlund
 */

#ifndef WIKIPEDIACORPUS_H_
#define WIKIPEDIACORPUS_H_

#include "Corpus.h"
#include <string>
#include <iostream>
#include <fstream>

namespace model {

class WikipediaCorpus: public model::Corpus {
public:
	WikipediaCorpus(std::string filepath);
	virtual ~WikipediaCorpus();

	float getProgress();
	bool read_line(std::string& val);
	void reset();

	std::string toString();
private:
	std::ifstream* file;
	unsigned long total_size;
	unsigned long total_read;
};

} /* namespace model */
#endif /* WIKIPEDIACORPUS_H_ */
