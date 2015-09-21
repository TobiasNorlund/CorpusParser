#include <iostream>
#include "Dictionary.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include "GpuParser.h"
#include "DummyCorpus.h"

using namespace std;
using namespace model;

void testRI(){

	unsigned int num_words = 1;
	int d = 100;
	int epsilon = 2;
	int k = 2;

	short* tmp_idx = (short*) malloc(num_words * epsilon * sizeof(short));
	tmp_idx[0] = 123;
	tmp_idx[1] = 456;

	int* tmp_ctx = (int*) malloc(num_words*(2*k)*d * sizeof(int));
	memset(tmp_ctx, 0, num_words*(2*k)*d * sizeof(int)); // Init to 0

	Dictionary dict(num_words, d, epsilon, k, tmp_idx, tmp_ctx);

	IndexVector i1 = dict.getIndexVector("hej");

	if(i1.getSource()[0] != 123 || i1.getSource()[1] != 456)
		throw runtime_error("RItest failed!");
}

int main()
{

	testRI();

	// Create parser
	/*GpuParser parser;
	DummyCorpus corpus;
	int k = 1;
	int d = 5;
	int epsilon = 1;
	unsigned long max_cpu_mem = 100000000;
	unsigned long max_gpu_mem = 10000000;
	unsigned long max_shared_mem = 1000;

	parser.parse(corpus, k, d, epsilon, max_cpu_mem, max_gpu_mem, max_shared_mem);

	Dictionary* result = parser.getDictionary();

	delete result;*/

	cout << "Hello" << endl;
	return 0;
}
