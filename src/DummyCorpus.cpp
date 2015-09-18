/*
 * DummyCorpus.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: tobiasnorlund
 */

#include <string>
#include "DummyCorpus.h"

using namespace std;

namespace model {

DummyCorpus::DummyCorpus() {
	// TODO Auto-generated constructor stub

}

DummyCorpus::~DummyCorpus() {
	// TODO Auto-generated destructor stub
}

string DummyCorpus::nextWord(){
	string words[3] = {"the", "cat", "sat"};
	return words[idx++];
}

float DummyCorpus::getProgress(){
	return idx/3.0;
}

} /* namespace model */
