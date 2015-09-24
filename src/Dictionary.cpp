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

using namespace std;

namespace model {

/*
 * Allocates and generates num_words index and context matrices
 */
Dictionary::Dictionary(unsigned int num_words, int d, int epsilon, int k){

	this->num_words = num_words;
	this->d = d;
	this->epsilon = epsilon;
	this->k = k;

	// Init next_word_idx
	next_word_idx = 0;

	// Allocate and initialize index vectors
	srand(time(0));
	index_vectors = (unsigned short*) malloc(num_words * epsilon * sizeof(unsigned short));
	for (unsigned int i = 0; i < num_words * epsilon; ++i){
		index_vectors[i] = rand() % (d << 1);
	}

	// Allocate and initialize context matrices
	contexts = (int*) malloc(num_words*(2*k)*d * sizeof(int));
	memset(contexts, 0, num_words*(2*k)*d * sizeof(int)); // Init to 0

}

Dictionary::Dictionary(unsigned int num_words, int d, int epsilon, int k, unsigned short* idx_vectors, int* ctxs) {

	this->num_words = num_words;
	this->d = d;
	this->epsilon = epsilon;
	this->k = k;

	// Init next_word_idx
	next_word_idx = 0;

	index_vectors = idx_vectors;
	contexts = ctxs;
}

Dictionary::~Dictionary() {

	// Delete index vectors
	free(index_vectors);

	// Delete contexts
	free(contexts);
}

unsigned int Dictionary::getNumWords(){
	return next_word_idx;
}

void Dictionary::incrementCount(string word){
	++index_map[word].second;
}

/*
 * Assigns a new index vector and context if the word is unseen
 */
void Dictionary::newWord(string word){
	/*if(word == "Volatility"){
		cout << "hejj" << endl;
	}*/
	if(index_map.find(word) == index_map.end()){
		// Assign new word if not buffer full
		if(next_word_idx == num_words)
			throw runtime_error("Maximum number of words exceeded");
		index_map[word].first = next_word_idx++;
	}
}

/*
 * Returns the word's corresponding index vector.
 */
IndexVector Dictionary::getIndexVector(string word){
	// Assign a new word if unseen
	newWord(word);
	return IndexVector((unsigned short*)(index_vectors + index_map[word].first*epsilon), epsilon); // * sizeof(unsigned short)
}

/**
 * Returns the word's corresponding context.
 */
Context Dictionary::getContext(string word){
	// Assign a new word if unseen
	newWord(word);
	return Context((int*)(contexts + index_map[word].first*(2*k)*d), d); // * sizeof(int)
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

	ofstream fm(dir + name + "-" + to_string(next_word_idx) + "-" + to_string(d) + ".map", ofstream::out);
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
	fc.close();

}


}
