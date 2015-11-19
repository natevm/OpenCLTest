//Associative Reduction
__kernel 
void reduce1(
	__global float* buffer, 
	__local float* scratch,
	__const int length,
	__global float* result)
{
	int global_index = get_global_id(0);
	int local_index = get_local_id(0);

	//Load data from global into local memory
	if (global_index < length) {
		scratch[local_index] = buffer[global_index];
	} else {
		//Infinity is the identity element for the in operation
		scratch[local_index] = INFINITY;
	}

	//Barrier syncs all workers.
	barrier(CLK_LOCAL_MEM_FENCE);

	for (int offset = 1; offset < get_local_size(0); offset <<= 1) {
		int mask = (offset << 1) - 1;
		if ((local_index & mask) == 0) {
			float other = scratch[local_index + offset];
			float mine = scratch[local_index];
			scratch[local_index] = (mine < other) ? mine : other;
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	if (local_index == 0) {
		result[get_group_id(0)] = scratch[0];
	}
}

//Commutative Reduction
__kernel
void addReduce(
__global float* buffer,
__local float* scratch,
__const int length,
__global float* result)
{
	int global_index = get_global_id(0);
	int local_index = get_local_id(0);

	//Load data from global into local memory
	if (global_index < length) {
		scratch[local_index] = buffer[global_index];
	}
	else {
		//Infinity is the identity element for the in operation
		scratch[local_index] = INFINITY;
	}

	//Barrier syncs all workers.
	barrier(CLK_LOCAL_MEM_FENCE);

	for (int offset = get_local_size(0) / 2; offset > 0; offset >>= 1) {
		if (local_index < offset) {
			float other = scratch[local_index + offset];
			float mine = scratch[local_index];
			scratch[local_index] = mine + other;
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	if (local_index == 0) {
		result[get_group_id(0)] = scratch[0];
	}
}