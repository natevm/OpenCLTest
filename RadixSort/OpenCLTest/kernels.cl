//1. Radix Predicate
//https://www.youtube.com/watch?v=RdfmxfZBHpo
__kernel void Predicate(
	__global int *inputBuffer,
	__global int *predicateBuffer,
	unsigned int index,
	unsigned int comparedWith)
{
	uint gid = get_global_id(0);
	predicateBuffer[gid] = ((inputBuffer[gid] & (1<<index)) >> index ) == comparedWith;
}

//2. StreamScan
//Hilles Steele
//Inclusive
//https://www.youtube.com/watch?v=RdfmxfZBHpo
#define SWAP(a,b) {__local int *tmp=a;a=b;b=tmp;}
__kernel
void StreamScan(
__global int* buffer,
__global int* result,
__global volatile int* I,
__local int* localBuffer,
__local int* scratch)
{
	int F = -1; //NULL FLAG
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
	
	//ADJACENT SYNCRONIZATION
	barrier(CLK_LOCAL_MEM_FENCE);
	int workgroupIndex = gid / get_local_size(0); //get workgroup id
	if (lid == 0) {
		int p = 0;
		if (workgroupIndex == 0) I[0] = scratch[0];
		else{
			while ((p = I[workgroupIndex - 1]) == F){}
			I[workgroupIndex] = p + scratch[0];
		}
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	result[gid] = I[workgroupIndex] - scratch[0]; //Calculating starting point of scan

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
__kernel void Sort(
	__global int *inputBuffer,
	__global int *lPredicateBuffer,
	__global int *leftBuffer,
	__global int *rightBuffer,
	unsigned int size)
{
	uint gid = get_global_id(0);
	int index = 0;
	if (lPredicateBuffer[gid] == 1) 
		index = leftBuffer[gid];
	else 
		index = rightBuffer[gid] + leftBuffer[size-1]; 
	int temp = inputBuffer[gid];
	barrier(CLK_GLOBAL_MEM_FENCE);
	inputBuffer[index-1] = temp;
}