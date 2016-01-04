//1. Radix Predicate
//https://www.youtube.com/watch?v=RdfmxfZBHpo
#define SWAP(a,b) {__local int *tmp=a;a=b;b=tmp;}
__kernel void Predicate(
	__global int *inputBuffer,
	__global int *predicateBuffer,
	unsigned int index,
	unsigned int comparedWith)
{
	uint gid = get_global_id(0);
	predicateBuffer[gid] = ((inputBuffer[gid] & (1<<index)) >> index ) == comparedWith;
}

//2. Scan
//Hilles Steele
//Inclusive
//https://www.youtube.com/watch?v=RdfmxfZBHpo
#define SWAP(a,b) {__global int *tmp=a;a=b;b=tmp;}
__kernel void Scan(
	__global int *buffer,
	__global int *result,
	__local int *previousStep,
	__local int *currentStep)
{
	uint gid = get_global_id(0);
	uint gs = get_global_size(0);

	//copy from global to local
	//currentStep[gid] = previousStep[gid] = buffer[gid];
	result[gid] = buffer[gid];
	barrier(CLK_GLOBAL_MEM_FENCE);

	//increment by powers of 2
	for (uint i = 1; i <= gs; i <<= 1) {
		//[0,1,2,3,4]
		//   ^ ^ ^ ^
		if (gid >(i - 1)) {
			//currentStep[gid] = previousStep[gid] + previousStep[gid - i];
			buffer[gid] = result[gid] + result[gid - i];
		}
		//[0,1,2,3,4]
		// ^
		else {
			//currentStep[gid] = previousStep[gid];
			buffer[gid] = result[gid];
		}
		//currentStep[gid] = gid;
		barrier(CLK_GLOBAL_MEM_FENCE);
		SWAP(result, buffer);
	}
	//result[gid] = previousStep[gid];
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
	if (lPredicateBuffer[gid] == 1) {
		index = leftBuffer[gid];
	}
	else {
		index = rightBuffer[gid] + leftBuffer[size-1]; 
	}
	int temp = inputBuffer[gid];
	barrier(CLK_GLOBAL_MEM_FENCE);
	inputBuffer[index-1] = temp;
}