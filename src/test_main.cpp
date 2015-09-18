#include <iostream>
#include "Dictionary.h"
#include <cstdlib>
#include <stdexcept>

using namespace std;
using namespace model;

void testRI(){

	unsigned int num_words = 1;
	int d = 100;
	int epsilon = 2;
	int k = 2;

	short** tmp_idx = new short*[1];
	tmp_idx[0] = new short[2];
	tmp_idx[0][0] = 123;
	tmp_idx[0][1] = 456;

	int*** tmp_ctx = new int**[num_words];
	for (unsigned int i = 0; i < num_words; ++i){
		tmp_ctx[i] = new int*[2*k];
		for (short j = 0; j < 2*k; ++j){
			tmp_ctx[i][j] = new int[d];
			for (short n = 0; n < d; ++n)
				tmp_ctx[i][j][n] = 0;
		}
	}

	Dictionary dict(num_words, d, epsilon, k, tmp_idx, tmp_ctx);

	IndexVector i1 = dict.getIndexVector("hej");

	if(i1.getSource()[0] != 123 || i1.getSource()[1] != 456)
		throw runtime_error("RItest failed!");
}

int main()
{

	//cout << "Hello" << endl;

	testRI();

	cout << "Hello" << endl;
	return 0;
}
