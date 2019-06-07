#!/bin/sh
mpicc quicksortMPI.c -o quicksortMPI
mpiexec.mpich -np 2 ./quicksortMPI