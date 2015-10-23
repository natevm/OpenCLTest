# OpenCL
Contains some Windows OpenCL test projects. 

## Requirements
* Visual Studio (2013 or later)
  * FYI Cuda 7.5 currently lacks support for 2015
* _For NVidia Only_ The Cuda Toolkit 7.5
  * Can be found here: https://developer.nvidia.com/cuda-downloads

## Setup
1. Verify you meet the requirements.
2. Clone this repository.
3. Open up the .sln
4. Right click on "OpenCLTest" in the solution explorer, and click properties.
5. In Configuration Properties -> C/C++ -> General
  * Verify Additional Include Directories contains "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.5\include"
6. In Configuration Properties -> Linker -> General
  * Verify Additional Library Directories contains "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.5\lib\Win32"
7. In Configuration Properties -> Linker -> Input
  * Verify Additional Dependencies includes "OpenCL.lib"
8. You should be able to compile and run the code!

