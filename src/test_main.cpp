#include <iostream>
#include "Dictionary.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include "CpuParser.h"
#include "DummyCorpus.h"
#include "OneBCorpus.h"

using namespace std;
using namespace model;

void testRI(){

	unsigned int num_words = 1;
	int d = 1000;
	int epsilon = 2;
	int k = 1;

	unsigned short* tmp_idx = (unsigned short*) malloc(num_words * epsilon * sizeof(unsigned short));
	tmp_idx[0] = 123;
	tmp_idx[1] = 456;

	int* tmp_ctx = (int*) malloc(num_words*(2*k)*d * sizeof(int));
	memset(tmp_ctx, 0, num_words*(2*k)*d * sizeof(int)); // Init to 0

	Dictionary dict(num_words, d, epsilon, k, tmp_idx, tmp_ctx);

	IndexVector i1 = dict.getIndexVector("hej");
	Context c1 = dict.getContext("hej");

	c1.add(i1, 0);
	c1.add(i1, 1);

	if(i1.getSource()[0] != 123 ||
			i1.getSource()[1] != 456 ||
			i1.getValueAt(1) != -1 ||
			i1.getIndexAt(1) != 228 ||
			c1.get(0, 228) != -1 ||
			c1.get(1, 61) != 1
			)
		throw runtime_error("RItest failed!");
}

void testOneBCorpus(){
	OneBCorpus corpus("/home/tobiasnorlund/Code/Exjobb/corpus/1-billion-word-language-modeling-benchmark-r13output/training-monolingual.tokenized.shuffled/news.en-00001-of-00100");
	string word;
	while(corpus >> word)
		cout << word << endl;
}

int main()
{

	//testRI();
	//testOneBCorpus();

	// Create parser
	CpuParser parser;
	OneBCorpus corpus("/home/tobiasnorlund/Code/Exjobb/corpus/1-billion-word-language-modeling-benchmark-r13output/training-monolingual.tokenized.shuffled/news.en-00001-of-00100");
	int k = 1;
	int d = 100;
	int epsilon = 1;
	unsigned long max_cpu_mem = 1000000000;
	unsigned long max_gpu_mem = 10000000;
	unsigned long max_shared_mem = 1000;

	parser.parse(corpus, k, d, epsilon, max_cpu_mem);

	Dictionary* result = parser.getDictionary();

	return 0;
}
