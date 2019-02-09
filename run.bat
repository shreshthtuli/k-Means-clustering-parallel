g++ -fopenmp -o seq main_sequential.c lab1_io.c lab1_sequential.cpp
g++ -fopenmp -o omp main_omp.c lab1_io.c lab1_omp.cpp
g++ -fopenmp -o pthread main_pthread.c lab1_io.c lab1_pthread.cpp

FOR %%K IN (2, 5, 10) DO (
    echo. > results/%%K.txt
    echo. 2> results/%%K-p.txt
    FOR /L %%N IN (100000, 100000, 1000000) DO (
        python ./test/data-gen.py %%K %%N
    )
    FOR /L %%N IN (100000, 100000, 1000000) DO (
        .\seq.exe %%K data/%%K-%%N.dat dump/seq-%%K-%%N-points.txt dump/seq-%%K-%%N-means.txt >> results/%%K.txt 2>> results/%%K-p.txt
    )
    echo. >> results/%%K.txt 
    echo. 2>> results/%%K-p.txt
    FOR /L %%N IN (100000, 100000, 1000000) DO (
        .\pthread.exe %%K 2 data/%%K-%%N.dat dump/p2-%%K-%%N-points.txt dump/p2-%%K-%%N-means.txt >> results/%%K.txt 2>> results/%%K-p.txt
    )
    echo. >> results/%%K.txt
    echo. 2>> results/%%K-p.txt
    FOR /L %%N IN (100000, 100000, 1000000) DO (
        .\pthread.exe %%K 4 data/%%K-%%N.dat dump/p4-%%K-%%N-points.txt dump/p4-%%K-%%N-means.txt >> results/%%K.txt 2>> results/%%K-p.txt
    )
    echo. >> results/%%K.txt
    echo. 2>> results/%%K-p.txt
    FOR /L %%N IN (100000, 100000, 1000000) DO (
        .\pthread.exe %%K 6 data/%%K-%%N.dat dump/p4-%%K-%%N-points.txt dump/p4-%%K-%%N-means.txt >> results/%%K.txt 2>> results/%%K-p.txt
    )
    echo. >> results/%%K.txt
    echo. 2>> results/%%K-p.txt    
    FOR /L %%N IN (100000, 100000, 1000000) DO (
        .\pthread.exe %%K 8 data/%%K-%%N.dat dump/p4-%%K-%%N-points.txt dump/p4-%%K-%%N-means.txt >> results/%%K.txt 2>> results/%%K-p.txt
    )
    echo. >> results/%%K.txt
    echo. 2>> results/%%K-p.txt
    FOR /L %%N IN (100000, 100000, 1000000) DO (
        .\omp.exe %%K 2 data/%%K-%%N.dat dump/o2-%%K-%%N-points.txt dump/o2-%%K-%%N-means.txt >> results/%%K.txt 2>> results/%%K-p.txt
    )
    echo. >> results/%%K.txt
    echo. 2>> results/%%K-p.txt
    FOR /L %%N IN (100000, 100000, 1000000) DO (
        .\omp.exe %%K 4 data/%%K-%%N.dat dump/o4-%%K-%%N-points.txt dump/o4-%%K-%%N-means.txt >> results/%%K.txt 2>> results/%%K-p.txt
    )
    echo. >> results/%%K.txt
    echo. 2>> results/%%K-p.txt
    FOR /L %%N IN (100000, 100000, 1000000) DO (
        .\omp.exe %%K 6 data/%%K-%%N.dat dump/o4-%%K-%%N-points.txt dump/o4-%%K-%%N-means.txt >> results/%%K.txt 2>> results/%%K-p.txt
    )
    echo. >> results/%%K.txt
    echo. 2>> results/%%K-p.txt    
    FOR /L %%N IN (100000, 100000, 1000000) DO (
        .\omp.exe %%K 8 data/%%K-%%N.dat dump/o4-%%K-%%N-points.txt dump/o4-%%K-%%N-means.txt >> results/%%K.txt 2>> results/%%K-p.txt
    )
    echo. >> results/%%K.txt
    echo. 2>> results/%%K-p.txt
)