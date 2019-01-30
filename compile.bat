g++ -fopenmp -o seq main_sequential.c lab1_io.c lab1_sequential.cpp
g++ -fopenmp -o omp main_omp.c lab1_io.c lab1_omp.cpp
g++ -fopenmp -o pthread main_pthread.c lab1_io.c lab1_pthread.cpp