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
	Dictionary(unsigned int max_words, unsigned int max_words_per_pass, int d, int epsilon, int k, std::string dump_path);
	Dictionary(unsigned int max_words, unsigned int max_words_per_pass, int d, int epsilon, int k, std::string dump_path, unsigned short* index_vectors);
	virtual ~Dictionary();

	void initPass();
	void endPass();

	bool newWord(std::string word);
	void incrementCount(std::string word);
	short getPass(std::string word);
	unsigned int getCount(std::string word);
	IndexVector getIndexVector(std::string word);
	Context*	getContext(std::string word);

	void save(std::string dir, std::string name);
	unsigned int getNumWords();

	unsigned short* index_vectors;
	float* contexts;

private:

	struct meta_data {
		unsigned int index_index; // Index in the index_vectors
		unsigned int context_index; // Index in the contexts, in its pass
		unsigned int count;
		short pass;
	};

	unsigned int max_words;
	unsigned int max_words_per_pass;
	int d;
	int epsilon;
	int k;

	std::unordered_map<std::string, meta_data> meta_map; // first = index, second = #count

	unsigned int next_index;
	short current_pass;

	std::string dump_path;
};

} // end namespace: model

#endif /* DICTIONARY_H_ */
