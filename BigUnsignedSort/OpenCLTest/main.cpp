//Following https://anteru.net/2012/11/03/2009/

#include <utility>
#include <CL\cl.hpp>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <bitset>
#include <chrono>

#include "CLWrapper.h"

#include "bigint\BigUnsigned.h"
#include "bigint\BigUnsigned_UnitTest.h"

#include <math.h>
#include <algorithm>

using namespace std;
std::vector<BigUnsigned> input;
int main()
{
  //Uncomment to test big unsigned.
  //testBigUnsigned();

	// Initialize OpenCL
  int numBits = 8;
  long numElements = 1 << numBits;
  CLWrapper CL(numElements, 256);

  //Generate big unsigned numbers.
	input.resize(numElements);
	for (long i = 0; i < numElements; ++i)
    initLongLongBU(&input[i], numElements-i);
  std::random_shuffle(input.begin(), input.end());

  cout << "Unsorted" << endl;
  for (int i = 0; i < numElements; ++i){
   cout << buToString(input[i]) << endl;
  }

  //Sort them.
	CL.RadixSort(input, 32);

  //Print them.
  cout << "Sorted" << endl;
  for (int i = 0; i < numElements; ++i){
    cout<<buToString(input[i])<<endl;
  }
  cout << "Finished!" << endl;
  getchar();
}