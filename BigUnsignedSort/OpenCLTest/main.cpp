//Following https://anteru.net/2012/11/03/2009/

#include <utility>
#include <CL\cl.hpp>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <bitset>
#include <chrono>

#include "bigint\BigUnsigned_UnitTest.h"
std::string GetPlatformName(cl_platform_id id)
{
	size_t size = 0;
	clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, nullptr, &size);

	std::string result;
	result.resize(size);
	clGetPlatformInfo(id, CL_PLATFORM_NAME, size,
		const_cast<char*> (result.data()), nullptr);

	return result;
}

std::string GetDeviceName(cl_device_id id)
{
	size_t size = 0;
	clGetDeviceInfo(id, CL_DEVICE_NAME, 0, nullptr, &size);

	std::string result;
	result.resize(size);
	clGetDeviceInfo(id, CL_DEVICE_NAME, size,
		const_cast<char*> (result.data()), nullptr);

	return result;
}

void CheckError(cl_int error)
{
	if (error != CL_SUCCESS) {
		std::cerr << "OpenCL call failed with error " << error << std::endl;
		if (error != CL_BUILD_PROGRAM_FAILURE)
			std::exit(1);
	}
}
void checkBuildError(cl_int err, cl_program program, cl_device_id deviceid) {
	if (err == CL_BUILD_PROGRAM_FAILURE) {
		// Determine the size of the log
		size_t log_size;
		clGetProgramBuildInfo(program, deviceid, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
		char *log = (char *)malloc(log_size);

		// Get the log
		clGetProgramBuildInfo(program, deviceid, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
		printf("%s\n", log);
		std::exit(1);
	}
}
std::string LoadKernel(const char* name)
{
	std::ifstream in(name);
	std::string result(
		(std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	return result;
}

cl_program CreateProgram(const std::string& source, cl_context context)
{
	size_t lengths[1] = { source.size() };
	const char* sources[1] = { source.data() };

	cl_int error = 0;
	cl_program program = clCreateProgramWithSource(context, 1, sources, lengths, &error);
	CheckError(error);

	return program;
}


int main()
{
	testBigUnsigned();
	std::getchar();


	std::ofstream myfile;
	myfile.open("example.txt");


	//--------------------------------- GETTING PLATFORM IDS --------------------------------------
	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetPlatformIDs.html
#pragma region Getting_Platform_IDS
	cl_uint platformIdCount = 0;
	clGetPlatformIDs(0, nullptr, &platformIdCount);

	if (platformIdCount == 0) {
		std::cerr << "No OpenCL platform found" << std::endl;
		return 1;
	}
	else {
		myfile << "Found " << platformIdCount << " platform(s)" << std::endl;
	}

	std::vector<cl_platform_id> platformIds(platformIdCount);
	clGetPlatformIDs(platformIdCount, platformIds.data(), nullptr);

	for (cl_uint i = 0; i < platformIdCount; ++i) {
		myfile << "\t (" << (i + 1) << ") : " << GetPlatformName(platformIds[i]) << std::endl;
	}
#pragma endregion Getting_Platform_IDS


	//---------------------------------- GETTING DEVICE IDS ---------------------------------------
	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceIDs.html
#pragma region Getting_Device_IDS
	cl_uint deviceIdCount = 0;
	clGetDeviceIDs(platformIds[platformIds.size() - 1], CL_DEVICE_TYPE_ALL, 0, nullptr,
		&deviceIdCount);

	if (deviceIdCount == 0) {
		std::cerr << "No OpenCL devices found" << std::endl;
		return 1;
	}
	else {
		myfile << "Found " << deviceIdCount << " device(s) for " + GetPlatformName(platformIds[platformIds.size() - 1]) << std::endl;
	}

	std::vector<cl_device_id> deviceIds(deviceIdCount);
	clGetDeviceIDs(platformIds[platformIds.size() - 1], CL_DEVICE_TYPE_ALL, deviceIdCount,
		deviceIds.data(), nullptr);

	for (cl_uint i = 0; i < deviceIdCount; ++i) {
		myfile << "\t (" << (i + 1) << ") : " << GetDeviceName(deviceIds[i]) << std::endl;
	}
#pragma endregion Getting_Device_IDS


	//------------------------------------ CREATING CONTEXT ----------------------------------------
	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateContext.html
#pragma region Creating_Context	
	myfile << "Creating a context: ";
	const cl_context_properties contextProperties[] =
	{
		CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties> (platformIds[platformIds.size() - 1]),
		0, 0
	};

	cl_int error = CL_SUCCESS;
	cl_context context = clCreateContext(contextProperties, deviceIdCount,
		deviceIds.data(), nullptr, nullptr, &error);
	CheckError(error);

	myfile << "SUCCESS" << std::endl;

#pragma endregion Creating_Context


	//--------------------------------- CREATING COMMAND QUEUE -------------------------------------
	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateCommandQueue.html
#pragma region Creating_Command_Queue
	myfile << "Creating a command queue: ";

	cl_command_queue queue = clCreateCommandQueue(context, deviceIds[0],
		0, &error);
	CheckError(error);
	myfile << "SUCCESS" << std::endl;

#pragma endregion Creating_Command_Queue

	//------------------------------------ CREATING PROGRAM ----------------------------------------
#pragma region Creating_Program	
	myfile << "Creating a program kernel: ";
	

	std::vector<std::string> sources;

	const int numFiles = 2;
	std::vector<std::string> files;
	files.push_back("bigint\\BigUnsigned.c");
	files.push_back("kernels.cl");
	std::vector<size_t> sourceLengths;

	for (int i = 0; i < numFiles; ++i){
		std::string k = LoadKernel(files[i].c_str());
		sources.push_back(k);
		sourceLengths.push_back((size_t)k.size());
	}

	std::vector<const char*> cstrings;
	for (size_t i = 0; i < sources.size(); ++i)
		cstrings.push_back(sources[i].data());

	cl_program program = clCreateProgramWithSource(context, numFiles, &cstrings[0], &sourceLengths[0], &error);
		//(LoadKernel("kernels.cl"), context);

	checkBuildError(clBuildProgram(program, deviceIdCount, deviceIds.data(), nullptr, nullptr, nullptr), program, deviceIds[0]);


	//Predication
	cl_kernel predicateKernel = clCreateKernel(program, "Predicate", &error);
	CheckError(error);
	myfile << "SUCCESS" << std::endl;
	//Scanning
	cl_kernel scanKernel = clCreateKernel(program, "StreamScan", &error);
	CheckError(error);
	myfile << "SUCCESS" << std::endl;
	//Sorting
	cl_kernel sortKernel = clCreateKernel(program, "Sort", &error);
	CheckError(error);
	myfile << "SUCCESS" << std::endl;


	// Prepare some test data
	myfile << "Preparing test data: ";
	int numBits = 14;
	static const size_t inputSize = 1 << numBits;//1 << numBits;
	static const size_t workgroupSize = 1024;
	std::vector<int> input(inputSize), intermediate(inputSize / workgroupSize), predicate(inputSize), address(inputSize), result(inputSize);
	for (int i = 0; i < inputSize; ++i)
		input[i] = static_cast<int> (inputSize - i);
	for (int i = 0; i < inputSize / workgroupSize; ++i)
		intermediate[i] = static_cast<int>(-1);
	cl_mem inputBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		sizeof (int)* (inputSize),
		input.data(), &error);
	cl_mem intermediateBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		sizeof (int)* (inputSize / workgroupSize),
		intermediate.data(), &error);
	cl_mem lPredicateBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		sizeof (int)* (inputSize),
		predicate.data(), &error);
	cl_mem rPredicateBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		sizeof (int)* (inputSize),
		predicate.data(), &error);
	cl_mem leftBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		sizeof (int)* (inputSize),
		address.data(), &error);
	cl_mem rightBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		sizeof (int)* (inputSize),
		address.data(), &error);
	cl_mem resultBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		sizeof (int)* (inputSize),
		result.data(), &error);
	CheckError(error);
