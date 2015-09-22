/*
 * IndexVector.h
 *
 *  Created on: Sep 18, 2015
 *      Author: tobiasnorlund
 */

#ifndef INDEXVECTOR_H_
#define INDEXVECTOR_H_

#include <string>

namespace model {

class IndexVector {
public:
	IndexVector(unsigned short* source, short epsilon) : source{source}, epsilon{epsilon} {};
	virtual ~IndexVector();

	unsigned short* getSource();
	short  getEpsilon();
	char   getValueAt(short ind);
	unsigned short getIndexAt(short ind);
	std::string toString();
private:
	unsigned short* source;
	short epsilon;
};

} /* namespace model */
#endif /* INDEXVECTOR_H_ */
