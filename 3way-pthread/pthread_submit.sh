#!/bin/bash
#SBATCH --job-name=pthread_ASCII
#SBATCH --mem-per-cpu=1G
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=20
#SBATCH --constraint=moles
#SBATCH --mail-type=ALL


gcc -pthread main.c -o pthread_ASCII



perf stat -r 5 -e task-clock,cache-references,cache-misses,branches,branch-misses ./pthread_ASCII

#sleep 60
