#!/bin/bash
#PBS -q qprod
#PBS -N EXP
#PBS -l select=1:ncpus=16:mpiprocs=16,walltime=12:00:00
#PBS -J 1-6
#PBS -A OPEN-8-36

module load openmpi

cd $PBS_O_WORKDIR

mpiexec ./2dca_evol
