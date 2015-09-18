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

#include <iostream>

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
	index_vectors = new short*[num_words];
	for (unsigned int i = 0; i < num_words; ++i){
		index_vectors[i] = new short[epsilon];
		for (int j = 0; j < epsilon; ++j)
			index_vectors[i][j] = rand();
	}

	// Allocate and initialize context matrices
	/*contexts = new int**[num_words];
	for (unsigned int i = 0; i < num_words; ++i){
		contexts[i] = new int*[2*k];
		for (short j = 0; j < 2*k; ++j){
			contexts[i][j] = new int[d];
			for (short n = 0; n < d; ++n)
				contexts[i][j][n] = 0;
		}
	}*/
}

Dictionary::Dictionary(unsigned int num_words, int d, int epsilon, int k, short** idx_vectors, int*** ctxs) {

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
	for (unsigned int i = 0; i < num_words; ++i)
		delete[] index_vectors[i];
	delete[] index_vectors;

	// Delete contexts
	/*for (unsigned int i = 0; i < num_words; ++i){
		for (short j = 0; j < 2*k; ++j)
			delete[] contexts[i][j];
		delete[] contexts[i];
	}
	delete[] contexts;*/
}

/*
 * Returns the word's corresponding index vector.
 */
IndexVector Dictionary::getIndexVector(string word){

	if(index_map.find(word) == index_map.end()){
		// Assign new word if not buffer full
		if(next_word_idx == num_words)
			throw runtime_error("Maximum number of words exceeded");
		index_map[word] = next_word_idx++;
	}
	return IndexVector(index_vectors[index_map[word]]);
}

/**
 * Returns the word's corresponding context.
 */
Context Dictionary::getContext(string word){
	if(index_map.find(word) == index_map.end()){
		// Assign new word if buffer not full
		if(next_word_idx == num_words)
			throw runtime_error("Maximum number of words exceeded");
		index_map[word] = next_word_idx++;
	}
}

}
