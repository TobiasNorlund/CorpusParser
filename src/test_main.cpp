#include <iostream>
#include "Dictionary.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include "CpuParser.h"
#include "DummyCorpus.h"
#include "OneBCorpus.h"
#include <algorithm>

using namespace std;
using namespace model;

void testRI(){

	unsigned int max_words = 1;
	int d = 1000;
	int epsilon = 2;
	int k = 1;

	unsigned short* tmp_idx = (unsigned short*) malloc(max_words * epsilon * sizeof(unsigned short));
	tmp_idx[0] = 123;
	tmp_idx[1] = 456;

	//int* tmp_ctx = (int*) malloc(num_words*(2*k)*d * sizeof(int));
	//memset(tmp_ctx, 0, num_words*(2*k)*d * sizeof(int)); // Init to 0

	Dictionary dict(max_words, max_words, d, epsilon, k, "", tmp_idx);
	dict.initPass();

	IndexVector i1 = dict.getIndexVector("hej");
	Context* c1 = dict.getContext("hej");

	c1->add(i1, 0);
	c1->add(i1, 1);

	if(i1.getSource()[0] != 123 ||
			i1.getSource()[1] != 456 ||
			i1.getValueAt(1) != -1 ||
			i1.getIndexAt(1) != 228 ||
			c1->get(0, 228) != -1 ||
			c1->get(1, 61) != 1
			){
		delete c1;
		delete tmp_idx;

		throw runtime_error("RItest failed!");
	}

}

void testOneBCorpus(){
	OneBCorpus corpus("/home/tobiasnorlund/Code/Exjobb/corpus/1-billion-word-language-modeling-benchmark-r13output/training-monolingual.tokenized.shuffled/news.en-00001-of-00100");
	string word;
	while(corpus >> word)
		cout << word << endl;
}

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return find(begin, end, option) != end;
}

Context* temp(){
	throw runtime_error("tewt");
}

int main(int argc, char** argv)
{
	testRI();
	//testOneBCorpus();

	// Parse command line arguments
	Corpus* corpus;
	if (cmdOptionExists(argv,argv+argc,"-corpus") && getCmdOption(argv,argv+argc, (string)"-corpus") == (string)"OneBCorpus")
		corpus = new OneBCorpus("/home/tobiasnorlund/Code/Exjobb/corpus/1-billion-word-language-modeling-benchmark-r13output/training-monolingual.tokenized.shuffled/");
	else
		throw runtime_error("A valid corpus must be given.");

	int k = (cmdOptionExists(argv,argv+argc,"-k")) ? atoi(getCmdOption(argv,argv+argc,"-k")) : 2 ;
	int d = (cmdOptionExists(argv,argv+argc,"-d")) ? atoi(getCmdOption(argv,argv+argc,"-d")) : 2000;
	int epsilon = (cmdOptionExists(argv,argv+argc,"-e")) ? atoi(getCmdOption(argv,argv+argc,"-e")) : 10;
	unsigned int max_words = (cmdOptionExists(argv,argv+argc,"-w")) ? atoi(getCmdOption(argv,argv+argc,"-w")) : 3000000 ;
	unsigned long max_cpu_mem = (cmdOptionExists(argv,argv+argc,"-mem")) ? atol(getCmdOption(argv,argv+argc,"-mem")) : 1000000000L;

	// Create parser
	CpuParser parser;

	parser.parse(*corpus, k, d, epsilon, max_cpu_mem, max_words, "/media/tobiasnorlund/ac861917-9ad7-4905-93e9-ee6ab16360ad/bigdata/Dump/" + corpus->toString());

	cout << "Parse complete!" << endl;
	//cout << "Saving ...";

	//Dictionary* result = parser.getDictionary();
	//result->save("/home/tobiasnorlund/Code/Dump/", corpus->toString());

	delete corpus;

	cout << "\rDone!           " << endl;

	return 0;
}
