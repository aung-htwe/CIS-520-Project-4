#!/bin/bash
#SBATCH --job-name=openmp_ASCII
#SBATCH --mem-per-cpu=1G
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=20
#SBATCH --constraint=moles
#SBATCH --mail-type=ALL

export OMP_NUM_THREADS=$SLURM_CPUS_ON_NODE

gcc -fopenmp main.c -o openmp_ASCII


perf stat -r 5 -e task-clock,cache-references,cache-misses,branches,branch-misses ./openmp_ASCII


#sleep 60
