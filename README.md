Each implementation of parallel programming has a CMakeLists.txt file that builds an executable using its respective main.c file.
There is a top level CMakeLists.txt file that is used to build all implementations for easier building. To build an executable, create a 
and enter "cmake .." in the build folder. After doing so, enter "make". An executable for each implementation should then be built.

To schedule a job in beocat, an *implementation*_submit.sh file is provided for each implementation. For examble, openmp_submit.sh.
The following command is used to schedule a job:
          sbatch *implementation*_submit.sh
where *implementation* is either pthread, mpi, or openmp.

The following contain instructions on how to change node/core configuration for each implementation:

FOR PTHREADS
Changing Node Count:
          Open pthread_submit.sh
          Change the value on the 4th line: #SBATCH --nodes=2

Changing Core Count:
          Open pthread_submit.sh
          Change the value on the 5th line: #SBATCH --ntasks-per-node=20

Changing Thread Count:
          open main.c
          Adjust the defined constaant NUM_THREADS on line 6


FOR MPI
Changing Node Count:
          Open mpi_submit.sh
          Change the value on the 4th line: $SBATCH --nodes=1

Changing Core Count:
          Open mpi_submit.sh
          Change the value on the 5th line: #SBATCH --ntasks-per-node=20

Changing Thread Count:
          Open mpi_submit.sh
          Change the value on the 17th line: NUM=8



FOR OPENMP
Changing Node Count:
          Open openmp_submit.sh
          Change the value on the 4th line: #SBATCH --nodes=1

Changing Core Count:
          Open openmp_submit.sh
          Change the value on the 5th line: #SBATCH --ntasks-per-node=20

Changing Thread Count:
          By default, thread count is equal to core count. Changing core count changes thread count.





