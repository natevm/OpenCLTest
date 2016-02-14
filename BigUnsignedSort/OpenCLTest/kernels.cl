//Radix Predication
__kernel void Predicate(
	__global BigUnsigned *inputBuffer,
	__global Index *predicateBuffer,
	Index index,
	unsigned char comparedWith)
{
	const size_t gid = get_global_id(0);
	BigUnsigned self = inputBuffer[gid];
  barrier(CLK_GLOBAL_MEM_FENCE);
  predicateBuffer[gid] = (getBUBit(&self, index) == comparedWith) ? 1:0;
	barrier(CLK_GLOBAL_MEM_FENCE);
}

//StreamScan
//https://www.youtube.com/watch?v=RdfmxfZBHpo
#define SWAP(a,b) {__local Index *tmp=a;a=b;b=tmp;}
__kernel
void StreamScan(
__global Index* buffer,
__global Index* result,
__global volatile int* I,
__local int* localBuffer,
__local int* scratch)
{
	//INITIALIZATION
  const size_t gid = get_global_id(0);
  const size_t lid = get_local_id(0);
  const size_t wid = get_group_id(0);
  const size_t ls = get_local_size(0);
	localBuffer[lid] = scratch[lid] = buffer[gid];
  int sum = 0;
  int temp = 0;
  barrier(CLK_GLOBAL_MEM_FENCE);

	//ADDITIVE REDUCTION
	for (int offset = ls / 2; offset > 0; offset >>= 1) {
    if (lid < offset){ scratch[lid] = scratch[lid + offset] + scratch[lid];}
		barrier(CLK_LOCAL_MEM_FENCE);
	}
  //I[wid] = scratch[0];
  
	//ADJACENT SYNCRONIZATION
  if (lid == 0 && gid != 0) {
    while (I[wid - 1] == -1);
    atomic_xchg(&I[wid], I[wid - 1] + scratch[0]);
  }
  if (gid == 0) {
    atomic_xchg(&I[0], scratch[0]);
  }
  barrier(CLK_GLOBAL_MEM_FENCE);

  //SCAN
  scratch[lid] = localBuffer[lid];
  int tmp = 0;
	for (uint i = 1; i < ls; i <<= 1) {
		if (lid >(i - 1))
			scratch[lid] = localBuffer[lid] + localBuffer[lid - i];
		else
			scratch[lid] = localBuffer[lid];
    //Swap
		barrier(CLK_LOCAL_MEM_FENCE);
    SWAP(scratch, localBuffer);
	  barrier(CLK_LOCAL_MEM_FENCE);
	}
  if (wid != 0) {
    sum += localBuffer[lid] + I[wid - 1];
  }
  else {
    sum += localBuffer[lid];
  }
  atomic_xchg(&result[gid], sum); 
  barrier(CLK_GLOBAL_MEM_FENCE);
}

//Double Compaction
__kernel void BUCompact(
	__global BigUnsigned *inputBuffer,
  __global BigUnsigned *resultBuffer,
	__global Index *lPredicateBuffer,
	__global Index *leftBuffer,
	__global Index *rightBuffer,
	Index size) //Index typedef'd in BigUnsigned.c
{
	const size_t gid = get_global_id(0);
	Index index = 0;
	if (lPredicateBuffer[gid] == 1)
		index = leftBuffer[gid];
	else
		index = rightBuffer[gid] + leftBuffer[size - 1];
  barrier(CLK_GLOBAL_MEM_FENCE);
	BigUnsigned temp = inputBuffer[gid];
	barrier(CLK_GLOBAL_MEM_FENCE);
  resultBuffer[index - 1] = temp;
  barrier(CLK_GLOBAL_MEM_FENCE);
  lPredicateBuffer[gid] = index; //for testing.
  barrier(CLK_GLOBAL_MEM_FENCE);
}
