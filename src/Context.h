/*
 * Context.h
 *
 *  Created on: Sep 18, 2015
 *      Author: tobiasnorlund
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

namespace model {

class Context {
public:
	Context(int** source);
	virtual ~Context();
private:
	int** source;
};

} /* namespace model */

#endif /* CONTEXT_H_ */
