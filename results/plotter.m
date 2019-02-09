seq = [];
pthread_2 = [];
pthread_4 = [];
omp = [];
omp_4 = [];

x = [10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000];

test = "10";
file1 = strcat(test,".txt");
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
    omp = [omp; str2double(temp)];
end
fgetl(f1);
for i = 1:10    
    temp = fgetl(f1);
    omp_4 = [omp_4; str2double(temp)];
end

fgetl(f1);
% 
% pthread2_2 = seq ./ pthread2_2;
% pthread2_4 = seq ./ pthread2_4;
% pthread_2 = seq ./ pthread_2;
% pthread_4 = seq ./pthread_4;
% omp = seq ./omp;

h = figure;
hold on;
grid on 
%grid minor
set(h,'Units','Inches');
% set(gca, 'YScale', 'log')
% set(gca,'ytick',logspace(0,1,100000)-1);
plot(x, seq, '-o');
plot(x, pthread_2, '-o');
plot(x, pthread_4, '-o');
plot(x, omp, '-o');
plot(x, omp_4, '-o');
title(strcat("Performance comparison for k = ", test));
xlabel("Number of Points")
ylabel("Time in seconds")
legend('Seq', 'pThread - 2', 'pThread - 4', 'omp - 2', 'omp - 4')
pos = get(h,'Position');
set(h,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
print(h,strcat("k",test),'-dpdf','-r0')
