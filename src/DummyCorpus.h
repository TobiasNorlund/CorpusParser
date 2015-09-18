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

	std::string nextWord();
	float getProgress();

private:
	int idx = 0;
};

} /* namespace model */
#endif /* DUMMYCORPUS_H_ */
