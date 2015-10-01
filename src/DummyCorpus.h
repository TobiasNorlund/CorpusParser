/*
 * DummyCorpus.h
 *
 *  Created on: Sep 18, 2015
 *      Author: tobiasnorlund
 */

#ifndef DUMMYCORPUS_H_
#define DUMMYCORPUS_H_

#include "Corpus.h"

namespace model {

class DummyCorpus: public Corpus {
public:
	DummyCorpus();
	virtual ~DummyCorpus();

	float getProgress();
	bool read_line(std::string& line);
	void reset();

	std::string toString();
private:
	int idx = 0;
};

} /* namespace model */
#endif /* DUMMYCORPUS_H_ */
