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
#include <sstream>
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

float CpuParser::getWordWeight(string word, float c){
	return exp(c * dictionary->getCount(word) / dictionary->getNumWords());
}

void CpuParser::parse(Corpus& corpus, int k, int d, int epsilon, float c, unsigned long max_cpu_mem, unsigned int max_words, string dump_path){

	size_t win_size = 2*k+1;

	// Calculate how to distribute this job
	int max_words_per_pass = (max_cpu_mem - sizeof(short)*epsilon*max_words) / (sizeof(float)*d * k*2);
	if(max_words_per_pass < 0) throw runtime_error("Max memory allowed too low");
	short passes_needed = 0;

	if(!dictSpecified)
		this->dictionary = new Dictionary(max_words, max_words_per_pass, d, epsilon, k, dump_path);

	// Begin parse !
	cout << endl;
	cout << "Beginning parse of corpus: " << corpus.toString() << endl << endl;
	cout << " Maximum unique words: " << max_words << endl;
	cout << " Maximum unique words per pass: " << max_words_per_pass << endl;
	cout << " Total memory allowed: " << max_cpu_mem << " bytes" << endl;
	cout << " Window size: " << k << " + " << k << endl;
	cout << " Word embedding dimensionality: " << d << endl;
	cout << " Non-zero elements in index vectors: " << epsilon << endl;
	cout << " Constant in weight function: " << c << endl;
	cout << endl;

	// Read in first window
	vector<string> window(win_size);
	char* win_idx = new char[2*k];
	for(short i=0;i<2*k;++i) win_idx[i] = (i<k)?i-k:i-k+1;

	int wi;
	unsigned long processed_words_count = 2*k;
	string line = "";

	// Iterate through all words in corpus
	short pass = 1;
	do{ // Loop over the dataset in multiple passes

		dictionary->initPass();

		// Read a line (paragraph)
		while(corpus.read_line(line)){

			// Split line to words
			vector<string> words = split(line, ' ');
			//print_window(words);
			int li = 0; // Line index
			wi = 0;

			for(li; li<words.size()+k; ++li){

				if(li<words.size()){
					window[wi] = words[li];
					//cout << window[wi] << endl;

					// Add word to dictionary if unseen and increment count in first pass
					if(dictionary->newWord(window[wi]) && pass == 1)
						dictionary->incrementCount(window[wi]);
				}

				// Determine the focus word
				int fwi = (win_size + wi-k) % win_size; // Focus word index (in window)
				Context* fContext;
				//cout << "focus: " << window[fwi] << endl;

				// Only process focus word if in dictionary, if in this pass and if focus word is in sentence (not in the first k iterations), otherwise skip this iteration
				if(dictionary->newWord(window[fwi]) && dictionary->getPass(window[fwi]) == pass && li-k >=0){
					fContext = dictionary->getContext(window[fwi]);

					// Loop over all words within window (except focus word)
					short wj;
					for(short j = 0; j < 2*k; ++j){
						wj = (win_size + fwi + win_idx[j]) % win_size; // word index (in window)
						if(dictionary->newWord(window[wj]) && (li-k) + win_idx[j] >= 0 && (li-k) + win_idx[j] < words.size()){ // If word has an index vector and window word is within line
							//cout << window[wj] << endl;
							IndexVector iv = dictionary->getIndexVector(window[wj]);
							fContext->add(iv, j, getWordWeight(window[wj], c));
						}
					}
					delete fContext;
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
		}

		cout << "\rEnding pass " << pass << " ...                                                            " << flush;

		dictionary->endPass();
		corpus.reset();

		passes_needed = ceil((float)dictionary->getNumWords() / max_words_per_pass);

	}while(++pass <= passes_needed);

	string passes_str = to_string(pass); if(passes_needed != 0) passes_str += " (of " + to_string(passes_needed) + ")";
	cout << "\rPass: " << passes_str <<
			" | Total words: " << dictionary->getNumWords() << " (" << dictionary->getNumWords() * 100 /max_words << "%)" <<
			" | Progress: " << corpus.getProgress()*100 << "%" << flush;

	delete[] win_idx;

}

vector<string> CpuParser::split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}
