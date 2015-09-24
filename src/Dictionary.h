/*
 * Dictionary.h
 *
 *  Created on: Sep 17, 2015
 *      Author: tobiasnorlund
 */
#include <unordered_map>
#include <string>
#include "IndexVector.h"
#include "CpuParser.h"
#include "Context.h"

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

namespace model {

class Dictionary {
	friend class CpuParser;
public:
	Dictionary(unsigned int num_words, int d, int epsilon, int k);
	Dictionary(unsigned int num_words, int d, int epsilon, int k, unsigned short* index_vectors, int* contexts);
	virtual ~Dictionary();

	void newWord(std::string word);
	void incrementCount(std::string word);
	void save(std::string dir, std::string name);

	IndexVector getIndexVector(std::string word);
	Context	getContext(std::string word);
	unsigned int getNumWords();

	unsigned short* index_vectors;
	int* contexts;

private:

	unsigned int num_words;
	int d;
	int epsilon;
	int  k;

	std::unordered_map<std::string, std::pair<unsigned int, unsigned int>> index_map; // first = index, second = #count

	unsigned int next_word_idx;
};

} // end namespace: model

#endif /* DICTIONARY_H_ */