#pragma endregion Creating_Program

	//----------------------------------- RADIX SORT ---------------------------------------
	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clEnqueueNDRangeKernel.html
#pragma region Defining_Dimension
	const size_t globalWorkSize[] = { inputSize, 0, 0 };
	const size_t localWorkSize[] = { workgroupSize, 0, 0 };

	//unsigned int index = 1;
	int compared;

	auto t1 = std::chrono::high_resolution_clock::now();

	for (int index = 0; index <= numBits; index++) {
		//Predication
		compared = 0;
		clSetKernelArg(predicateKernel, 0, sizeof (cl_mem), &inputBuffer);
		clSetKernelArg(predicateKernel, 1, sizeof (cl_mem), &lPredicateBuffer);
		clSetKernelArg(predicateKernel, 2, sizeof (unsigned int), &index);
		clSetKernelArg(predicateKernel, 3, sizeof (int), &compared);
		CheckError(clEnqueueNDRangeKernel(queue, predicateKernel, 1,
			nullptr,
			globalWorkSize,
			localWorkSize,
			0, nullptr, nullptr));
		compared = 1;
		clSetKernelArg(predicateKernel, 0, sizeof (cl_mem), &inputBuffer);
		clSetKernelArg(predicateKernel, 1, sizeof (cl_mem), &rPredicateBuffer);
		clSetKernelArg(predicateKernel, 2, sizeof (unsigned int), &index);
		clSetKernelArg(predicateKernel, 3, sizeof (int), &compared);
		CheckError(clEnqueueNDRangeKernel(queue, predicateKernel, 1,
			nullptr,
			globalWorkSize,
			localWorkSize,
			0, nullptr, nullptr));
		//Scanning
		intermediateBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof (int)* (inputSize / workgroupSize),
			intermediate.data(), &error);
		clSetKernelArg(scanKernel, 0, sizeof (cl_mem), &lPredicateBuffer);
		clSetKernelArg(scanKernel, 1, sizeof (cl_mem), &leftBuffer);
		clSetKernelArg(scanKernel, 2, sizeof (cl_mem), &intermediateBuffer);
		clSetKernelArg(scanKernel, 3, workgroupSize * sizeof(cl_int), NULL);
		clSetKernelArg(scanKernel, 4, workgroupSize * sizeof(cl_int), NULL);
		CheckError(clEnqueueNDRangeKernel(queue, scanKernel, 1,
			nullptr,
			globalWorkSize,
			localWorkSize,
			0, nullptr, nullptr));
		intermediateBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof (int)* (inputSize / workgroupSize),
			intermediate.data(), &error);
		clSetKernelArg(scanKernel, 0, sizeof (cl_mem), &rPredicateBuffer);
		clSetKernelArg(scanKernel, 1, sizeof (cl_mem), &rightBuffer);
		clSetKernelArg(scanKernel, 2, sizeof (cl_mem), &intermediateBuffer);
		clSetKernelArg(scanKernel, 3, workgroupSize * sizeof(cl_int), NULL);
		clSetKernelArg(scanKernel, 4, workgroupSize * sizeof(cl_int), NULL);
		CheckError(clEnqueueNDRangeKernel(queue, scanKernel, 1,
			nullptr,
			globalWorkSize,
			localWorkSize,
			0, nullptr, nullptr));
		//Sorting
		clSetKernelArg(sortKernel, 0, sizeof (cl_mem), &inputBuffer);
		clSetKernelArg(sortKernel, 1, sizeof (cl_mem), &lPredicateBuffer);
		clSetKernelArg(sortKernel, 2, sizeof (cl_mem), &leftBuffer);
		clSetKernelArg(sortKernel, 3, sizeof (cl_mem), &rightBuffer);
		clSetKernelArg(sortKernel, 4, sizeof (unsigned int), &inputSize);
		CheckError(clEnqueueNDRangeKernel(queue, sortKernel, 1,
			nullptr,
			globalWorkSize,
			localWorkSize,
			0, nullptr, nullptr));
		clWaitForEvents(1, nullptr);
	}

	CheckError(clEnqueueReadBuffer(queue, inputBuffer, CL_TRUE, 0,
		inputSize * sizeof (int),
		result.data(),
		0, nullptr, nullptr));
	for (int i = 0; i < inputSize; ++i){
		//std::cout << (std::bitset< 16 >(result[i])) << std::endl;
		std::cout << (std::to_string(result[i])) << std::endl;
	}

	auto t2 = std::chrono::high_resolution_clock::now();

