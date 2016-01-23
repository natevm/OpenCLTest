#ifndef BIGUNSIGNED_C
#define BIGUNSIGNED_C

//#include <stdexcept>

#define BIG_INTEGER_STATIC
#define BIG_INTEGER_SIZE 100

typedef unsigned int Index; // Type for the index of a block in the array
typedef unsigned long Blk;  // Type for the blocks
#ifndef __cplusplus
typedef int bool;
#endif
#define true 1
#define false 0

// Enumeration for result of a comparison.
//enum CmpRes { less = -1, equal = 0, greater = 1 } ;     

// Make sure we have NULL.
#ifndef NULL
#define NULL 0
#endif
const unsigned int N = 8 * sizeof(Blk);

// struct BigUnsigned allows storing integers larger than a long using an array of blk.
struct BigUnsigned {
  //~~ NumberlikeArray Fields ~~//
	/*static*/ unsigned int N;  // Number of bits in a block.
  Index cap;                                      // Current allocated capacity (in blocks)
  Index len;                                      // Actual length of the value stored (in blocks)
  #ifdef BIG_INTEGER_STATIC                       // Heap-allocated array of the blocks (can be NULL if len == 0)
	Blk blk[BIG_INTEGER_SIZE];
  #else
	Blk *blk;
  #endif

  //~~ BigUnsigned Fields  ~~//
};

//~~HELPER FUNCTIONS~~//
// Decreases len to eliminate any leading zero blocks.
void zapLeadingZeros(struct BigUnsigned * bu) {
  while (bu->len > 0 && bu->blk[bu->len - 1] == 0)
    bu->len--;
}
int isBUZero(struct BigUnsigned *bu) { return bu->len == 0; }
void allocateBU(struct BigUnsigned *bu, Index c) {
  // If the requested capacity is more than the current capacity...
  #ifdef BIG_INTEGER_STATIC
    //if (c > BIG_INTEGER_SIZE) throw std::logic_error("BIG_INTEGER_SIZE too small");
  #endif

  if (c > bu->cap) {
    // Delete the old number array
    #ifndef BIG_INTEGER_STATIC
      delete [] bu->blk;
    #endif
      // Allocate the new array
      bu->cap = c;
    #ifndef BIG_INTEGER_STATIC
      bu->blk = new Blk[bu->cap];
    #endif
  }
}
void allocateBUAndCopy(struct BigUnsigned *x, Index c) {
	// If the requested capacity is more than the current capacity...
#ifdef BIG_INTEGER_STATIC
	//if (c > BIG_INTEGER_SIZE) throw std::logic_error("BIG_INTEGER_SIZE too small");
#endif

	if (c > x->cap) {
		Blk *oldBlk = x->blk;
		// Allocate the new number array
		x->cap = c;
#ifndef BIG_INTEGER_STATIC
		x.blk = new Blk[x.cap];
		// Copy number blocks
		Index i;
		for (i = 0; i < x.len; i++)
			x.blk[i] = oldBlk[i];
		// Delete the old array
		delete[] oldBlk;
#endif
	}
}

