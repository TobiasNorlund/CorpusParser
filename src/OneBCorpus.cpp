/*
 * OneBCorpus.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: tobiasnorlund
 */

#include "OneBCorpus.h"
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

namespace model {

OneBCorpus::OneBCorpus(string filepath) : file(filepath) {
	// Look up the total filesize
	ifstream in(filepath, ifstream::ate | ifstream::binary);
	total_size = in.tellg();
	total_read = 0;
}

OneBCorpus::~OneBCorpus() {
	file.close();
}

string OneBCorpus::nextWord(){
	string word;
	if(file >> word)
		return word;
	else
		throw runtime_error("No more words");
}
float OneBCorpus::getProgress(){
	return ((float)total_read) / total_size;
}

bool OneBCorpus::hasMore(){
	return !file.eof();
}

bool OneBCorpus::operator>>(string& val){
	return (bool)(file >> val);
}

string OneBCorpus::toString(){
	return "OneBCorpus";
}

} /* namespace model */
