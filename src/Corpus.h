/*
 * Corpus.h
 *
 *  Created on: Sep 18, 2015
 *      Author: tobiasnorlund
 */

#include <string>

#ifndef CORPUS_H_
#define CORPUS_H_

namespace model {

class Corpus {
public:
	Corpus();
	virtual ~Corpus();

	virtual std::string nextWord() =0;
	virtual float getProgress() =0;
};

} /* namespace model */
#endif /* CORPUS_H_ */
