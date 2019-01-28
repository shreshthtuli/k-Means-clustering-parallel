#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<climits>
#include<algorithm>
#include<cmath>
#include<sstream>
#include <omp.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t lock;
int numThreads = 4;

struct Point{
    int x; 
    int y; 
    int z;
    int clusterID;
};

vector<Point*> points;
vector<Point*> means;

int max_val = INT_MIN, min_val = INT_MAX;

void readData(string filename){
    ifstream file(filename);
    string line;
    int x, y, z; istringstream ss;
    while(getline(file, line)){
        // cout << line;
        Point* p = new Point;
        istringstream ss(line);
        ss >> p->x;
        ss >> p->y;
        ss >> p->z;
        // cout << p->x << " " <<  p->y << " " <<  p->z << endl;
        p->clusterID = 0;
        max_val = max({max_val, p->x, p->y, p->z});
        min_val = min({min_val, p->x, p->y, p->z});
        points.push_back(p);
    }
    file.close();
}

void init_means(int num){
    for(int i = 0; i < num; i++){
        Point* p = new Point;
        p->x = min_val + (float(i)/num)*(max_val - min_val);
        p->y = min_val + (float(i)/num)*(max_val - min_val);
        p->z = min_val + (float(i)/num)*(max_val - min_val);
        // cout << p->x << " " <<  p->y << " " <<  p->z << endl;
        means.push_back(p);
    }
}

float distance(Point a, Point b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)); 
}


void* find_clusters(void *tid){
    int *id = (int*) tid;
    int start = (*id / numThreads) * points.size();
    int stop = (*id == (numThreads - 1)) ? points.size() : ((*id + 1) / numThreads) * points.size(); 
    float min_dist = INT_MAX;
    int cluster_num = 0;
    float dist;
    for(int i = start; i < stop; i++){
        min_dist = INT_MAX;
        for(int j = 0; j < means.size(); j++){
            dist = distance(*points[i], *means[j]);
            if(min_dist > dist){
                min_dist = dist;
                cluster_num = i;
            }
        }
        points[i]->clusterID = cluster_num;
    }
}


void update_cluster(int clusterID){
    float sumx = 0, sumy = 0, sumz = 0;
    int num = 0;
    for(int i = 0; i < points.size(); i++){
        if(points[i]->clusterID == clusterID){
            sumx += points[i]->x;
            sumy += points[i]->y;
            sumz += points[i]->z;
            num++; 
        }
    }
    if(num == 0){
        return;
    }
    means[clusterID]->x = sumx / num;
    means[clusterID]->y = sumy / num;
    means[clusterID]->z = sumz / num;
}

void print_means(){
    for(int i = 0; i < means.size(); i++){
        cout << "Means " << i << " : (" << means[i]->x << ", " << means[i]->y << ", " << means[i]->z << ")\n";
    }
    cout << endl;
}

void print_points(){
    for(int i = 0; i < points.size(); i++){
        cout << "Point " << i << " : (" << points.at(i)->x << ", " << points[i]->y << ", " << points[i]->z << ") -> " << points[i]->clusterID << endl;
    }
    cout << endl;
}

int main(int argc, char** argv){
    readData("points.dat");
    init_means(10);
    // print_means();
    double start;
    start = omp_get_wtime();

    pthread_t kcluster_thr[numThreads];
    int* tid = new int[numThreads];
    for(int i = 0; i < numThreads; i++)
        tid[i] = i;

    pthread_mutex_init(&lock, NULL);

    for(int i = 0; i < 100; i++){
        // cout << "Iteration "  << i << "\n";
        for(int j = 0; j < numThreads; j++){
            pthread_create(&kcluster_thr[j], NULL, find_clusters, &tid[j]);
        }
        for(int j = 0; j < numThreads; j++){
            pthread_join(kcluster_thr[j], NULL);
        }
        // print_points();
        for(int j = 0; j < means.size(); j++){
            update_cluster(j);
        }
        // print_means();
    }

    cout << "Time : " << (omp_get_wtime() - start) << endl;
    print_means();
    return 0;
}