//~~CONSTRUCTORS~~//
struct BigUnsigned createBUBU(struct BigUnsigned *x) {
  struct BigUnsigned bu;
  bu.N = N;
  bu.len = x->len;
  // Create array
  bu.cap = x->len;
  #ifndef BIG_INTEGER_STATIC
    bu.blk = new Blk[x.cap];
  #endif
  // Copy blocks
  Index i;
  for (i = 0; i < x->len; i++)
	  bu.blk[i] = x->blk[i];
  return bu;
}
struct BigUnsigned createBU(){
  struct BigUnsigned bu;
  bu.N = N;
  bu.cap = 0;
  bu.len = 0;
  #ifndef BIG_INTEGER_STATIC
    bu.blk = NULL;
  #endif
  return bu;
}
struct BigUnsigned createULBU(unsigned long x) {
  struct BigUnsigned bu;
  if (x == 0)
      return createBU();
  else {
	bu.N = N;
    bu.cap = 1;
    #ifndef BIG_INTEGER_STATIC
        bu.blk = new Blk[1];
    #endif
    bu.len = 1;
	Blk b = x;
    bu.blk[0] = b;
  }
  return bu;
}
struct BigUnsigned createUIBU(unsigned int x) {
  struct BigUnsigned bu;
  if (x == 0)
    return createBU();
  else {
	bu.N = N;
    bu.cap = 1;
    #ifndef BIG_INTEGER_STATIC
        bu.blk = new Blk[1];
    #endif
    bu.len = 1;
	Blk b = x;
    bu.blk[0] = b;
  }
  return bu;
}
struct BigUnsigned createUSBU(unsigned short x) {
  struct BigUnsigned bu;
  if (x == 0)
    return createBU();
  else {
	bu.N = N;
    bu.cap = 1;
    #ifndef BIG_INTEGER_STATIC
        bu.blk = new Blk[1];
    #endif
    bu.len = 1;
	Blk b = x;
    bu.blk[0] = b;
  }
  return bu;
}
struct BigUnsigned createLBU(long x) {
  //if (x < 0)
  //  throw "BigUnsigned constructor: "
  //      "Cannot construct a BigUnsigned from a negative number";
  if (x >=0) {
    struct BigUnsigned bu;
    if (x == 0)
      return createBU();
    else {
	  bu.N = N;
      bu.cap = 1;
      #ifndef BIG_INTEGER_STATIC
          bu.blk = new Blk[1];
      #endif
      bu.len = 1;
	  Blk b = x;
      bu.blk[0] = b;
    }
    return bu;
  }
}
struct BigUnsigned createIBU(int x) {
  //if (x < 0)
  //  throw "BigUnsigned constructor: "
  //    "Cannot construct a BigUnsigned from a negative number";
  if (x>=0) {
    struct BigUnsigned bu;
    if (x == 0)
      return createBU();
    else {
	  bu.N = N;
      bu.cap = 1;
      #ifndef BIG_INTEGER_STATIC
          bu.blk = new Blk[1];
      #endif
      bu.len = 1;
	  Blk b = x;
      bu.blk[0] = b;
    }
    return bu;
  }
}
struct BigUnsigned createSBU(short x) {
  //if (x < 0)
  //  throw "BigUnsigned constructor: "
  //      "Cannot construct a BigUnsigned from a negative number";
  if (x>=0) {
    struct BigUnsigned bu;
    if (x == 0)
      return createBU();
    else {
	  bu.N = N;
      bu.cap = 1;
      #ifndef BIG_INTEGER_STATIC
          bu.blk = new Blk[1];
      #endif
      bu.len = 1;
	  Blk b = x;
      bu.blk[0] = b;
    }
    return bu;
  }
}
struct BigUnsigned createIMorton(int x) {
	return createIBU(x);
}
struct BigUnsigned createUIMorton(unsigned int x) {
	return createUIBU(x);
}

//~~CONVERSIONS~~//
unsigned long buToULong(struct BigUnsigned *bu) {
  if (bu->len == 0)
    return 0; // The number is zero; return zero.
  else if (bu->len == 1) {
	  unsigned long ul = bu->blk[0];
    unsigned long x = ul; // The single block might fit in an X.  Try the conversion.
	Blk b = x;
	if (b == bu->blk[0]) // Make sure the result accurately represents the block.
      return x; // Successful conversion.
  }// Otherwise fall through.
  //throw "BigUnsigned to unsigned long : "
  //    "Value is too big to fit in the requested type";
}
unsigned int buToUInt(struct BigUnsigned *bu) {
  if (bu->len == 0)
    return 0; // The number is zero; return zero.
  else if (bu->len == 1) {
	  unsigned int ui = bu->blk[0];
    unsigned int x = ui; // The single block might fit in an X.  Try the conversion.
	Blk b = x;
	if (b == bu->blk[0]) // Make sure the result accurately represents the block.
      return x; // Successful conversion.
  }// Otherwise fall through.
  //throw "BigUnsigned to unsigned int : "
  //    "Value is too big to fit in the requested type";
}
unsigned short buToUShort(struct BigUnsigned *bu) {
  if (bu->len == 0)
    return 0; // The number is zero; return zero.
  else if (bu->len == 1) {
	  unsigned short us = bu->blk[0];
    unsigned short x = us; // The single block might fit in an X.  Try the conversion.
	Blk b = x;
    if (b == bu->blk[0]) // Make sure the result accurately represents the block.
      return x; // Successful conversion.
  }// Otherwise fall through.
  //throw "BigUnsigned to unsigned short : "
  //    "Value is too big to fit in the requested type";
}
long buToLong(struct BigUnsigned *bu) {
  long x = buToULong(bu);
  if (x >= 0)
    return x;
  //else
  //  throw "BigUnsigned to long : "
  //      "Value is too big to fit in the requested type";
}
int buToInt(struct BigUnsigned *bu) {
  int x = buToUInt(bu);
  if (x >= 0)
    return x;
  //else
  //  throw "BigUnsigned to int : "
  //      "Value is too big to fit in the requested type";
}
short buToShort(struct BigUnsigned *bu) {
  short x = buToUShort(bu);
  if (x >= 0)
    return x;
  //else
  //  throw "BigUnsigned to short : "
  //      "Value is too big to fit in the requested type";
}

