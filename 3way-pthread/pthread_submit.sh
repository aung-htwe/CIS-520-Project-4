#!/bin/bash
#SBATCH --job-name=pthread_ASCII
#SBATCH --mem-per-cpu=1G
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=20
#SBATCH --constraint=moles
#SBATCH --mail-type=ALL


$HOME/hw4/build/3way-pthread/pthread_ascii

perf stat echo "Initializing perf"

perf stat -e cycles,instructions,cache-references,cache-misses,branches,branch-misses 
