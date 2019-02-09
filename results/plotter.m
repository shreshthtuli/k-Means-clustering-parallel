seq = [];
pthread_2 = [];
pthread_4 = [];
omp = [];
omp_4 = [];

x = [10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000];

test = "5";
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

s_pthread_2 = seq ./ pthread_2;

h = figure;
hold on;
grid on 
grid minor
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

h1 = figure;
hold on;
grid on 
grid minor
set(h1,'Units','Inches');
% set(gca, 'YScale', 'log')
% set(gca,'ytick',logspace(0,1,100000)-1);
plot(x, seq ./ pthread_2, '-o');
plot(x, seq ./ pthread_4, '-o');
plot(x, seq ./ omp, '-o');
plot(x, seq ./ omp_4, '-o');
title(strcat("Speedup comparison for k = ", test));
xlabel("Number of Points")
ylabel("Speedup")
legend('pThread - 2', 'pThread - 4', 'omp - 2', 'omp - 4')
pos = get(h1,'Position');
set(h1,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
print(h1,strcat("sk",test),'-dpdf','-r0')


h2 = figure;
hold on;
grid on 
grid minor
set(h2,'Units','Inches');
% set(gca, 'YScale', 'log')
% set(gca,'ytick',logspace(0,1,100000)-1);
plot(x, seq ./ (2 * pthread_2), '-o');
plot(x, seq ./ (4 * pthread_4), '-o');
plot(x, seq ./ (2 * omp), '-o');
plot(x, seq ./ (4 * omp_4), '-o');
title(strcat("Efficiency comparison for k = ", test));
xlabel("Number of Points")
ylabel("Efficiency")
legend('pThread - 2', 'pThread - 4', 'omp - 2', 'omp - 4')
pos = get(h2,'Position');
set(h2,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
print(h2,strcat("ek",test),'-dpdf','-r0')
