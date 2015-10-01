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
	Context(float* source, unsigned short d) : source{source}, d{d} {};
	virtual ~Context();

	void add(IndexVector& vec, short relWindPos, float weight);
	float get(short relWindPos, unsigned short dim);
private:
	float* source;
	unsigned short d; // Word dimensionality
};

} /* namespace model */

#endif /* CONTEXT_H_ */
