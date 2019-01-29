@echo off

REM COMPILE ALL CODES
g++ -fopenmp ./k-mean-sequential.cpp -o seq
g++ -fopenmp ./k-mean-pthread.cpp -o pthread
g++ -fopenmp ./k-mean-pthread2.cpp -o pthread2
g++ -fopenmp ./k-mean-omp.cpp -o omp
