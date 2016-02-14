extern "C" {
#include "BigUnsigned.h"
}
#include <iostream>
#include <climits>

using namespace std;

//CONSTRUCTION
void testBigUnsignedConstructors(){
	cout << "TESTING INITIATORS." << endl;
  {
    cout << "\tBigUnsigned should be initializable with no parameters." << endl;
    BigUnsigned bu = { 0 };
    if (initBU(&bu))
      cout << "\t\tERROR: Void constructor shouldn't fail!" << endl;
    if (bu.isNULL == true)
      cout << "\t\tERROR: Void constructor shouldn't initialize isNULL to true!" << endl;
    if (bu.len != 0)
      cout << "\t\tERROR: length not 0!" << endl;
    if (bu.cap != 0)
      cout << "\t\tERROR: cap not 0!" << endl;
  }
  {
    cout << "\tBigUnsigned should be initializable with a Blk." << endl;
    BigUnsigned bu = { 0 };
    //printf("%d\n", &bu.blk);
    
   // printf("%d\n", offsetof(BigUnsigned, blk));

    if (initBlkBU(&bu, 5))
      cout << "\t\tERROR: Blk constructor shouldn't fail!" << endl;
    if (bu.isNULL == true)
      cout << "\t\tERROR: Blk constructor shouldn't initialize isNULL to true!" << endl;
    if (bu.len != 1)
      cout << "\t\tERROR: length not 0!" << endl;
    if (bu.cap != 1)
      cout << "\t\tERROR: cap not 0!" << endl;
    
    if (bu.blk[0] != 5)
      cout << "\t\tERROR: Block at 0 does not equal 5!" << endl;
  }
  {
    cout << "\tNULL BigUnsigned should be initializable." << endl;
    BigUnsigned bu = { 0 };
    if (initNULLBU(&bu))
      cout << "\t\tERROR: NULL constructor shouldn't fail!" << endl;
    if (bu.isNULL == false)
      cout << "\t\tERROR: NULL constructor shouldn't initialize isNULL to false!" << endl;
    if (bu.len != 0)
      cout << "\t\tERROR: length not 0!" << endl;
    if (bu.cap != 0)
      cout << "\t\tERROR: cap not 0!" << endl;
  }
  {
    cout << "\tBigUnsigned should be initializable with another BU." << endl;
    BigUnsigned buA = { 0 };
    initBlkBU(&buA, 5);
    BigUnsigned bu = { 0 };
    
    if (initBUBU(&bu, &buA))
    cout << "\t\tERROR: BU constructor shouldn't fail!" << endl;
    if (bu.isNULL != buA.isNULL)
      cout << "\t\tERROR: BU constructor should match its parameter's isNULL value!" << endl;
    if (bu.len != 1)
      cout << "\t\tERROR: length not 1!" << endl;
    if (bu.cap != 1)
      cout << "\t\tERROR: cap not 1!" << endl;
    if (bu.blk[0] != 5)
      cout << "\t\tERROR: Block at 0 does not equal 5!" << endl;
  }
}

//ARITHMETIC OPERATORS
void testBigUnsignedAddition() {
	cout << "TESTING BU ADDITION" << endl;
	cout << "\tAddition of a BigUnsigned should be defined." << endl;
  BigUnsigned result, a, b;
  
  initBlkBU(&a, (pow(2, (sizeof(Blk)* 8)) - 1));
  initBlkBU(&b, 1);
  addBU(&result, &a, &b);
	if (buToString(result).compare("[1][0]") != 0) 
    cout << "\t\tERROR: [" << (pow(2, (sizeof(Blk)* 8)) - 1) << "] + [1] does not equal [1][0]" << endl;
	else 
		cout << "\t\tSUCCESS: [4294967295] + [1] = " << buToString(result) << endl;
}

