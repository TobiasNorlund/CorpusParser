/*
 * IndexVector.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: tobiasnorlund
 */

#include "IndexVector.h"

namespace model {

IndexVector::~IndexVector() {
	// TODO Auto-generated destructor stub
}

unsigned short* IndexVector::getSource(){
	return source;
}

short IndexVector::getEpsilon(){
	return epsilon;
}

unsigned short IndexVector::getIndexAt(short ind){
	return *(source + ind) >> 1;
}

char IndexVector::getValueAt(short ind){
	return (*(source + ind) & 1 == 1)*2-1; // returns 1 if first bit == 1, otherwise -1
}

} /* namespace model */
