#!/bin/bash
#SBATCH --job-name=mpi_ASCII
#SBATCH --mem-per-cpu=1g
#SBATCH --nodes=2
#SBATCH --tasks-per-node=20
#SBATCH --constraint=moles
#SBATCH --mail-type=ALL

module load OpenMPI/4.1.4-GCC-11.3.0


SRC=main.c

OBJ=mpi_ascii

# Number of threads
NUM=8

mpicc -g -o $OBJ $SRC

mpirun -n $NUM ./$OBJ /homes/dan/625/wiki_dump.txt

perf stat -r 5 -e task-clock,cache-references,cache-misses  ./$OBJ /homes/dan/625/wiki_dump.txt