void testBigUnsignedSubtraction() {
	cout << "TESTING BU SUBTRACTION" << endl;
	cout << "\tBigUnsigned is defined under subtraction." << endl;
  BigUnsigned result, a, b;
  initBU(&a);
  initBlkBU(&b, 1);
  a.len = 2;
	a.blk[0] = 0;
	a.blk[1] = 1;
  subtractBU(&result, &a, &b);
  if (buToString(result).compare("[" + std::to_string((unsigned long)pow(2, (sizeof(Blk)* 8)) - 1) + "]") != 0){
    cout << "\t\tERROR: [1][0] - [1] does not equal [" + std::to_string((unsigned long)pow(2, (sizeof(Blk)* 8)) - 1) + "]" << " using subtractIBU." << endl;
  }
	else 
		cout << "\t\tSUCCESS: [1][0] - [1] = " << buToString(result) <<" using subtractBU." <<endl;
  subtractIBU(&result, &a, 1);
  if (buToString(result).compare("[" + std::to_string((unsigned long)pow(2, (sizeof(Blk)* 8)) - 1) + "]") != 0){
    cout << "\t\tERROR: [1][0] - [1] does not equal [" + std::to_string((unsigned long)pow(2, (sizeof(Blk)* 8)) - 1) + "]" << " using subtractIBU." << endl;
  }
  else
    cout << "\t\tSUCCESS: [1][0] - [1] = " << buToString(result) << " using subtractIBU." << endl;
}

//~~BIT/BLOCK ACCESSORS~~//
void testBigUnsignedBlockAccessors(){
	cout << "TESTING BU BLOCK ACCESSORS AND MUTATORS" << endl;
	cout << "\tA block from within a Big Unsigned should be accessable."<<endl;
	//getBUBlock
  BigUnsigned bu = { 0 };
  initBlkBU(&bu, 1);
	if (getBUBlock(&bu, 0) == 1)
		cout << "\t\tSUCCESS: The first block of createBU(1) is: " << std::to_string(getBUBlock(&bu, 0)) << endl;
	else	
		cout << "\t\tERROR: The first block of createBU(1) does not equal 1" << endl;

	//setBUBlock
	cout << "\tA block from within a BigUnsigned should be mutatable." << endl;
	setBUBlock(&bu, 1, 1);
	if (getBUBlock(&bu, 1) == 1)
    cout << "\t\tSUCCESS: Setting the second block of createBU(1) to 1 yields: " << std::to_string(getBUBlock(&bu, 1)) << endl;
	else
		cout << "\t\tERROR: Setting the second block of createBU(1) does not yield 1" << endl;

  initBlkBU(&bu, 1);
	//getShiftedBUBlock
	cout << "\tA shifted block from within a Big Unsigned should be accessable." << endl;
  if (getShiftedBUBlock(&bu, 0, 1) != 2) {
    cout << "\t\tERROR: getShiftedBUBlock(bu, 0, -1) of the BU [1] does not equal 2" << endl;
    cout<<std::to_string(getShiftedBUBlock(&bu, 0, -1)) << endl;
  }
	else
		cout << "\t\tSUCCESS: getShiftedBUBlock(bu, 0, -1) of the BU[1][1] equals 2147483648" << endl;
}
void testBigUnsignedBitAccessors(){
	cout << "TESTING BU BIT ACCESSORS AND MUTATORS" << endl;
  BigUnsigned bu = { 0 };
  initBlkBU(&bu, 7);

	//getBUBitLength
	cout << "\tThe number of bits used within a BU should be accessable." << endl;
	if (getBUBitLength(&bu) != 3)
		cout << "\t\tERROR: BU [7] does not return 3 bits in length." << endl;
	else
		cout << "\t\tSUCCESS: BU [7] returns 3 bits in length." << endl;
	//getBUBit
	cout << "\tA bit within a BU should be accessable." << endl;
	if (getBUBit(&bu, 0) != 1)
		cout << "\t\tERROR: BU [7] does not yield 1 at bit index 0." <<endl;
	else
		cout << "\t\tSUCCESS: BU [7] yields 1 at bit index 0." << endl;
	//setBUBit
	cout << "\tA bit within a BU should be mutatable." << endl;
	setBUBit(&bu, 0, 0);
	if (getBUBit(&bu, 0) != 0)
		cout << "\t\tERROR: BU [7] does not yield 0 at the modified bit at index 0." << endl;
	else
		cout << "\t\tSUCCESS: BU [7] yields 0 at the modified bit at index 0." << endl;
}


