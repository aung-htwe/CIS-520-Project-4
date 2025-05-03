#!/bin/bash
#SBATCH --job-name=mpi_ASCII
#SBATCH --mem-per-cpu=1G
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=20
#SBATCH --constraint=moles
#SBATCH --mail-type=ALL

cd /homes/htwe/hw4/3way-mpi/

SRC=main.c

OBJ=mpi_ascii

NUM=20

mpicc -g -O2 -o $OBJ $SRC

mpirun -n $NUM ./$OBJ /homes/dan/625/wiki_dump.txt


#rm $OBJ
