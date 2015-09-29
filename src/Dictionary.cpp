/*
 * Dictionary.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: tobiasnorlund
 */

#include "Dictionary.h"
#include <cstdlib>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

namespace model {

/*
 * Allocates and generates num_words index and context matrices
 */
Dictionary::Dictionary(unsigned int max_words, unsigned int max_words_per_pass, int d, int epsilon, int k, string dump_path){

	this->max_words = max_words;
	this->max_words_per_pass = max_words_per_pass;
	this->d = d;
	this->epsilon = epsilon;
	this->k = k;
	this->dump_path = dump_path;

	next_index = 0;
	current_pass = 0;

	// Allocate and initialize index vectors
	srand(time(0));
	index_vectors = (unsigned short*) malloc(max_words * epsilon * sizeof(unsigned short));
	for (unsigned int i = 0; i < max_words * epsilon; ++i){
		index_vectors[i] = rand() % (d << 1);
	}

}

Dictionary::Dictionary(unsigned int max_words, unsigned int max_words_per_pass, int d, int epsilon, int k, string dump_path, unsigned short* index_vectors){

	this->max_words = max_words;
	this->max_words_per_pass = max_words_per_pass;
	this->d = d;
	this->epsilon = epsilon;
	this->k = k;
	this->dump_path = dump_path;

	next_index = 0;
	current_pass = 0;

	// Allocate and initialize index vectors
	this->index_vectors = index_vectors;

}

void Dictionary::initPass(){

	current_pass++;

	// Allocate and initialize context matrices
	contexts = (int*) malloc(max_words_per_pass*(2*k)*d * sizeof(int));
	memset(contexts, 0, max_words_per_pass*(2*k)*d * sizeof(int)); // Init to 0

}

void Dictionary::endPass(){

	unsigned int words_in_this_pass = (current_pass < ceil((float)next_index / max_words_per_pass))?max_words_per_pass:next_index % max_words_per_pass;

	// Save contexts
	auto flags = ios::out | ios::binary;
	if(current_pass > 1) flags |= ios::app;
	ofstream fc(dump_path + "-" + to_string(next_index) + "-" + to_string(d) + "-" + to_string(k) + ".context.bin", flags);
	if (!fc || !fc.good()) throw runtime_error("Error saving contexts. Couldn't open/create file.");
	fc.write((char*)contexts, words_in_this_pass * (2*k)*d * sizeof(int));
	if (fc.fail()) throw runtime_error("Error saving contexts. Couldn't write data. ");
	fc.close();

	// Write word map text file (sorted on occurance and unsorted)
	ofstream fmo(dump_path + "-" + to_string(next_index) + "-" + to_string(d) + "-" + to_string(k) + ".map", ofstream::out);
	for(auto it : meta_map)
		fmo << it.first << " ";
	fmo.close();
	ofstream fmuo(dump_path + "-" + to_string(next_index) + "-" + to_string(d) + "-" + to_string(k) + ".ordered.map", ofstream::out);
	vector<pair<string, meta_data >> pairs;
	for (auto itr = meta_map.begin(); itr != meta_map.end(); ++itr)
	    pairs.push_back(*itr);
	sort(pairs.begin(), pairs.end(), [=](pair<string, meta_data>& a, pair<string, meta_data>& b)
		{
			return a.second.count > b.second.count;
		});
	for(auto it : pairs)
		fmuo << it.first << "\t" << it.second.count << endl;
	fmuo.close();

	// If first pass, save index vectors as well
	if(current_pass == 1){

		// Write index dump file
		ofstream fi(dump_path + "-" + to_string(next_index) + "-" + to_string(d) + "-" + to_string(k) + ".index.bin", ios::out | ios::binary);
		fi.write((char*)index_vectors, next_index * epsilon * sizeof(unsigned short));
		fi.close();

	}

	// Free the contexts
	free(contexts);

}

Dictionary::~Dictionary() {

	// Delete index vectors
	free(index_vectors);

	// Delete contexts
	free(contexts);
}

unsigned int Dictionary::getNumWords(){
	return next_index;
}

void Dictionary::incrementCount(string word){
	++meta_map[word].count;
}

/*
 * Assigns a new index vector and context if the word is unseen
 */
void Dictionary::newWord(string word){
	if(meta_map.find(word) == meta_map.end()){
		// Assign new word if not buffer full
		if(next_index == max_words)
			throw runtime_error("Maximum total number of words exceeded");

		meta_map[word] = {
				next_index, // index_index
				next_index % max_words_per_pass, //context_index
				0, //count
				1+next_index/max_words_per_pass // pass
		};
		next_index++;
	}
}

/*
 * Returns the word's corresponding index vector.
 */
IndexVector Dictionary::getIndexVector(string word){
	// Assign a new word if unseen
	newWord(word);
	return IndexVector((unsigned short*)(index_vectors + meta_map[word].index_index*epsilon), epsilon); // * sizeof(unsigned short)
}

/**
 * Returns the word's corresponding context.
 */
Context* Dictionary::getContext(string word){
	// Assign a new word if unseen
	newWord(word);
	if(meta_map[word].pass != current_pass)
		throw runtime_error("Word not in this pass");
	return new Context((int*)(contexts + meta_map[word].context_index*(2*k)*d), d); // * sizeof(int)
}

short Dictionary::getPass(string word){
	if(meta_map.find(word) == meta_map.end())
		throw runtime_error("Word '" + word + "' is not present in the Dictionary");
	return meta_map[word].pass;
}

/*
 * Creates three files:
 *
 *  - <filename>.map			= The words
 *  - <filename>.context.bin
 *  - <filename>.index.bin
 *
 *  which is a dump of the contexts and index vectors respectively
 */
void Dictionary::save(string dir, string name){

	/*ofstream fm(dir + name + "-" + to_string(next_word_idx) + "-" + to_string(d) + ".map", ofstream::out);
	ofstream fc(dir + name + "-" + to_string(next_word_idx) + "-" + to_string(d) + ".context.bin", ios::out | ios::binary);
	ofstream fi(dir + name + "-" + to_string(next_word_idx) + "-" + to_string(d) + ".index.bin", ios::out | ios::binary);

	// Write map file
	vector<pair<string, pair<unsigned int, unsigned int> >> pairs;
	for (auto itr = index_map.begin(); itr != index_map.end(); ++itr)
	    pairs.push_back(*itr);

	sort(pairs.begin(), pairs.end(), [=](pair<string, pair<unsigned int, unsigned int>>& a, pair<string, pair<unsigned int, unsigned int>>& b)
		{
			return a.second.second > b.second.second;
		});
	for(auto it : pairs)
		fm << it.first << "\t" << it.second.second << endl;
	fm.close();

	// Write index dump file
	fi.write((char*)index_vectors, next_word_idx * epsilon * sizeof(unsigned short));
	fi.close();

	// Write context dump file
	fc.write((char*)contexts, next_word_idx * (2*k)*d * sizeof(int));
	fc.close();*/

}


}
