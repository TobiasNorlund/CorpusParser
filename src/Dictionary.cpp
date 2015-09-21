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
	index_vectors = (short*) malloc(num_words * epsilon * sizeof(short));
	for (unsigned int i = 0; i < num_words * epsilon; ++i){
		index_vectors[i] = rand();
	}

	// Allocate and initialize context matrices
	contexts = (int*) malloc(num_words*(2*k)*d * sizeof(int));
	memset(contexts, 0, num_words*(2*k)*d * sizeof(int)); // Init to 0

}

Dictionary::Dictionary(unsigned int num_words, int d, int epsilon, int k, short* idx_vectors, int* ctxs) {

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
	return IndexVector((short*)(index_vectors + index_map[word]*epsilon * sizeof(short)));
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
	return Context((int*)(contexts + index_map[word]*(2*k)*d * sizeof(int)));
}

}
