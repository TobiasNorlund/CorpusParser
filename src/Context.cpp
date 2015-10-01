/*
 * Context.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: tobiasnorlund
 */

#include "Context.h"

namespace model {


Context::~Context() {
	// TODO Auto-generated destructor stub
}

void Context::add(IndexVector& vec, short relWindPos, float weight){
	short epsilon = vec.getEpsilon();
	for(short i = 0; i < epsilon; ++i){
		*(source + relWindPos * d + vec.getIndexAt(i)) += weight * vec.getValueAt(i);
	}
}

float Context::get(short relWindPos, unsigned short dim){
	return *(source + relWindPos*d + dim);
}

} /* namespace model */
