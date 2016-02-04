#include "../bigint/BigUnsigned.c"
#include <iostream>
using namespace std;

//CONSTRUCTION
void testBigUnsignedConstructors(){
	cout << "TESTING CONSTRUCTORS. (If no errors are thrown here, SUCCESS is implied.)" << endl;
	cout << "\tBigUnsigned should be constructed with no parameters." << endl;
	createBU();
	struct BigUnsigned bu = createBU();
	if (bu.len != 0)
		cout << "\t\tERROR: length not 0!" << endl;
	if (bu.cap != 0)
		cout << "\t\tERROR: cap not 0!" << endl;
	if (numBUBits != 8 * sizeof(Blk))
		cout << "\t\tERROR: N does not equal " << 8 * sizeof(Blk) << endl;

	cout << "\tBigUnsigned should be constructed with a short." << endl;
	short x = 10;
	bu = createSBU(x);
	if (bu.len != 1)
		cout << "\tERROR: length not 0!" << endl;
	if (bu.cap != 1)
		cout << "\t\tERROR: cap not 0!" << endl;
	if (numBUBits != 8 * sizeof(Blk))
		cout << "\t\tERROR: N does not equal " << 8 * sizeof(Blk) << endl;

	cout << "\tBigUnsigned should be constructed with a int." << endl;
	bu = createIBU((int)x);
	if (bu.len != 1)
		cout << "\t\tERROR: length not 0!" << endl;
	if (bu.cap != 1)
		cout << "\t\tERROR: cap not 0!" << endl;
	if (numBUBits != 8 * sizeof(Blk))
		cout << "\t\tERROR: N does not equal " << 8 * sizeof(Blk) << endl;


	cout << "\tBigUnsigned should be constructed with a long." << endl;
	bu = createLBU((long)x);
	if (bu.len != 1)
		cout << "\t\tERROR: length not 0!" << endl;
	if (bu.cap != 1)
		cout << "\t\tERROR: cap not 0!" << endl;
	if (numBUBits != 8 * sizeof(Blk))
		cout << "\t\tERROR: N does not equal " << 8 * sizeof(Blk) << endl;

	cout << "\tBigUnsigned should be constructed with an unsigned short." << endl;
	bu = createUSBU((unsigned short)x);
	if (bu.len != 1)
		cout << "\t\tERROR: length not 0!" << endl;
	if (bu.cap != 1)
		cout << "\t\tERROR: cap not 0!" << endl;
	if (numBUBits != 8 * sizeof(Blk))
		cout << "\t\tERROR: N does not equal " << 8 * sizeof(Blk) << endl;

	cout << "\tBigUnsigned should be constructed with an unsigned int." << endl;
	bu = createUIBU((unsigned int)x);
	if (bu.len != 1)
		cout << "\t\tERROR: length not 0!" << endl;
	if (bu.cap != 1)
		cout << "\t\tERROR: cap not 0!" << endl;
	if (numBUBits != 8 * sizeof(Blk))
		cout << "\t\tERROR: N does not equal " << 8 * sizeof(Blk) << endl;

	cout << "\tBigUnsigned should be constructed with an unsigned long." << endl;
	bu = createULBU((unsigned long)x);
	if (bu.len != 1)
		cout << "\t\tERROR: length not 0!" << endl;
	if (bu.cap != 1)
		cout << "\t\tERROR: cap not 0!" << endl;
	if (numBUBits != 8 * sizeof(Blk))
		cout << "\t\tERROR: N does not equal " << 8 * sizeof(Blk) << endl;
}

//ARITHMETIC OPERATORS
void testBigUnsignedAddition() {
	cout << "TESTING BU ADDITION" << endl;
	cout << "\tAddition of a BigUnsigned should be defined." << endl;
	struct BigUnsigned result = addBU(&createULBU((unsigned long)(4294967295)), &createIBU(1));	
	if (buToString(&result).compare("[1][0]") != 0) 
		cout << "\t\tERROR: [4294967295] + [1] does not equal [1][0]" << endl;
	else 
		cout << "\t\tSUCCESS: [4294967295] + [1] = " << buToString(&result) << endl;
}
void testBigUnsignedSubtraction() {
	cout << "TESTING BU SUBTRACTION" << endl;
	cout << "\tBigUnsigned is defined under subtraction." << endl;
	struct BigUnsigned bigNumber = createBU();
	bigNumber.len = 2;
	bigNumber.blk[0] = 0;
	bigNumber.blk[1] = 1;

	struct BigUnsigned result = subtractBU(&bigNumber, &createIBU(1));
	if (buToString(&result).compare("[4294967295]") != 0)
		cout << "\t\tERROR: [1][0] - [1] does not equal [4294967295]" << endl;
	else 
		cout << "\t\tSUCCESS: [1][0] - [1] = " << buToString(&result) << endl;
}