#ifdef __cplusplus
#include <string>
std::string buToString(struct BigUnsigned *bu) {
	std::string representation = "";
	for (int i = bu->len; i > 0; --i) {
		representation += "[" + std::to_string(bu->blk[i - 1]) + "]";
	}
	return representation;
}
#endif

//~~BIT/BLOCK ACCESSORS~~//
Blk getBUBlock(struct BigUnsigned *bu, Index i){
  return i >= bu->len ? 0 : bu->blk[i];
}
void setBUBlock(struct BigUnsigned *bu, Index i, Blk newBlock) {
  if (newBlock == 0) {
    if (i < bu->len) {
      bu->blk[i] = 0;
      zapLeadingZeros(bu);
    }                       // If i >= len, no effect.
  } else {
    if (i >= bu->len) {      // The nonzero block extends the number.
      allocateBUAndCopy(bu, i+1);// Zero any added blocks that we aren't setting.
      for (Index j = bu->len; j < i; j++)
        bu->blk[j] = 0;
      bu->len = i+1;
    }
    bu->blk[i] = newBlock;
  }
}
Blk getShiftedBUBlock(struct BigUnsigned *num, Index x, unsigned int y) {
	Blk part1 = (x == 0 || y == 0) ? 0 : (num->blk[x - 1] >> (num->N - y));
	Blk part2 = (x == num->len) ? 0 : (num->blk[x] << y);
  return part1 | part2;
}
Index getBUBitLength(struct BigUnsigned *bu) {
  if (isBUZero(bu))
    return 0;
  else {
    Blk leftmostBlock = getBUBlock(bu, bu->len - 1);
    Index leftmostBlockLen = 0;
    while (leftmostBlock != 0) {
      leftmostBlock >>= 1;
      leftmostBlockLen++;
    }
    return leftmostBlockLen + (bu->len - 1) * bu->N;
  }
}
bool getBUBit(struct BigUnsigned *bu, Index bi) {
	Blk b = 1;
  return (getBUBlock(bu, bi / bu->N) & (b << (bi % bu->N))) != 0;
}
void setBUBit(struct BigUnsigned *bu, Index bi, bool newBit) {
  Index blockI = bi / bu->N;
  Blk b = 1;
  Blk block = getBUBlock(bu, blockI), mask = b << (bi % bu->N);
  block = newBit ? (block | mask) : (block & ~mask);
  setBUBlock(bu, blockI, block);
}

//~~COMPARISON~~//
int compareBU(struct BigUnsigned *x, struct BigUnsigned *y) {
  // A bigger length implies a bigger number.
	if (x->len < y->len)
    return -1;
	/*CmpRes x = less;*/
	else if (x->len > y->len)
    return 1;
  else {
    // Compare blocks one by one from left to right.
	  Index i = x->len;
    while (i > 0) {
      i--;
	  if (x->blk[i] == y->blk[i])
		  continue;
	  else if (x->blk[i] > y->blk[i])
		  return 1;
      else
        return -1;
    }
    // If no blocks differed, the numbers are equal.
    return 0;
  }
}

//~~ARITHMATIC OPERATIONS~~//
struct BigUnsigned addBU(struct BigUnsigned *a, struct BigUnsigned *b) {
  //DTRT_ALIASED(this == &a || this == &b, add(a, b));
  // If one argument is zero, copy the other.
  if (a->len == 0) {
    return createBUBU(b); //Copy B, return that.
  } else if (b->len == 0) {
    return createBUBU(a); //Copy A, return that.
  }

