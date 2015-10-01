/*
 * WikipediaCorpus.cpp
 *
 *  Created on: Oct 1, 2015
 *      Author: tobiasnorlund
 */

#include "WikipediaCorpus.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

namespace model {

WikipediaCorpus::WikipediaCorpus(string filepath) {
	// Determine size of file
	ifstream in(filepath, ifstream::ate | ifstream::binary);
	total_size = in.tellg();
	in.close();
	total_read = 0;

	file = new ifstream(filepath);
}

WikipediaCorpus::~WikipediaCorpus() {
	file->close();
}

float WikipediaCorpus::getProgress(){
	return (float)total_read / total_size;
}

bool WikipediaCorpus::read_line(string& line){
	bool res = (bool) getline(*file, line);
	if(res) total_read += line.size() + 1;
	return res;
}

void WikipediaCorpus::reset(){
	// Seek file to beginning
	file->seekg(0, file->beg);
}

string WikipediaCorpus::toString(){
	return "Wikipedia";
}

} /* namespace model */
