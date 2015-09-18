/*
 * IndexVector.h
 *
 *  Created on: Sep 18, 2015
 *      Author: tobiasnorlund
 */

#ifndef INDEXVECTOR_H_
#define INDEXVECTOR_H_

namespace model {

class IndexVector {
public:
	IndexVector(short* source) : _source{source} {};
	virtual ~IndexVector();

	short* getSource();
private:
	short* _source;
};

} /* namespace model */
#endif /* INDEXVECTOR_H_ */
