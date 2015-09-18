#include <iostream>
#include "Dictionary.h"
#include <cstdlib>

using namespace std;
using namespace model;

bool testRI(){

	unsigned int num_words = 1;
	int d = 100;
	int epsilon = 2;
	int k = 2;

	short tmp_idx[1] = { 123 };

	Dictionary dict(num_words, d, epsilon, k);

	IndexVector i1 = dict.getIndexVector("hej");
	//IndexVector i2 = dict.getIndexVector("tja");

	// Print it!
	for (unsigned int i = 0; i < epsilon+10000; ++i)
	    cout << i1.getSource()[i] << " ";
	//cout << i2 << endl;
}
#pragma optimize( "", off )

int main()
{

	//cout << "Hello" << endl;

	//testRI();
	int n = 10000000;
	short test_arr[n];

	cout << test_arr[n] << endl;

	//for (unsigned int i = 0; i < 1000; ++i)
	/*unsigned int i = 0;
	while(true){
		try{
			test_arr[i++];
		}catch(...){
			cout << endl << "error" << endl;
			break;
		}
	}*/

	//delete[] test_arr;

	//cout << "Hello" << endl;
	return 0;
}
