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

	virtual float getProgress() =0;
	virtual bool read_line(std::string& line) =0;
	virtual void reset() =0;

	virtual std::string toString() =0;
};

} /* namespace model */
#endif /* CORPUS_H_ */
