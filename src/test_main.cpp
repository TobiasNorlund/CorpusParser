#include <iostream>
#include "Dictionary.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include "CpuParser.h"
#include "DummyCorpus.h"
#include "WikipediaCorpus.h"
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

	Dictionary dict(max_words, max_words, d, epsilon, k, "", tmp_idx);
	dict.initPass();

	IndexVector i1 = dict.getIndexVector("hej");
	Context* c1 = dict.getContext("hej");

	c1->add(i1, 0, 1);
	c1->add(i1, 1, 1);

	if(i1.getSource()[0] != 123 ||
			i1.getSource()[1] != 456 ||
			i1.getValueAt(1) != -1 ||
			i1.getIndexAt(1) != 228 ||
			c1->get(0, 228) != -1 || // Dangerous !!
			c1->get(1, 61) != 1
			){
		delete c1;
		delete tmp_idx;

		throw runtime_error("RItest failed!");
	}

}

void testCpuParser(){

	string dump_path = "/home/tobiasnorlund/Code/Dump/ParserTest";
	unsigned int max_words = 2;
	int d = 1000;
	int epsilon = 1;
	int k = 1;
	float c = 1;
	unsigned int max_cpu_mem = 30000;
	unsigned int max_words_per_pass = (max_cpu_mem - sizeof(short)*epsilon*max_words) / (sizeof(float)*d * k*2);

	unsigned short* tmp_idx = (unsigned short*) malloc(max_words * epsilon * sizeof(unsigned short));
	tmp_idx[0] = 123; // 1 at index 61
	tmp_idx[1] = 456; // -1  at index 228
	tmp_idx[2] = 789; // 1 at index 394

	DummyCorpus corpus;
	Dictionary* dict = new Dictionary(max_words, max_words_per_pass, d, epsilon, k, dump_path, tmp_idx);

	CpuParser parser(dict);
	parser.parse(corpus, k, d, epsilon, c, max_cpu_mem, max_words, dump_path);

	if(dict->getNumWords() != 2)
		throw runtime_error("Test Parser Error: Not correct number of words in corpus");

	Context* cCat = dict->getContext("cat");
	if(cCat->get(0, 61) <= 0 ||
	   cCat->get(1, 394) != 0)
		throw runtime_error("Test Parser Error: Bad value in context");

	delete dict;
}

void testWikiParser(){
	WikipediaCorpus corpus("/bigdata/public/wikipedia/en/wiki.2010.txt");

	cout << "Reads first pass..." << endl;
	string line = "";
	unsigned long i = 0;
	while(corpus.read_line(line)){
		corpus.read_line(line);
		if(i++ % 10000000 == 0)
			cout << "\r" + to_string(corpus.getProgress());
	}

	cout << "Resets" << endl;

	corpus.reset();

	for(int i = 0; i<=5; ++i){
		corpus.read_line(line);
		cout << line << endl;
	}
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

int main(int argc, char** argv)
{
	//testRI();
	//testOneBCorpus();
	//testCpuParser();
	//testWikiParser();

	// Parse command line arguments
	Corpus* corpus;
	if (cmdOptionExists(argv,argv+argc,"-corpus") && getCmdOption(argv,argv+argc, (string)"-corpus") == (string)"OneBCorpus")
		corpus = new OneBCorpus("/home/tobiasnorlund/Code/Exjobb/corpus/1-billion-word-language-modeling-benchmark-r13output/training-monolingual.tokenized.shuffled/");
	else if(cmdOptionExists(argv,argv+argc,"-corpus") && getCmdOption(argv,argv+argc, (string)"-corpus") == (string)"Wikipedia")
		corpus = new WikipediaCorpus("/home/tobiasnorlund/Corpus/wiki.2010.txt");
	else if(cmdOptionExists(argv,argv+argc,"-corpus") && getCmdOption(argv,argv+argc, (string)"-corpus") == (string)"DummyCorpus")
		corpus = new DummyCorpus();
	else
		throw runtime_error("A valid corpus must be given.");

	int k = (cmdOptionExists(argv,argv+argc,"-k")) ? atoi(getCmdOption(argv,argv+argc,"-k")) : 2 ;
	int d = (cmdOptionExists(argv,argv+argc,"-d")) ? atoi(getCmdOption(argv,argv+argc,"-d")) : 2000;
	int epsilon = (cmdOptionExists(argv,argv+argc,"-e")) ? atoi(getCmdOption(argv,argv+argc,"-e")) : 10;
	int c = (cmdOptionExists(argv,argv+argc,"-c")) ? atoi(getCmdOption(argv,argv+argc,"-c")) : -5;
	unsigned int max_words = (cmdOptionExists(argv,argv+argc,"-w")) ? atoi(getCmdOption(argv,argv+argc,"-w")) : 3000000 ;
	unsigned long max_cpu_mem = (cmdOptionExists(argv,argv+argc,"-mem")) ? atol(getCmdOption(argv,argv+argc,"-mem")) : 1000000000L;

	// Create parser
	CpuParser parser;
	parser.parse(*corpus, k, d, epsilon, c, max_cpu_mem, max_words, "/media/tobiasnorlund/ac861917-9ad7-4905-93e9-ee6ab16360ad/bigdata/Dump/" + corpus->toString());//"/home/tobiasnorlund/Code/Dump/" + corpus->toString());//

	cout << "Parse complete!" << endl;

	delete corpus;

	cout << "\rDone!           " << endl;

	return 0;
}
