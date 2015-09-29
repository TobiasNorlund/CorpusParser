/*
 * CpuParser.cpp
 *
 *  Created on: Sep 21, 2015
 *      Author: tobiasnorlund
 */

#include "CpuParser.h"
#include "Corpus.h"
#include "Dictionary.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>

using namespace model;
using namespace std;

CpuParser::CpuParser(){
	this->dictSpecified = false;
}

CpuParser::CpuParser(Dictionary* dictionary){
	this->dictionary = dictionary;
	this->dictSpecified = true;
}

CpuParser::~CpuParser(){
	if(!dictSpecified)
		delete dictionary;
}

Dictionary* CpuParser::getDictionary(){
	return dictionary;
}

void print_window(vector<string> wind){
	for(auto it = wind.begin(); it != wind.end(); ++it)
		cout << *it << " ";
	cout << endl;
}


void CpuParser::parse(Corpus& corpus, int k, int d, int epsilon, unsigned long max_cpu_mem, unsigned int max_words, string dump_path){

	size_t win_size = 2*k+1;

	// Calculate how to distribute this job
	unsigned int max_words_per_pass = (max_cpu_mem - sizeof(short)*epsilon*max_words) / (sizeof(int)*d * k*2);
	short passes_needed = 0;

	if(!dictSpecified)
		this->dictionary = new Dictionary(max_words, max_words_per_pass, d, epsilon, k, dump_path);

	// Begin parse !
	cout << endl;
	cout << "Begin parsing of corpus: " << corpus.toString() << endl << endl;
	cout << " Maximum unique words: " << max_words << endl;
	cout << " Maximum unique words per pass: " << max_words_per_pass << endl;
	cout << " Total memory allowed: " << max_cpu_mem << " bytes" << endl;
	cout << " Window size: " << k << " + " << k << endl;
	cout << " Word embedding dimensionality: " << d << endl;
	cout << " Non-zero elements in index vectors: " << epsilon << endl;
	cout << endl;

	// Read in first window
	vector<string> window(win_size);
	int wi;
	for(wi = 0; wi < 2*k; ++wi){
		corpus >> window[wi];
		dictionary->newWord(window[wi]);
	}

	unsigned long processed_words_count = 2*k;

	// Iterate through all words in corpus
	char* win_idx = new char[2*k];
	for(short i=0;i<2*k;++i) win_idx[i] = (i<k)?i-k:i-k+1;

	short pass = 1;
	do{ // Loop over the dataset in multiple passes

		dictionary->initPass();

		while(corpus >> window[wi]){

			// Determine the focus word
			int fwi = (win_size + wi-k) % win_size; // Focus word index (in window)
			Context* fContext;
			dictionary->newWord(window[fwi]);
			if(dictionary->getPass(window[fwi]) == pass){ // Only process word if in this pass, otherwise skip this iteration
				fContext = dictionary->getContext(window[fwi]);

				// Loop over all words within window (except focus word)
				short wj;
				for(short j = 0; j < 2*k; ++j){
					wj = (win_size + fwi + win_idx[j]) % win_size; // word index (in window)
					try{
						IndexVector iv = dictionary->getIndexVector(window[wj]);
						fContext->add(iv, j);
					}catch (const runtime_error& error){
						continue; // Out of words, skip add this context
					}
				}
				delete fContext;
				dictionary->incrementCount(window[fwi]);
			}

			// Update wi
			wi = (wi + 1) % win_size;
			processed_words_count += 1;

			if(processed_words_count % 40000 == 0){
				string passes_str = to_string(pass); if(passes_needed != 0) passes_str += " (of " + to_string(passes_needed) + ")";
				cout << "\rPass: " << passes_str <<
						" | Total words: " << dictionary->getNumWords() << " (" << dictionary->getNumWords() * 100 /max_words << "%)" <<
						" | Progress: " << corpus.getProgress()*100 << "%" << flush;
			}
		}

		cout << "\rEnding pass " << pass << " ...                                                            " << flush;

		dictionary->endPass();
		corpus.reset();

		passes_needed = ceil((float)dictionary->getNumWords() / max_words_per_pass);

	}while(++pass <= passes_needed);

	cout << "\rWords processed: " << processed_words_count <<
						" | Total words: " << dictionary->getNumWords() << " (" << dictionary->getNumWords() * 100 /max_words << "%)" <<
						" | Progress: " << corpus.getProgress()*100 << "%" << endl;
	delete[] win_idx;

}
