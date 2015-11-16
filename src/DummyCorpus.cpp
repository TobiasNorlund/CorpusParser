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

void DummyCorpus::reset(){
	idx = 0;
}

bool DummyCorpus::read_line(string& line){
	if(idx++ == 0){
		line = "the cat sat on";
		return true;
	}else
		return false;
}
/*bool DummyCorpus::operator>>(string& val){
	string words[3] = {"the", "cat", "sat"};
	if(idx < 3){
		val = words[idx++];
		return true;
	}else{
		return false;
	}
}*/

string DummyCorpus::toString(){
	return "DummyCorpus";
}

} /* namespace model */
