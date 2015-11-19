//Hilles Steele
//inclusive
//https://www.youtube.com/watch?v=RdfmxfZBHpo
#define SWAP(a,b) {__local float *tmp=a;a=b;b=tmp;}
__kernel void HillesSteelScan(
	__global float *buffer,
	__global float *result,
	__local float *previousStep,
	__local float *currentStep)
{
	uint gid = get_global_id(0);
	uint lid = get_local_id(0);
	uint gs = get_local_size(0);

	//copy from global to local
	currentStep[lid] = previousStep[lid] = buffer[gid];
	barrier(CLK_LOCAL_MEM_FENCE);

	//increment by powers of 2
	for (uint i = 1; i < gs; i <<= 1) {
		//[0,1,2,3,4]
		//   ^ ^ ^ ^
		if (lid > (i - 1)) {
			currentStep[lid] = previousStep[lid] + previousStep[lid - i];
		}
		//[0,1,2,3,4]
		// ^
		else {
			currentStep[lid] = previousStep[lid];
		}
		barrier(CLK_LOCAL_MEM_FENCE);
		SWAP(previousStep, currentStep);
	}
	result[gid] = previousStep[lid];
}

//Blelloch
//exclusive
//https://www.udacity.com/course/viewer#!/c-cs344/l-86719951/e-88035124/m-87690374

__kernel void BlellochScan(
	__global float *buffer,
	__global float *result,
	__local float *lBuffer,
uint n_items)
{
	uint gid = get_global_id(0);
	uint lid = get_local_id(0);
	uint dp = 1;

	//Note half + 1 times as many threads as work items
	lBuffer[2 * lid] = buffer[2 * gid];
	lBuffer[2 * lid + 1] = buffer[2 * gid + 1];

	//Up sweep
	for (uint s = n_items >> 1; s > 0; s >>= 1) {
		barrier(CLK_LOCAL_MEM_FENCE);
		if (lid < s) {
			uint i = dp*(2 * lid + 1) - 1;
			uint j = dp*(2 * lid + 2) - 1;

			//[  i,    j  ]
			//vvvvvvvvvvvvv
			//[  i,  i + j]
			lBuffer[j] += lBuffer[i];
		}

		dp <<= 1;
	}

	if (lid == 0) lBuffer[n_items - 1] = 0;

	//Down sweep
	for (uint s = 1; s < n_items; s <<= 1) {
		dp >>= 1;
		barrier(CLK_LOCAL_MEM_FENCE);

		if (lid < s) {
			uint i = dp*(2 * lid + 1) - 1;
			uint j = dp*(2 * lid + 2) - 1;

			//[  i,    j  ]
			//vvvvvvvvvvvvv
			//[  j,  i + j]
			float t = lBuffer[j];
			lBuffer[j] += lBuffer[i];
			lBuffer[i] = t;
		}
	}

	barrier(CLK_LOCAL_MEM_FENCE);

	result[2 * gid] = lBuffer[2 * lid];
	result[2 * gid + 1] = lBuffer[2 * lid + 1];
}
