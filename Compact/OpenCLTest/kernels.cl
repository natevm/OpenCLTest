//1. Predicate
//https://www.youtube.com/watch?v=RdfmxfZBHpo
#define SWAP(a,b) {__local int *tmp=a;a=b;b=tmp;}
__kernel void Predicate(
	__global int *inputBuffer,
	__global int *predicateBuffer)
{
	uint gid = get_global_id(0);
	predicateBuffer[gid] = !(inputBuffer[gid] & 1);
}

//2. Scan
//Hilles Steele
//exclusive
//https://www.youtube.com/watch?v=RdfmxfZBHpo
#define SWAP(a,b) {__local int *tmp=a;a=b;b=tmp;}
__kernel void Scan(
	__global int *buffer,
	__global int *result,
	__local int *previousStep,
	__local int *currentStep)
{
	uint gid = get_global_id(0);
	uint gs = get_global_size(0);

	//copy from global to local
	currentStep[gid] = previousStep[gid] = buffer[gid];
	barrier(CLK_GLOBAL_MEM_FENCE);

	//increment by powers of 2
	for (uint i = 1; i <= gs; i <<= 1) {
		//[0,1,2,3,4]
		//   ^ ^ ^ ^
		if (gid >(i - 1)) {
			currentStep[gid] = previousStep[gid] + previousStep[gid - i];
		}
		//[0,1,2,3,4]
		// ^
		else {
			currentStep[gid] = previousStep[gid];
		}
		//currentStep[gid] = gid;
		barrier(CLK_GLOBAL_MEM_FENCE);
		SWAP(previousStep, currentStep);
	}
	//Inclusive to exclusive
	if (gid < gs && gid > 0) {
		//result[gid] = gid;
		result[gid] = previousStep[gid-1];
	}
}

//3. Compact
__kernel void Compact(
	__global int *inputBuffer,
	__global int *predicateBuffer,
	__global int *addressBuffer,
	__global int *result)
{
	uint gid = get_global_id(0);
	if (predicateBuffer[gid] == 1) {
		result[addressBuffer[gid]] = inputBuffer[gid];
	}
}