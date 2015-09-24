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


void CpuParser::parse(Corpus& corpus, int k, int d, int epsilon, unsigned long max_cpu_mem){

	size_t win_size = 2*k+1;

	// Calculate how to distribute this job
	unsigned int max_words = max_cpu_mem / (sizeof(short)*epsilon + sizeof(int)*d * k*2);

	if(!dictSpecified)
		this->dictionary = new Dictionary(max_words, d, epsilon, k);

	// Begin parse !
	cout << endl;
	cout << "Begin parsing of corpus: " << corpus.toString() << endl << endl;
	cout << " Maximum number of unique words allowed: " << max_words << endl;
	cout << " Maximum number of memory allowed: " << max_cpu_mem << " bytes" << endl;
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
		//print_window(window);
	}

	unsigned long processed_words_count = 2*k;

	// Iterate through all words in corpus
	char* win_idx = new char[2*k];
	for(short i=0;i<2*k;++i) win_idx[i] = (i<k)?i-k:i-k+1;
	try {
		while(corpus >> window[wi]){

			//print_window(window);

			// Determine the focus word
			int fwi = (win_size + wi-k) % win_size; // Focus word index (in window)
			if(window[fwi] == "Volatility" || processed_words_count == 217611){
				cout << "Hej" << endl;
			}
			Context fContext = dictionary->getContext(window[fwi]);

			// Loop over all words within window (except focus word)
			short wj;
			for(short j = 0; j < 2*k; ++j){
				wj = (win_size + fwi + win_idx[j]) % win_size; // word index (in window)
				IndexVector iv = dictionary->getIndexVector(window[wj]);
				fContext.add(iv, j);
			}

			// Update wi
			wi = (wi + 1) % win_size;
			processed_words_count += 1;
			dictionary->incrementCount(window[fwi]);

			if(processed_words_count % 40000 == 0){
				cout << "\rWords processed: " << processed_words_count <<
						" | Total words: " << dictionary->getNumWords() << " (" << dictionary->getNumWords() * 100 /max_words << "%)" <<
						" | Progress: " << corpus.getProgress()*100 << "%" << flush;
			}

		}
	}catch (const runtime_error& error){
		cout << "\rMaximum number of words exceeded. Stopping.                                " << endl;
	}

	cout << "\rWords processed: " << processed_words_count <<
						" | Total words: " << dictionary->getNumWords() << " (" << dictionary->getNumWords() * 100 /max_words << "%)" <<
						" | Progress: " << corpus.getProgress()*100 << "%" << endl;
	delete[] win_idx;

}