  // Some variables...
  // Carries in and out of an addition stage
  bool carryIn, carryOut;
  Blk temp;
  Index i;
  // a2 points to the longer input, b2 points to the shorter
  const struct BigUnsigned *a2, *b2;
  struct BigUnsigned result;
  if (a->len >= b->len) {
    a2 = a;
    b2 = b;
  } else {
    a2 = b;
    b2 = a;
  }
  // Set prelimiary length and make room in this BigUnsigned
  result.len = a2->len + 1;
  allocateBU(&result, result.len);
  // For each block index that is present in both inputs...
  for (i = 0, carryIn = false; i < b2->len; i++) {
    // Add input blocks
    temp = a2->blk[i] + b2->blk[i];
    // If a rollover occurred, the result is less than either input.
    // This test is used many times in the BigUnsigned code.
    carryOut = (temp < a2->blk[i]);
    // If a carry was input, handle it
    if (carryIn) {
      temp++;
      carryOut |= (temp == 0);
    }
    result.blk[i] = temp; // Save the addition result
    carryIn = carryOut; // Pass the carry along
  }
  // If there is a carry left over, increase blocks until
  // one does not roll over.
  for (; i < a2->len && carryIn; i++) {
    temp = a2->blk[i] + 1;
    carryIn = (temp == 0);
    result.blk[i] = temp;
  }
  // If the carry was resolved but the larger number
  // still has blocks, copy them over.
  for (; i < a2->len; i++)
    result.blk[i] = a2->blk[i];
  // Set the extra block if there's still a carry, decrease length otherwise
  if (carryIn)
    result.blk[i] = 1;
  else
    result.len--;
  return result;
}
struct BigUnsigned subtractBU(struct BigUnsigned *a, struct BigUnsigned *b) {
  //DTRT_ALIASED(this == &a || this == &b, subtract(a, b));
  if (b->len == 0) {
    // If b is zero, copy a.
	 return createBUBU(a);
  }
 //else if (a->len < b->len)
 // // If a is shorter than b, the result is negative.
 // throw "BigUnsigned::subtract: "
 //   "Negative result in unsigned calculation";
  // Some variables...
  bool borrowIn, borrowOut;
  Blk temp;
  Index i;
  struct BigUnsigned result;
  // Set preliminary length and make room
  result.len = a->len;
  allocateBU(&result, result.len);
  // For each block index that is present in both inputs...
  for (i = 0, borrowIn = false; i < b->len; i++) {
    temp = a->blk[i] - b->blk[i];
    // If a reverse rollover occurred,
    // the result is greater than the block from a.
    borrowOut = (temp > a->blk[i]);
    // Handle an incoming borrow
    if (borrowIn) {
      borrowOut |= (temp == 0);
      temp--;
    }
    result.blk[i] = temp; // Save the subtraction result
    borrowIn = borrowOut; // Pass the borrow along
  }
  // If there is a borrow left over, decrease blocks until
  // one does not reverse rollover.
  for (; i < a->len && borrowIn; i++) {
    borrowIn = (a->blk[i] == 0);
    result.blk[i] = a->blk[i] - 1;
  }
  /* If there's still a borrow, the result is negative.
   * Throw an exception, but zero out this object so as to leave it in a
   * predictable state. */
  if (borrowIn) {
    result.len = 0;
    //throw "BigUnsigned::subtract: Negative result in unsigned calculation";
  } else
    // Copy over the rest of the blocks
    for (; i < a->len; i++)
      result.blk[i] = a->blk[i];
  // Zap leading zeros
  zapLeadingZeros(&result);
  return result;
}

//~~BITWISE OPERATORS~~//
/* These are straightforward blockwise operations except that they differ in
 * the output length and the necessity of zapLeadingZeros. */
