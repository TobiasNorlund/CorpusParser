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
	contexts = (float*) malloc(max_words_per_pass*(2*k)*d * sizeof(float));
	memset(contexts, 0, max_words_per_pass*(2*k)*d * sizeof(float)); // Init to 0

}

void Dictionary::endPass(){

	unsigned int words_in_this_pass = (current_pass < ceil((float)next_index / max_words_per_pass))?max_words_per_pass:next_index % max_words_per_pass;

	// Save contexts
	auto flags = ios::out | ios::binary;
	if(current_pass > 1) flags |= ios::app;
	ofstream fc(dump_path + "-" + to_string(next_index) + "-" + to_string(d) + "-" + to_string(k) + ".context.bin", flags);
	if (!fc || !fc.good()) throw runtime_error("Error saving contexts. Couldn't open/create file.");
	fc.write((char*)contexts, words_in_this_pass * (2*k)*d * sizeof(float));
	if (fc.fail()) throw runtime_error("Error saving contexts. Couldn't write data. ");
	fc.close();

	// If first pass, save index vectors as well
	if(current_pass == 1){

		// Write index dump file
		ofstream fi(dump_path + "-" + to_string(next_index) + "-" + to_string(d) + "-" + to_string(k) + ".index.bin", ios::out | ios::binary);
		fi.write((char*)index_vectors, next_index * epsilon * sizeof(unsigned short));
		fi.close();

		// Write word map text file (sorted on occurance and unsorted)
		ofstream fmo(dump_path + "-" + to_string(next_index) + "-" + to_string(d) + "-" + to_string(k) + ".map", ofstream::out);
		vector<pair<string, meta_data >> pairs;
		for (auto itr = meta_map.begin(); itr != meta_map.end(); ++itr)	pairs.push_back(*itr);
		sort(pairs.begin(), pairs.end(), [=](pair<string, meta_data> const& a, pair<string, meta_data> const& b)
			{
				return a.second.index_index < b.second.index_index;
			});
		for(auto it : pairs)
			fmo << it.first << " ";
		fmo.close();

		ofstream fmuo(dump_path + "-" + to_string(next_index) + "-" + to_string(d) + "-" + to_string(k) + ".ordered.map", ofstream::out);
		pairs.clear();
		for (auto itr = meta_map.begin(); itr != meta_map.end(); ++itr)
		    pairs.push_back(*itr);
		sort(pairs.begin(), pairs.end(), [=](pair<string, meta_data> const& a, pair<string, meta_data> const& b)
			{
				return a.second.count > b.second.count;
			});
		for(auto it : pairs)
			fmuo << it.first << "\t" << it.second.count << endl;
		fmuo.close();

	}

	// Free the contexts
	free(contexts);

}

Dictionary::~Dictionary() {

	// Delete index vectors
	free(index_vectors);

}

unsigned int Dictionary::getNumWords(){
	return next_index;
}

unsigned int Dictionary::getCount(string word){
	return meta_map[word].count;
}

void Dictionary::incrementCount(string word){
	++meta_map[word].count;
}

/*
 * Assigns a new index vector and context if the word is unseen. Returns false if maximum words is reached, otherwise true
 */
bool Dictionary::newWord(string word){
	if(meta_map.find(word) == meta_map.end()){
		// Assign new word if not buffer full
		if(next_index == max_words){
			return false;
		}

		meta_map[word] = {
				next_index, // index_index
				next_index % max_words_per_pass, //context_index
				0, //count
				1+next_index/max_words_per_pass // pass
		};
		next_index++;
	}
	return true;
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
	return new Context((float*)(contexts + meta_map[word].context_index*(2*k)*d), d); // * sizeof(int)
}

short Dictionary::getPass(string word){
	if(meta_map.find(word) == meta_map.end())
		throw runtime_error("Word '" + word + "' is not present in the Dictionary");
	return meta_map[word].pass;
}


}
