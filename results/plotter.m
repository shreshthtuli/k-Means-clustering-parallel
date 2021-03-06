seq = [];
pthread_2 = [];
pthread_4 = [];
pthread_6 = [];
pthread_8 = [];

omp_2 = [];
omp_4 = [];
omp_6 = [];
omp_8 = [];

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
    pthread_6 = [pthread_6; str2double(temp)];
end
fgetl(f1);
for i = 1:10    
    temp = fgetl(f1);
    pthread_8 = [pthread_8; str2double(temp)];
end
fgetl(f1);
for i = 1:10    
    temp = fgetl(f1);
    omp_2 = [omp_2; str2double(temp)];
end
fgetl(f1);
for i = 1:10    
    temp = fgetl(f1);
    omp_4 = [omp_4; str2double(temp)];
end
fgetl(f1);
for i = 1:10    
    temp = fgetl(f1);
    omp_6 = [omp_6; str2double(temp)];
end
fgetl(f1);
for i = 1:10    
    temp = fgetl(f1);
    omp_8 = [omp_8; str2double(temp)];
end
fgetl(f1);


% PLOTTING:: 

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
plot(x, pthread_8, '-o');
plot(x, omp_2, '-o');
plot(x, omp_4, '-o');
plot(x, omp_8, '-o');
title(strcat("Performance comparison for k = ", test));
xlabel("Number of Points")
ylabel("Time in seconds")
legend('Seq', 'pThread - 2', 'pThread - 4', 'pThread - 8', 'omp - 2', 'omp - 4', 'omp - 8')
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
plot(x, seq ./ pthread_8, '-o');
plot(x, seq ./ omp_2, '-o');
plot(x, seq ./ omp_4, '-o');
plot(x, seq ./ omp_8, '-o');
title(strcat("Speedup comparison for k = ", test));
xlabel("Number of Points")
ylabel("Speedup")
legend('pThread - 2', 'pThread - 4', 'pThread - 8', 'omp - 2', 'omp - 4', 'omp - 8')
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
plot(x, seq ./ (8 * pthread_8), '-o');
plot(x, seq ./ (2 * omp_2), '-o');
plot(x, seq ./ (4 * omp_4), '-o');
plot(x, seq ./ (8 * omp_8), '-o');
title(strcat("Efficiency comparison for k = ", test));
xlabel("Number of Points")
ylabel("Efficiency")
legend('pThread - 2', 'pThread - 4', 'pThread - 8', 'omp - 2', 'omp - 4', 'omp - 8')
pos = get(h2,'Position');
set(h2,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
print(h2,strcat("ek",test),'-dpdf','-r0')


% PLOTTING threads 

threads = [2, 4, 6, 8];

h3 = figure;
hold on;
grid on 
grid minor
set(h3,'Units','Inches');
plot(threads, [mean(pthread_2), mean(pthread_4), mean(pthread_6), mean(pthread_8)], '-o');
plot(threads, [mean(omp_2), mean(omp_4), mean(omp_6), mean(omp_8)], '-o');
title(strcat("Performance comparison for k = ", test));
xlabel("Number of Threads")
ylabel("Time in seconds")
legend('pThread', 'omp')
pos = get(h3,'Position');
set(h3,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
print(h3,strcat("tk",test),'-dpdf','-r0')

h4 = figure;
hold on;
grid on 
grid minor
set(h4,'Units','Inches');
plot(threads, [mean(seq./pthread_2), mean(seq./pthread_4), mean(seq./pthread_6), mean(seq./pthread_8)], '-o');
plot(threads, [mean(seq./omp_2), mean(seq./omp_4), mean(seq./omp_6), mean(seq./omp_8)], '-o');
title(strcat("Speedup comparison for k = ", test));
xlabel("Number of Threads")
ylabel("Speedup")
legend('pThread', 'omp')
pos = get(h4,'Position');
set(h4,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
print(h4,strcat("tsk",test),'-dpdf','-r0')

h5 = figure;
hold on;
grid on 
grid minor
set(h5,'Units','Inches');
plot(threads, [mean(seq./(2*pthread_2)), mean(seq./(4*pthread_4)), mean(seq./(6*pthread_6)), mean(seq./(8*pthread_8))], '-o');
plot(threads, [mean(seq./(2*omp_2)), mean(seq./(4*omp_4)), mean(seq./(6*omp_6)), mean(seq./(8*omp_8))], '-o');
title(strcat("Efficiency comparison for k = ", test));
xlabel("Number of Threads")
ylabel("Efficiency")
legend('pThread', 'omp')
pos = get(h5,'Position');
set(h5,'PaperPositionMode','Auto','PaperUnits','Inches','PaperSize',[pos(3), pos(4)])
print(h5, strcat("tek",test),'-dpdf','-r0')