//~~BIT/BLOCK ACCESSORS~~//
void testBigUnsignedBlockAccessors(){
	cout << "TESTING BU BLOCK ACCESSORS AND MUTATORS" << endl;
	cout << "\tA block from within a Big Unsigned should be accessable."<<endl;
	//getBUBlock
	struct BigUnsigned bu = createIBU(1);
	if (getBUBlock(&bu, 0) == 1)
		cout << "\t\tSUCCESS: The first block of createBU(1) is: " << getBUBlock(&bu, 0) << endl;
	else	
		cout << "\t\tERROR: The first block of createBU(1) does not equal 1" << endl;

	//setBUBlock
	cout << "\tA block from within a BigUnsigned should be mutatable." << endl;
	setBUBlock(&bu, 1, 1);
	if (getBUBlock(&bu, 1) == 1)
		cout << "\t\tSUCCESS: Setting the second block of createBU(1) to 1 yields: " << getBUBlock(&bu, 1) << endl;
	else
		cout << "\t\tERROR: Setting the second block of createBU(1) does not yield 1" << endl;

	//getShiftedBUBlock
	cout << "\tA shifted block from within a Big Unsigned should be accessable." << endl;
	if (getShiftedBUBlock(&bu, 0, -1) != 2147483648)
		cout << "\t\tERROR: getShiftedBUBlock(bu, 0, -1) of the BU [1][1] does not equal 2147483648" << endl;
	else
		cout << "\t\tSUCCESS: getShiftedBUBlock(bu, 0, -1) of the BU[1][1] equals 2147483648" << endl;
}
void testBigUnsignedBitAccessors(){
	cout << "TESTING BU BIT ACCESSORS AND MUTATORS" << endl;
	struct BigUnsigned bu = createIBU(7);

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
	struct BigUnsigned x = createIBU(12);
	x.len++;
	x.blk[1] = 12;
	struct BigUnsigned y = createIBU(9);
	y.len++;
	y.blk[1] = 9;
	if (buToString(&andBU(&x, &y)).compare("[8][8]") != 0)
		cout << "\t\tERROR: [12][12] & [9][9] != [8][8]" << endl;
	else
		cout << "\t\tSUCCESS: [12][12] & [9][9] = [8][8]" << endl;
}
void testBigUnsignedOr(){
	cout << "TESTING OR" << endl;
	cout << "\tBU's should support bitwise OR." << endl;
	struct BigUnsigned x = createIBU(12);
	x.len++;
	x.blk[1] = 12;
	struct BigUnsigned y = createIBU(9);
	y.len++;
	y.blk[1] = 9;
	if (buToString(&orBU(&x, &y)).compare("[13][13]") != 0)
		cout << "\t\tERROR: [12][12] | [9][9] != [13][13]" << endl;
	else
		cout << "\t\tSUCCESS: [12][12] | [9][9] = [13][13]" << endl;
}
void testBigUnsignedXOr(){
	cout << "TESTING XOR" << endl;
	cout << "\tBU's should support bitwise XOR." << endl;
	struct BigUnsigned x = createIBU(12);
	x.len++;
	x.blk[1] = 12;
	struct BigUnsigned y = createIBU(9);
	y.len++;
	y.blk[1] = 9;
	if (buToString(&xOrBU(&x, &y)).compare("[5][5]") != 0)
		cout << "\t\tERROR: [12][12] ^ [9][9] != [5][5]" << endl;
	else
		cout << "\t\tSUCCESS: [12][12] ^ [9][9] = [5][5]" << endl;
}
void testBigUnsignedShiftLeft(){
	cout << "TESTING SHIFT LEFT" << endl;
	cout << "\tBU's should support bitwise left shift." << endl;
	struct BigUnsigned x = createIBU(2);
	if (buToString(&shiftBULeft(&x, 1)).compare("[4]") != 0)
		cout << "\t\tERROR: [2] << [1] != [4]" << endl;
	else
		cout << "\t\tSUCCESS: [2] << [1] = [4]" << endl;
}
void testBigUnsignedShiftRight(){
	cout << "TESTING SHIFT RIGHT" << endl;
	cout << "\tBU's should support bitwise right shift." << endl;
	struct BigUnsigned x = createIBU(2);
	if (buToString(&shiftBURight(&x, 1)).compare("[1]") != 0)
		cout << "\t\tERROR: [2] >> [1] != [1]" << endl;
	else
		cout << "\t\tSUCCESS: [2] >> [1] = [1]" << endl;
}

//COMPARISON
void testBigUnsignedComparison(){
	cout << "TESTING COMPARISON" << endl;
	struct BigUnsigned x = createIBU(1);
	struct BigUnsigned y = createIBU(1);
	y.len++;
	y.blk[1] = 1;
	struct BigUnsigned z = createIBU(1);
	struct BigUnsigned w = createIBU(0);

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
	testBigUnsignedAddition();
	testBigUnsignedSubtraction();
	testBigUnsignedAnd();
	testBigUnsignedOr();
	testBigUnsignedXOr();
	testBigUnsignedShiftLeft();
	testBigUnsignedShiftRight();
	testBigUnsignedBlockAccessors();
	testBigUnsignedBitAccessors();
	testBigUnsignedComparison();
}