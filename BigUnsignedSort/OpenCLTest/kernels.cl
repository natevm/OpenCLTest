//1. Radix Predicate
//https://www.youtube.com/watch?v=RdfmxfZBHpo
__kernel void Predicate(
	__global struct BigUnsigned *inputBuffer,
	__global Index *predicateBuffer,
	Index index,
	short comparedWith)
{
	uint gid = get_global_id(0);
	struct BigUnsigned self = inputBuffer[gid];
	predicateBuffer[gid] = (getBUBit(&self, index) == comparedWith) ? 0:1;
	barrier(CLK_GLOBAL_MEM_FENCE);
}


//2. StreamScan
//Hilles Steele
//Inclusive
//https://www.youtube.com/watch?v=RdfmxfZBHpo
#define SWAP(a,b) {__local int *tmp=a;a=b;b=tmp;}
__kernel
void StreamScan(
__global Index* buffer,
__global Index* result,
__global volatile Index* I,
__local Index* localBuffer,
__local Index* scratch)
{
	Index F = -1; //NULL FLAG
	//INITIALIZATION
	uint gid = get_global_id(0);
	uint lid = get_local_id(0);
	uint ls = get_local_size(0);
	localBuffer[lid] = scratch[lid] = buffer[gid];

	//ADDITIVE REDUCTION
	barrier(CLK_LOCAL_MEM_FENCE);
	for (int offset = ls / 2; offset > 0; offset >>= 1) {
		barrier(CLK_LOCAL_MEM_FENCE);
		if (lid < offset)
			scratch[lid] = scratch[lid + offset] + scratch[lid];
	}
	result[gid] = buffer[gid];

	//ADJACENT SYNCRONIZATION
	barrier(CLK_LOCAL_MEM_FENCE);
	int intermediateIndex = gid / get_local_size(0);
	if (lid == 0) {
		Index p = 0;
		if (intermediateIndex == 0) I[0] = scratch[0];
		else{
			while ((p = I[intermediateIndex - 1]) == F){}
			I[intermediateIndex] = p + scratch[0];
		}
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	result[gid] = I[intermediateIndex] - scratch[0];
	//HILLIES SCAN
	
	barrier(CLK_LOCAL_MEM_FENCE);
	scratch[lid] = localBuffer[lid];
	for (uint i = 1; i < ls; i <<= 1) {
		if (lid >(i - 1))
			scratch[lid] = localBuffer[lid] + localBuffer[lid - i];
		else
			scratch[lid] = localBuffer[lid];
		barrier(CLK_LOCAL_MEM_FENCE);
		SWAP(localBuffer, scratch);
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	result[gid] += localBuffer[lid];
}

//3. Compact
__kernel void BUCompact(
	__global struct BigUnsigned *inputBuffer,
	__global Index *lPredicateBuffer,
	__global Index *leftBuffer,
	__global Index *rightBuffer,
	Index size) //Index typedef'd in BigUnsigned.c
{
	uint gid = get_global_id(0);
	Index index = 0;
	if (lPredicateBuffer[gid] == 1)
		index = leftBuffer[gid];
	else
		index = rightBuffer[gid] + leftBuffer[size - 1];
	struct BigUnsigned temp = inputBuffer[gid];
	barrier(CLK_GLOBAL_MEM_FENCE);
	inputBuffer[index - 1] = temp;
}
