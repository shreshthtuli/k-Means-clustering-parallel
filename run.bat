rm ./*.exe *.txt *.exe.stackdump ./*.dat
rm ./data/*

REM COMPILE ALL CODES
g++ -fopenmp ./k-mean-sequential.cpp -o seq
g++ -fopenmp ./k-mean-pthread.cpp -o pthread
g++ -fopenmp ./k-mean-pthread2.cpp -o pthread2
g++ -fopenmp ./k-mean-pthread3.cpp -o pthread3
g++ -fopenmp ./k-mean-omp.cpp -o omp


FOR %%K IN (5, 10, 20, 50, 100) DO (
    echo. > results/%%K.txt
    FOR /L %%N IN (1000, 1000, 10000) DO (
        python ./data-gen.py %%K %%N
    )
    FOR /L %%N IN (1000, 1000, 10000) DO (
        seq.exe %%K %%N
    )
    echo. >> results/%%K.txt
    FOR /L %%N IN (1000, 1000, 10000) DO (
        pthread.exe %%K %%N 2
    )
    echo. >> results/%%K.txt
    FOR /L %%N IN (1000, 1000, 10000) DO (
        pthread.exe %%K %%N 4
    )
    echo. >> results/%%K.txt
    FOR /L %%N IN (1000, 1000, 10000) DO (
        pthread2.exe %%K %%N 2
    )
    echo. >> results/%%K.txt
    FOR /L %%N IN (1000, 1000, 10000) DO (
        pthread2.exe %%K %%N 4
    )
    echo. >> results/%%K.txt
    FOR /L %%N IN (1000, 1000, 10000) DO (
        pthread3.exe %%K %%N 2
    )
    echo. >> results/%%K.txt
    FOR /L %%N IN (1000, 1000, 10000) DO (
        pthread3.exe %%K %%N 4
    )
    echo. >> results/%%K.txt
    FOR /L %%N IN (1000, 1000, 10000) DO (
        omp.exe %%K %%N
    )
   echo. >> results/%%K.txt
)