struct BigUnsigned andBU(struct BigUnsigned *a, struct BigUnsigned *b) {
  //DTRT_ALIASED(this == &a || this == &b, bitAnd(a, b));
  struct BigUnsigned result = createBU();
  // The bitwise & can't be longer than either operand.
  result.len = (a->len >= b->len) ? b->len : a->len;
  allocateBU(&result, result.len);
  Index i;
  for (i = 0; i < result.len; i++)
	  result.blk[i] = a->blk[i] & b->blk[i];
  zapLeadingZeros(&result);
  return result;
}
struct BigUnsigned orBU(struct BigUnsigned *a, struct BigUnsigned *b) {
  //DTRT_ALIASED(this == &a || this == &b, bitOr(a, b));
  struct BigUnsigned result;
  Index i;
  struct BigUnsigned *a2, *b2;
  if (a->len >= b->len) {
    a2 = a;
    b2 = b;
  } else {
    a2 = b;
    b2 = a;
  }
  allocateBU(&result, a2->len);
  for (i = 0; i < b2->len; i++)
    result.blk[i] = a2->blk[i] | b2->blk[i];
  for (; i < a2->len; i++)
    result.blk[i] = a2->blk[i];
  result.len = a2->len;
  // Doesn't need zapLeadingZeros.
  return result;
}
struct BigUnsigned xOrBU(struct BigUnsigned *a, struct BigUnsigned *b) {
  //DTRT_ALIASED(this == &a || this == &b, bitXor(a, b));
  struct BigUnsigned result;
  Index i;
  struct BigUnsigned *a2, *b2;
  if (a->len >= b->len) {
    a2 = a;
    b2 = b;
  } else {
    a2 = b;
    b2 = a;
  }
  allocateBU(&result, a2->len);
  for (i = 0; i < b2->len; i++)
    result.blk[i] = a2->blk[i] ^ b2->blk[i];
  for (; i < a2->len; i++)
    result.blk[i] = a2->blk[i];
  result.len = a2->len;
  zapLeadingZeros(&result);
  return result;
}
struct BigUnsigned shiftBURight(struct BigUnsigned *a, int b);
struct BigUnsigned shiftBULeft(struct BigUnsigned *a, int b);
struct BigUnsigned shiftBURight(struct BigUnsigned *a, int b) {
	//DTRT_ALIASED(this == &a, bitShiftRight(a, b));
	struct BigUnsigned result=createBU();
	if (b < 0) {
		//if (b << 1 == 0)
			//throw "BigUnsigned::bitShiftRight: "
			//"Pathological shift amount not implemented";
			
		if (!(b << 1 == 0)) {
			return shiftBULeft(a, -b);
		}
	}
	// This calculation is wacky, but expressing the shift as a left bit shift
	// within each block lets us use getShiftedBlock.
	Index rightShiftBlocks = (b + result.N - 1) / result.N;
	unsigned int leftShiftBits = result.N * rightShiftBlocks - b;
	// Now (N * rightShiftBlocks - leftShiftBits) == b
	// and 0 <= leftShiftBits < N.
	if (rightShiftBlocks >= a->len + 1) {
		// All of a is guaranteed to be shifted off, even considering the left
		// bit shift.
		result.len = 0;
		return result;
	}
	// Now we're allocating a positive amount.
	// + 1: room for high bits nudged left into another block
	result.len = a->len + 1 - rightShiftBlocks;
	allocateBU(&result, result.len);
	Index i, j;
	for (j = rightShiftBlocks, i = 0; j <= a->len; j++, i++)
		result.blk[i] = getShiftedBUBlock(a, j, leftShiftBits);
	// Zap possible leading zero
	if (result.blk[result.len - 1] == 0)
		result.len--;
	return result;
}
struct BigUnsigned shiftBULeft(struct BigUnsigned *a, int b) {
  //DTRT_ALIASED(this == &a, bitShiftLeft(a, b));
  struct BigUnsigned result=createBU();
  if (b < 0) {
    //if (b << 1 == 0)
    //  throw "BigUnsigned bitShiftLeft: "
    //    "Pathological shift amount not implemented";
	  if (!(b << 1 == 0)) {
      return shiftBURight(a, -b);
    }
  }
  Index shiftBlocks = b / result.N;
  unsigned int shiftBits = b % result.N;
  // + 1: room for high bits nudged left into another block
  result.len = a->len + shiftBlocks + 1;
  allocateBU(&result, result.len);
  Index i, j;
  for (i = 0; i < shiftBlocks; i++)
    result.blk[i] = 0;
  for (j = 0, i = shiftBlocks; j <= a->len; j++, i++)
    result.blk[i] = getShiftedBUBlock(a, j, shiftBits);
  // Zap possible leading zero
  if (result.blk[result.len - 1] == 0)
    result.len--;
  return result;
}

#endif