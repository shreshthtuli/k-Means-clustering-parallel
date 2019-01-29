seq = [];
pthread_2 = [];
pthread_4 = [];
pthread2_2 = [];
pthread2_4 = [];
omp = [];

x = [1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000];

file1 = "5.txt";
f1 = fopen(file1, 'rt');

fgetl(f1);

for i = 1:10    
    temp = fgetl(f1);
    seq = [seq; str2double(temp)];
end
fgetl(f1);
for i = 1:10    
    temp = fgetl(f1);
    pthread_2 = [pthread_2; str2double(temp)];
end
fgetl(f1);
for i = 1:10    
    temp = fgetl(f1);
    pthread_4 = [pthread_4; str2double(temp)];
end
fgetl(f1);
for i = 1:10    
    temp = fgetl(f1);
    pthread2_2 = [pthread2_2; str2double(temp)];
end
fgetl(f1);
for i = 1:10    
    temp = fgetl(f1);
    pthread2_4 = [pthread2_4; str2double(temp)];
end
fgetl(f1);
for i = 1:10    
    temp = fgetl(f1);
    omp = [omp; str2double(temp)];
end
   
fgetl(f1);

pthread2_2 = seq ./ pthread2_2;
pthread2_4 = seq ./ pthread2_4;
pthread_2 = seq ./ pthread_2;
pthread_4 = seq ./pthread_4;
omp = seq ./omp;

hold on;
% plot(x, seq, '-o');
plot(x, pthread_2, '-o');
plot(x, pthread_4, '-o');
plot(x, pthread2_2, '-o');
plot(x, pthread2_4, '-o');
legend('Seq', 'pThread - 2', 'pThread - 4', 'pThread2 - 2', 'pThread2 - 4', 'omp')
