#!/bin/bash
#SBATCH --job-name=openmp_ASCII
#SBATCH --mem-per-cpu=1G
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=20
#SBATCH --constraint=moles
#SBATCH --mail-type=ALL

export OMP_NUM_THREADS=20

gcc -fopenmp main.c -o openmp_ASCII

./openmp_ASCII
