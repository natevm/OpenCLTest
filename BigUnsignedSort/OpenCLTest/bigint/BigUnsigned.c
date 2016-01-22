#ifndef BIGUNSIGNED_C
#define BIGUNSIGNED_C

#include <stdexcept>

#define BIG_INTEGER_STATIC
#define BIG_INTEGER_SIZE 100

typedef unsigned int Index; // Type for the index of a block in the array
typedef unsigned long Blk;  // Type for the blocks

// Enumeration for result of a comparison.
enum CmpRes { less = -1, equal = 0, greater = 1 };     

// Make sure we have NULL.
#ifndef NULL
#define NULL 0
#endif

// struct BigUnsigned allows storing integers larger than a long using an array of blk.
struct BigUnsigned {
  //~~ NumberlikeArray Fields ~~//
  static const unsigned int N = 8 * sizeof(Blk);  // Number of bits in a block.
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
inline void zapLeadingZeros(struct BigUnsigned &bu) {
  while (bu.len > 0 && bu.blk[bu.len - 1] == 0)
    bu.len--;
}
inline bool isBUZero(struct BigUnsigned &bu) { return bu.len == 0; }
inline void allocateBU(struct BigUnsigned &bu, Index c) {
  // If the requested capacity is more than the current capacity...
  #ifdef BIG_INTEGER_STATIC
    if (c > BIG_INTEGER_SIZE) throw std::logic_error("BIG_INTEGER_SIZE too small");
  #endif

  if (c > bu.cap) {
    // Delete the old number array
    #ifndef BIG_INTEGER_STATIC
      delete [] bu.blk;
    #endif
      // Allocate the new array
      bu.cap = c;
    #ifndef BIG_INTEGER_STATIC
      bu.blk = new Blk[bu.cap];
    #endif
  }
}
inline void allocateBUAndCopy(struct BigUnsigned &x, Index c) {
	// If the requested capacity is more than the current capacity...
#ifdef BIG_INTEGER_STATIC
	if (c > BIG_INTEGER_SIZE) throw std::logic_error("BIG_INTEGER_SIZE too small");
#endif

	if (c > x.cap) {
		Blk *oldBlk = x.blk;
		// Allocate the new number array
		x.cap = c;
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
inline BigUnsigned createBU(BigUnsigned &x) {
  struct BigUnsigned bu;
  bu.len = x.len;
  // Create array
  bu.cap = x.len;
  #ifndef BIG_INTEGER_STATIC
    bu.blk = new Blk[x.cap];
  #endif
  // Copy blocks
  Index i;
  for (i = 0; i < x.len; i++)
    bu.blk[i] = x.blk[i];
  return bu;
}
inline BigUnsigned createBU(){
  struct BigUnsigned bu;
  bu.cap = 0;
  bu.len = 0;
  #ifndef BIG_INTEGER_STATIC
    bu.blk = NULL;
  #endif
  return bu;
}
inline BigUnsigned createBU(unsigned long x) {
  struct BigUnsigned bu;
  if (x == 0)
      return createBU();
  else {
    bu.cap = 1;
    #ifndef BIG_INTEGER_STATIC
        bu.blk = new Blk[1];
    #endif
    bu.len = 1;
    bu.blk[0] = Blk(x);
  }
  return bu;
}
inline BigUnsigned createBU(unsigned int x) {
  struct BigUnsigned bu;
  if (x == 0)
    return createBU();
  else {
    bu.cap = 1;
    #ifndef BIG_INTEGER_STATIC
        bu.blk = new Blk[1];
    #endif
    bu.len = 1;
    bu.blk[0] = Blk(x);
  }
  return bu;
}
inline BigUnsigned createBU(unsigned short x) {
  struct BigUnsigned bu;
  if (x == 0)
    return createBU();
  else {
    bu.cap = 1;
    #ifndef BIG_INTEGER_STATIC
        bu.blk = new Blk[1];
    #endif
    bu.len = 1;
    bu.blk[0] = Blk(x);
  }
  return bu;
}
inline BigUnsigned createBU(long x) {
  if (x < 0)
    throw "BigUnsigned constructor: "
        "Cannot construct a BigUnsigned from a negative number";
  else {
    struct BigUnsigned bu;
    if (x == 0)
      return createBU();
    else {
      bu.cap = 1;
      #ifndef BIG_INTEGER_STATIC
          bu.blk = new Blk[1];
      #endif
      bu.len = 1;
      bu.blk[0] = Blk(x);
    }
    return bu;
  }
}
inline BigUnsigned createBU(int x) {
  if (x < 0)
    throw "BigUnsigned constructor: "
      "Cannot construct a BigUnsigned from a negative number";
  else {
    struct BigUnsigned bu;
    if (x == 0)
      return createBU();
    else {
      bu.cap = 1;
      #ifndef BIG_INTEGER_STATIC
          bu.blk = new Blk[1];
      #endif
      bu.len = 1;
      bu.blk[0] = Blk(x);
    }
    return bu;
  }
}
inline BigUnsigned createBU(short x) {
  if (x < 0)
    throw "BigUnsigned constructor: "
        "Cannot construct a BigUnsigned from a negative number";
  else {
    struct BigUnsigned bu;
    if (x == 0)
      return createBU();
    else {
      bu.cap = 1;
      #ifndef BIG_INTEGER_STATIC
          bu.blk = new Blk[1];
      #endif
      bu.len = 1;
      bu.blk[0] = Blk(x);
    }
    return bu;
  }
}
inline BigUnsigned createMorton(int x) {
	return createBU(x);
}
inline BigUnsigned createMorton(unsigned int x) {
	return createBU(x);
}

//~~CONVERSIONS~~//
inline unsigned long buToULong(struct BigUnsigned bu) {
  if (bu.len == 0)
    return 0; // The number is zero; return zero.
  else if (bu.len == 1) {
    unsigned long x = unsigned long(bu.blk[0]); // The single block might fit in an X.  Try the conversion.
    if (Blk(x) == bu.blk[0]) // Make sure the result accurately represents the block.
      return x; // Successful conversion.
  }// Otherwise fall through.
  throw "BigUnsigned to unsigned long : "
      "Value is too big to fit in the requested type";
}
inline unsigned int buToUInt(struct BigUnsigned bu) {
  if (bu.len == 0)
    return 0; // The number is zero; return zero.
  else if (bu.len == 1) {
    unsigned int x = unsigned int(bu.blk[0]); // The single block might fit in an X.  Try the conversion.
    if (Blk(x) == bu.blk[0]) // Make sure the result accurately represents the block.
      return x; // Successful conversion.
  }// Otherwise fall through.
  throw "BigUnsigned to unsigned int : "
      "Value is too big to fit in the requested type";
}
inline unsigned short buToUShort(struct BigUnsigned bu) {
  if (bu.len == 0)
    return 0; // The number is zero; return zero.
  else if (bu.len == 1) {
    unsigned short x = unsigned short(bu.blk[0]); // The single block might fit in an X.  Try the conversion.
    if (Blk(x) == bu.blk[0]) // Make sure the result accurately represents the block.
      return x; // Successful conversion.
  }// Otherwise fall through.
  throw "BigUnsigned to unsigned short : "
      "Value is too big to fit in the requested type";
}
inline long buToLong(struct BigUnsigned bu) {
  long x = buToULong(bu);
  if (x >= 0)
    return x;
  else
    throw "BigUnsigned to long : "
        "Value is too big to fit in the requested type";
}
inline int buToInt(struct BigUnsigned bu) {
  int x = buToUInt(bu);
  if (x >= 0)
    return x;
  else
    throw "BigUnsigned to int : "
        "Value is too big to fit in the requested type";
}
inline short buToShort(struct BigUnsigned bu) {
  short x = buToUShort(bu);
  if (x >= 0)
    return x;
  else
    throw "BigUnsigned to short : "
        "Value is too big to fit in the requested type";
}

#ifdef __cplusplus
#include <string>
inline std::string buToString(struct BigUnsigned &bu) {
	std::string representation = "";
	for (int i = bu.len; i > 0; --i) {
		representation += "[" + std::to_string(bu.blk[i - 1]) + "]";
	}
	return representation;
}
#endif

//~~BIT/BLOCK ACCESSORS~~//
inline Blk getBUBlock(struct BigUnsigned &bu, Index i){
  return i >= bu.len ? 0 : bu.blk[i];
}
inline void setBUBlock(struct BigUnsigned &bu, Index i, Blk newBlock) {
  if (newBlock == 0) {
    if (i < bu.len) {
      bu.blk[i] = 0;
      zapLeadingZeros(bu);
    }                       // If i >= len, no effect.
  } else {
    if (i >= bu.len) {      // The nonzero block extends the number.
      allocateBUAndCopy(bu, i+1);// Zero any added blocks that we aren't setting.
      for (Index j = bu.len; j < i; j++)
        bu.blk[j] = 0;
      bu.len = i+1;
    }
    bu.blk[i] = newBlock;
  }
}
inline Blk getShiftedBUBlock(const BigUnsigned &num, Index x, unsigned int y) {
  Blk part1 = (x == 0 || y == 0) ? 0 : (num.blk[x - 1] >> (num.N - y));
  Blk part2 = (x == num.len) ? 0 : (num.blk[x] << y);
  return part1 | part2;
}
inline Index getBUBitLength(struct BigUnsigned &bu) {
  if (isBUZero(bu))
    return 0;
  else {
    Blk leftmostBlock = getBUBlock(bu, bu.len - 1);
    Index leftmostBlockLen = 0;
    while (leftmostBlock != 0) {
      leftmostBlock >>= 1;
      leftmostBlockLen++;
    }
    return leftmostBlockLen + (bu.len - 1) * bu.N;
  }
}
inline bool getBUBit(struct BigUnsigned &bu, Index bi) {
  return (getBUBlock(bu, bi / bu.N) & (Blk(1) << (bi % bu.N))) != 0;
}
inline void setBUBit(struct BigUnsigned &bu, Index bi, bool newBit) {
  Index blockI = bi / bu.N;
  Blk block = getBUBlock(bu, blockI), mask = Blk(1) << (bi % bu.N);
  block = newBit ? (block | mask) : (block & ~mask);
  setBUBlock(bu, blockI, block);
}

//~~COMPARISON~~//
inline CmpRes compareBU(const BigUnsigned &x, const BigUnsigned &y) {
  // A bigger length implies a bigger number.
  if (x.len < y.len)
    return CmpRes::less;
  else if (x.len > y.len)
    return greater;
  else {
    // Compare blocks one by one from left to right.
    Index i = x.len;
    while (i > 0) {
      i--;
      if (x.blk[i] == y.blk[i])
        continue;
      else if (x.blk[i] > y.blk[i])
        return greater;
      else
        return CmpRes::less;
    }
    // If no blocks differed, the numbers are equal.
    return CmpRes::equal;
  }
}

//~~ARITHMATIC OPERATIONS~~//
inline BigUnsigned addBU(BigUnsigned &a, BigUnsigned &b) {
  //DTRT_ALIASED(this == &a || this == &b, add(a, b));
  // If one argument is zero, copy the other.
  if (a.len == 0) {
    return createBU(b); //Copy B, return that.
  } else if (b.len == 0) {
    return createBU(a); //Copy A, return that.
  }

  // Some variables...
  // Carries in and out of an addition stage
  bool carryIn, carryOut;
  Blk temp;
  Index i;
  // a2 points to the longer input, b2 points to the shorter
  const struct BigUnsigned *a2, *b2;
  struct BigUnsigned result;
  if (a.len >= b.len) {
    a2 = &a;
    b2 = &b;
  } else {
    a2 = &b;
    b2 = &a;
  }
  // Set prelimiary length and make room in this BigUnsigned
  result.len = a2->len + 1;
  allocateBU(result, result.len);
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
inline BigUnsigned subtractBU(BigUnsigned &a, BigUnsigned &b) {
  //DTRT_ALIASED(this == &a || this == &b, subtract(a, b));
  if (b.len == 0) {
    // If b is zero, copy a.
	 return createBU(a);
  } else if (a.len < b.len)
    // If a is shorter than b, the result is negative.
    throw "BigUnsigned::subtract: "
      "Negative result in unsigned calculation";
  // Some variables...
  bool borrowIn, borrowOut;
  Blk temp;
  Index i;
  struct BigUnsigned result;
  // Set preliminary length and make room
  result.len = a.len;
  allocateBU(result, result.len);
  // For each block index that is present in both inputs...
  for (i = 0, borrowIn = false; i < b.len; i++) {
    temp = a.blk[i] - b.blk[i];
    // If a reverse rollover occurred,
    // the result is greater than the block from a.
    borrowOut = (temp > a.blk[i]);
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
  for (; i < a.len && borrowIn; i++) {
    borrowIn = (a.blk[i] == 0);
    result.blk[i] = a.blk[i] - 1;
  }
  /* If there's still a borrow, the result is negative.
   * Throw an exception, but zero out this object so as to leave it in a
   * predictable state. */
  if (borrowIn) {
    result.len = 0;
    throw "BigUnsigned::subtract: Negative result in unsigned calculation";
  } else
    // Copy over the rest of the blocks
    for (; i < a.len; i++)
      result.blk[i] = a.blk[i];
  // Zap leading zeros
  zapLeadingZeros(result);
  return result;
}

//~~BITWISE OPERATORS~~//
/* These are straightforward blockwise operations except that they differ in
 * the output length and the necessity of zapLeadingZeros. */
inline BigUnsigned andBU(const BigUnsigned &a, const BigUnsigned &b) {
  //DTRT_ALIASED(this == &a || this == &b, bitAnd(a, b));
  struct BigUnsigned result = createBU();
  // The bitwise & can't be longer than either operand.
  result.len = (a.len >= b.len) ? b.len : a.len;
  allocateBU(result, result.len);
  Index i;
  for (i = 0; i < result.len; i++)
    result.blk[i] = a.blk[i] & b.blk[i];
  zapLeadingZeros(result);
  return result;
}
inline BigUnsigned orBU(const BigUnsigned &a, const BigUnsigned &b) {
  //DTRT_ALIASED(this == &a || this == &b, bitOr(a, b));
  struct BigUnsigned result;
  Index i;
  const BigUnsigned *a2, *b2;
  if (a.len >= b.len) {
    a2 = &a;
    b2 = &b;
  } else {
    a2 = &b;
    b2 = &a;
  }
  allocateBU(result, a2->len);
  for (i = 0; i < b2->len; i++)
    result.blk[i] = a2->blk[i] | b2->blk[i];
  for (; i < a2->len; i++)
    result.blk[i] = a2->blk[i];
  result.len = a2->len;
  // Doesn't need zapLeadingZeros.
  return result;
}
inline BigUnsigned xOrBU(const BigUnsigned &a, const BigUnsigned &b) {
  //DTRT_ALIASED(this == &a || this == &b, bitXor(a, b));
  struct BigUnsigned result;
  Index i;
  const BigUnsigned *a2, *b2;
  if (a.len >= b.len) {
    a2 = &a;
    b2 = &b;
  } else {
    a2 = &b;
    b2 = &a;
  }
  allocateBU(result, a2->len);
  for (i = 0; i < b2->len; i++)
    result.blk[i] = a2->blk[i] ^ b2->blk[i];
  for (; i < a2->len; i++)
    result.blk[i] = a2->blk[i];
  result.len = a2->len;
  zapLeadingZeros(result);
  return result;
}
BigUnsigned shiftBURight(const BigUnsigned &a, int b);
BigUnsigned shiftBULeft(const BigUnsigned &a, int b);
inline BigUnsigned shiftBURight(const BigUnsigned &a, int b) {
	//DTRT_ALIASED(this == &a, bitShiftRight(a, b));
	struct BigUnsigned result;
	if (b < 0) {
		if (b << 1 == 0)
			throw "BigUnsigned::bitShiftRight: "
			"Pathological shift amount not implemented";
		else {
			return shiftBULeft(a, -b);
		}
	}
	// This calculation is wacky, but expressing the shift as a left bit shift
	// within each block lets us use getShiftedBlock.
	Index rightShiftBlocks = (b + result.N - 1) / result.N;
	unsigned int leftShiftBits = result.N * rightShiftBlocks - b;
	// Now (N * rightShiftBlocks - leftShiftBits) == b
	// and 0 <= leftShiftBits < N.
	if (rightShiftBlocks >= a.len + 1) {
		// All of a is guaranteed to be shifted off, even considering the left
		// bit shift.
		result.len = 0;
		return result;
	}
	// Now we're allocating a positive amount.
	// + 1: room for high bits nudged left into another block
	result.len = a.len + 1 - rightShiftBlocks;
	allocateBU(result, result.len);
	Index i, j;
	for (j = rightShiftBlocks, i = 0; j <= a.len; j++, i++)
		result.blk[i] = getShiftedBUBlock(a, j, leftShiftBits);
	// Zap possible leading zero
	if (result.blk[result.len - 1] == 0)
		result.len--;
	return result;
}
inline BigUnsigned shiftBULeft(const BigUnsigned &a, int b) {
  //DTRT_ALIASED(this == &a, bitShiftLeft(a, b));
  struct BigUnsigned result;
  if (b < 0) {
    if (b << 1 == 0)
      throw "BigUnsigned bitShiftLeft: "
        "Pathological shift amount not implemented";
    else {
      return shiftBURight(a, -b);
    }
  }
  Index shiftBlocks = b / result.N;
  unsigned int shiftBits = b % result.N;
  // + 1: room for high bits nudged left into another block
  result.len = a.len + shiftBlocks + 1;
  allocateBU(result, result.len);
  Index i, j;
  for (i = 0; i < shiftBlocks; i++)
    result.blk[i] = 0;
  for (j = 0, i = shiftBlocks; j <= a.len; j++, i++)
    result.blk[i] = getShiftedBUBlock(a, j, shiftBits);
  // Zap possible leading zero
  if (result.blk[result.len - 1] == 0)
    result.len--;
  return result;
}

#endif