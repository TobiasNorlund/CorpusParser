
#include "GpuParser.h"
#include "Corpus.h"
#include "Dictionary.h"
#include <iostream>

using namespace model;
using namespace std;

GpuParser::GpuParser(){

}

GpuParser::GpuParser(Dictionary* dictionary){
	this->dictionary = dictionary;
	this->dictSpecified = true;
}

GpuParser::~GpuParser(){
	if(!dictSpecified)
		delete dictionary;
}

Dictionary* GpuParser::getDictionary(){
	return dictionary;
}


void GpuParser::parse(Corpus& corpus, int k, int d, int epsilon, unsigned long max_cpu_mem, unsigned long max_gpu_mem, unsigned long max_shared_mem){

	/*int win_size = 2*k+1;

	// Calculate how to distribute this job
	unsigned int max_words = max_cpu_mem / (sizeof(short)*epsilon + sizeof(int)*d * k*2);
	unsigned int batch_size = max_gpu_mem / (sizeof(short)*epsilon + sizeof(short)*d * k*2);

	if(!dictSpecified)
		this->dictionary = new Dictionary(max_words, d, epsilon, k);

*/


}
