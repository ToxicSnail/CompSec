#!/bin/bash
#SBATCH --job-name=OMPJob
#SBATCH --output=OMP.out
#SBATCH --time=00:10:00
#SBATCH --cpus-per-task=6
#SBATCH --mem-per-cpu=100M

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

gcc -O2 -fopenmp integrate.c -o integrate -lm

# export OMP_SCHEDULE="dynamic,4"

srun ./integrate