//BITWISE OPERATORS
void testBigUnsignedAnd(){
	cout << "TESTING AND" << endl;
	cout << "\tBU's should support bitwise AND." << endl;
  BigUnsigned x = { 0 };
  initBlkBU(&x, 12);
	x.len++;
	x.blk[1] = 12;
  BigUnsigned y = { 0 };
  initBlkBU(&y,9);
	y.len++;
	y.blk[1] = 9;
  BigUnsigned result = { 0 };
  andBU(&result, &x, &y);
	if (buToString(result).compare("[8][8]") != 0)
		cout << "\t\tERROR: [12][12] & [9][9] != [8][8]" << endl;
	else
		cout << "\t\tSUCCESS: [12][12] & [9][9] = [8][8]" << endl;
}
void testBigUnsignedOr(){
	cout << "TESTING OR" << endl;
	cout << "\tBU's should support bitwise OR." << endl;
  BigUnsigned x = { 0 };
  initBlkBU(&x, 12);
	x.len++;
	x.blk[1] = 12;
  BigUnsigned y = { 0 };
  initBlkBU(&y, 9);
	y.len++;
	y.blk[1] = 9;
  BigUnsigned result = { 0 };
  orBU(&result, &x, &y);
	if (buToString(result).compare("[13][13]") != 0)
		cout << "\t\tERROR: [12][12] | [9][9] != [13][13]" << endl;
	else
		cout << "\t\tSUCCESS: [12][12] | [9][9] = [13][13]" << endl;
}
void testBigUnsignedXOr(){
	cout << "TESTING XOR" << endl;
	cout << "\tBU's should support bitwise XOR." << endl;
  BigUnsigned x = { 0 };
  initBlkBU(&x,12);
	x.len++;
	x.blk[1] = 12;
  BigUnsigned y = { 0 };
  initBlkBU(&y, 9);
	y.len++;
	y.blk[1] = 9;
  BigUnsigned result = { 0 };
  xOrBU(&result, &x, &y);
	if (buToString(result).compare("[5][5]") != 0)
		cout << "\t\tERROR: [12][12] ^ [9][9] != [5][5]" << endl;
	else
		cout << "\t\tSUCCESS: [12][12] ^ [9][9] = [5][5]" << endl;
}
void testBigUnsignedShiftLeft(){
	cout << "TESTING SHIFT LEFT" << endl;
	cout << "\tBU's should support bitwise left shift." << endl;
  BigUnsigned x = { 0 };
  initBlkBU(&x,2);
  BigUnsigned result = { 0 };
  shiftBULeft(&result, &x, 1);
	if (buToString(result).compare("[4]") != 0)
		cout << "\t\tERROR: [2] << [1] != [4]" << endl;
	else
		cout << "\t\tSUCCESS: [2] << [1] = [4]" << endl;
}
void testBigUnsignedShiftRight(){
	cout << "TESTING SHIFT RIGHT" << endl;
	cout << "\tBU's should support bitwise right shift." << endl;
  BigUnsigned x = { 0 };
  initBlkBU(&x, 2);
  BigUnsigned result = { 0 };
  shiftBURight(&result, &x, 1);
	if (buToString(result).compare("[1]") != 0)
		cout << "\t\tERROR: [2] >> [1] != [1]" << endl;
	else
		cout << "\t\tSUCCESS: [2] >> [1] = [1]" << endl;
}

//COMPARISON
void testBigUnsignedComparison(){
	cout << "TESTING COMPARISON" << endl;
  BigUnsigned x = { 0 };
  initBlkBU(&x, 1);
  BigUnsigned y = { 0 };
  initBlkBU(&y, 1);
	y.len++;
	y.blk[1] = 1;
  BigUnsigned z = { 0 };
  initBlkBU(&z, 1);
  BigUnsigned w = { 0 };
  initBlkBU(&w, 0);

	//equal
	cout << "\tA BU can be equivalent to another BU." << endl;
	if (compareBU(&x, &z) != 0)
		cout << "\t\tERROR: [1] == [1] = false" << endl;
	else
		cout << "\t\tSUCCESS: [1] == [1] = true" << endl;
	//less than
	cout << "\tA BU can be less than another BU." << endl;
	if (compareBU(&x, &y) != -1)
		cout << "\t\tERROR: [1] < [1][1] = false" << endl;
	else
		cout << "\t\tSUCCESS: [1] < [1][1] = true" << endl;
	//greater than
	cout << "\tA BU can be greater than another BU." << endl;
	if (compareBU(&x, &w) != 1)
		cout << "\t\tERROR: [1] > [0] = false" << endl;
	else
		cout << "\t\tSUCCESS: [1] > [0] = true" << endl;
}


inline void testBigUnsigned() {
	using namespace std;
	testBigUnsignedConstructors();
	
  testBigUnsignedBlockAccessors();
  testBigUnsignedBitAccessors();

  testBigUnsignedAddition();
	testBigUnsignedSubtraction();
  testBigUnsignedAnd();
	testBigUnsignedOr();
	testBigUnsignedXOr();
	testBigUnsignedShiftLeft();
	testBigUnsignedShiftRight();
	testBigUnsignedComparison();
}