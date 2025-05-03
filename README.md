Each implementation of parallel programming has a CMakeLists.txt file that builds an executable using its respective main.c file.
There is a top level CMakeLists.txt file that is used to build all implementations for easier building. To build an executable, create a 
and enter "cmake .." in the build folder. After doing so, enter "make". An executable for each implementation should then be built.

To schedule a job in beocat, an *implementation*_submit.sh file is provided for each implementation. For examble, openmp_submit.sh.
The following command is used to schedule a job:
          sbatch *implementation*_submit.sh
where *implementation* is either pthread, mpi, or openmp.

Inside each submit.sh file contains the configuration used for running the code. In this file, the node, core, and memory configuration
can be changed.


