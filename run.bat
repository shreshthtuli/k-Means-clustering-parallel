rm ./*.exe *.txt *.exe.stackdump ./*.dat

REM COMPILE ALL CODES
g++ -fopenmp ./k-mean-sequential.cpp -o seq
g++ -fopenmp ./k-mean-pthread.cpp -o pthread
g++ -fopenmp ./k-mean-pthread2.cpp -o pthread2
g++ -fopenmp ./k-mean-omp.cpp -o omp


FOR %%K IN (5,10,20,50) DO (
    echo. 2>%%K.txt
    FOR %%N IN (0, 10000, 1000) DO (
        python ./data-gen.py %%K %%N
        seq.exe %%K >> %%K.txt
        echo \n >> %%K.txt
        pthread.exe %%K 2 >> %%K.txt
        echo \n >> %%K.txt
        pthread.exe %%K 4 >> %%K.txt
        echo \n >> %%K.txt
        pthread2.exe %%K >> %%K.txt
        echo \n >> %%K.txt
        omp.exe %%K >> %%K.txt
        echo \n >> %%K.txt
    )
)
