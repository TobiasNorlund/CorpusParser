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
#include <sstream>
#include <iomanip>

using namespace std;

namespace model {

OneBCorpus::OneBCorpus(string dir) : dir{dir} {
	current_file = 1;
	initFile(current_file);
}

OneBCorpus::~OneBCorpus() {
	closeFile();
}

void OneBCorpus::initFile(int ind){
	total_read = 0;

	stringstream ss;
	ss << setw(5) << setfill('0') << ind;
	string filepath = dir + "news.en-" + ss.str() + "-of-00100";
	file = new ifstream(filepath);

	// Determine size of file
	ifstream in(filepath, ifstream::ate | ifstream::binary);
	total_size = in.tellg();

}
void OneBCorpus::closeFile(){
	file->close();
	delete file;
}


float OneBCorpus::getProgress(){
	return ((float)total_read) / (total_size * 99) + ((float)current_file-1)/99;
}

bool OneBCorpus::operator>>(string& val){
	bool res = (bool)(*file >> val);
	if(!res && current_file < 99){
		closeFile();
		initFile(++current_file);
		res = (bool)(*file >> val);
	}
	total_read += val.size() + 1;
	return res;
}

string OneBCorpus::toString(){
	return "OneBCorpus";
}

} /* namespace model */
