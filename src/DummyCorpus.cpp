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

float DummyCorpus::getProgress(){
	return idx/3.0;
}


bool DummyCorpus::operator>>(string& val){
	string words[3] = {"the", "cat", "sat"};
	if(idx < 3){
		val = words[idx++];
		return true;
	}else{
		return false;
	}
}

string DummyCorpus::toString(){
	return "DummyCorpus";
}

} /* namespace model */