#pragma endregion Defining_Dimension


	//----------------------------------- RETRIEVING RESULTS ---------------------------------------
	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clEnqueueReadBuffer.html
#pragma region Retrieving_Result
	CheckError(clEnqueueReadBuffer(queue, inputBuffer, CL_TRUE, 0,
		inputSize * sizeof (int),
		result.data(),
		0, nullptr, nullptr));

	myfile << "\nResult:" << std::endl;

	/*for (int i = 0; i < inputSize; ++i){
	myfile << (std::to_string(result[i])) << std::endl;
	}*/

	auto diff = t2 - t1;
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff);

	myfile << "Parallel radix takes " << (ms.count()) << " milliseconds" << std::endl;
#pragma endregion Retrieving_Results

	clReleaseCommandQueue(queue);
	clReleaseMemObject(leftBuffer);
	clReleaseMemObject(rightBuffer);
	clReleaseMemObject(lPredicateBuffer);
	clReleaseMemObject(rPredicateBuffer);
	clReleaseMemObject(inputBuffer);
	clReleaseMemObject(intermediateBuffer);
	clReleaseKernel(scanKernel);
	clReleaseKernel(predicateKernel);
	clReleaseProgram(program);
	clReleaseContext(context);


	myfile.close();

	std::cout << "\nDone! Press enter to exit.";
	std::getchar();
}