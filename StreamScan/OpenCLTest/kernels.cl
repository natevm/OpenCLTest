/* 
 * Nathan Vollmer. January 3, 2016.
 * The following is an implementation of StreamScan. 
 * To learn more, visit https://code.google.com/p/streamscan/downloads/list
 */
#define SWAP(a,b) {__local float *tmp=a;a=b;b=tmp;}
__kernel
void StreamScan(
__global int* buffer,
__global volatile int* I,
__local int* localBuffer,
__local int* scratch)
{
	int F = -1; //NULL FLAG
//INITIALIZATION
	uint gid = get_global_id(0);
	uint lid = get_local_id(0);
	uint gs = get_local_size(0);
	localBuffer[lid] = scratch[lid] = buffer[gid];

//ADDITIVE REDUCTION
	for (int offset = gs / 2; offset > 0; offset >>= 1) {
		barrier(CLK_LOCAL_MEM_FENCE);
		if (lid < offset) 
			scratch[lid] = scratch[lid + offset] + scratch[lid];
	}
	barrier(CLK_LOCAL_MEM_FENCE);

//ADJACENT SYNCRONIZATION
	int intermediateIndex = gid / get_local_size(0);
	if (lid == 0) {
		int p = 0;
		if (intermediateIndex == 0) I[0] = scratch[0];
		else{
			while ((p = I[intermediateIndex - 1]) == F){}
			I[intermediateIndex] = p + scratch[0];
		}
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	buffer[gid] = I[intermediateIndex] - scratch[0];
	barrier(CLK_LOCAL_MEM_FENCE);
//HILLIES SCAN
	scratch[lid] = localBuffer[lid];
	barrier(CLK_LOCAL_MEM_FENCE);
	for (uint i = 1; i < gs; i <<= 1) {
		if (lid >(i - 1)) 
			scratch[lid] = localBuffer[lid] + localBuffer[lid - i];
		else 
			scratch[lid] = localBuffer[lid];
		barrier(CLK_LOCAL_MEM_FENCE);
		SWAP(localBuffer, scratch);
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	buffer[gid] += localBuffer[lid];
}