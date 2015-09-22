/*
 * IndexVector.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: tobiasnorlund
 */

#include "IndexVector.h"
#include <string>

using namespace std;

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

string IndexVector::toString(){
	string s = "";
	for(int i = 0; i<epsilon; ++i){
		s += to_string(getIndexAt(i)) + ": " + to_string((int)getValueAt(i)) + "\n";
	}
	return s;
}

} /* namespace model */
