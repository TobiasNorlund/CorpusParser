/*
 * Context.h
 *
 *  Created on: Sep 18, 2015
 *      Author: tobiasnorlund
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "IndexVector.h"

namespace model {

class Context {
public:
	Context(int* source, unsigned short d) : source{source}, d{d} {};
	virtual ~Context();

	void add(IndexVector& vec, short relWindPos);
	int get(short relWindPos, unsigned short dim);
private:
	int* source;
	unsigned short d; // Word dimensionality
};

} /* namespace model */

#endif /* CONTEXT_H_ */
