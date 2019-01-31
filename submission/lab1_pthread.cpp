#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<climits>
#include<algorithm>
#include<cmath>
#include<sstream>
#include <omp.h>
#include <iomanip>
#include "lab1_sequential.h"

using namespace std;

int* points;
vector<int> means;
vector<int> all_means;
int points_size;
int means_size;

int numThreads = 4;
int* sta = new int[numThreads];
int* sto = new int[numThreads];
int* work_done = new int[numThreads];

void readData(int* data_points, int N){
    for(int i = 0; i < N; i++){
        points[4*i] = data_points[3*i];
        points[4*i+1] = data_points[3*i+1];
        points[4*i+2] = data_points[3*i+2];

        points[4*i+3] = 0;
    }
}

void init_means(int num){
    int index;
    for(int i = 0; i < num; i++){
        index = rand()%points_size;
        means.push_back(points[4*index]);
        means.push_back(points[4*index+1]);
        means.push_back(points[4*index+2]);
        // cout << means[3*i] << " " << means[3*i+1] << " " << means[3*i+2] << endl;
    }
}

float distance(int x1, int y1, int z1, int x2, int y2, int z2){
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2)); 
}



void* find_clusters(void *tid){
    int *id = (int*) tid;
    while(work_done[*id] < 2){
        float min_dist = INT_MAX;
        int cluster_num = 0;
        float dist;
        for(int i = sta[*id]; i < sto[*id]; i++){
            min_dist = INT_MAX;
            for(int j = 0; j < means_size; j++){
                dist = distance(points[4*i], points[4*i+1], points[4*i+2], means[3*j], means[3*j+1], means[3*j+2]);
                if(min_dist > dist){
                    min_dist = dist;
                    cluster_num = j;
                }
            }
            
            points[4*i+3] = cluster_num;
        }
        work_done[*id] = 1;
        while(work_done[*id] == 1){}
    }
}


bool update_cluster(int clusterID){
    float sumx = 0, sumy = 0, sumz = 0;
    int num = 0; bool val;
    for(int i = 0; i < points_size; i++){
        if(points[4*i+3] == clusterID){
            sumx += points[4*i];
            sumy += points[4*i+1];
            sumz += points[4*i+2];
            num++; 
        }
    }
    if(num == 0){
        return true;
    }
    val = (means[3*clusterID] == int(sumx/num)) && (means[3*clusterID+1] == int(sumy/num)) && (means[3*clusterID+2] == int(sumz/num));
    means[3*clusterID] = sumx / num;
    means[3*clusterID+1] = sumy / num;
    means[3*clusterID+2] = sumz / num;
    return val;
}

void performance(){
    double perf = 0;
    vector<int> indices;
    for(int i = 0; i < means_size; i++){
        indices.clear();
        // Get all points of cluster i
        for(int j = 0; j < points_size; j++){
            if(points[4*j+3] == i)
                indices.push_back(j);
        }
        // Find sum of distances for all possible y
        double sum = 0;
        for(int x = 0; x < indices.size(); x++){
            for(int y = x+1; y < indices.size(); y++){
                sum += distance(points[4*x], points[4*x+1], points[4*x+2], points[4*y], points[4*y+1], points[4*y+2]);
            }
        }
        perf += (sum / (2 * indices.size()));
    }
    cout << "Performance : " << perf << endl;
}

void kmeans_pthread(int thr, int N, int K, int* data_points, int** data_point_cluster, int** centroids, int* num_iterations){
    
    points = new int[N*4];
    points_size = N;
    means_size = K;

    numThreads = thr;
    
    readData(data_points, points_size);
    init_means(means_size);

    sta = new int[numThreads];
    sto = new int[numThreads];
    work_done = new int[numThreads];

    pthread_t kcluster_thr[numThreads];
    int* tid = new int[numThreads];
    for(int i = 0; i < numThreads; i++){
        tid[i] = i;
        sta[i] = (float(i) / numThreads) * points_size;
        sto[i] = (i == (numThreads - 1)) ? points_size : (float(i + 1) / numThreads) * points_size; 
        work_done[i] = 0;
        pthread_create(&kcluster_thr[i], NULL, find_clusters, &tid[i]);
    }

    bool all_done = false;
    int iterations = 0;
    bool complete = false;
    while(!complete && iterations < N){
        
        // Wait till all thread finish fniding clusters
        all_done = false;
        while(!all_done){
            all_done = true;
            for(int j = 0; j < numThreads; j++){
                if(work_done[j] != 1){
                    all_done = false; break;
                }
            }
        }

        complete = true;
        all_means.insert(std::end(all_means), means.begin(), means.end());
        for(int j = 0; j < means_size; j++){
            complete = complete && update_cluster(j);
        }

        // Tell threads to work again
        for(int j = 0; j < numThreads; j++){
            work_done[j] = 0;
        }
        
        iterations++;
    }

    // Tell all threads to stop
    for(int j = 0; j < numThreads; j++){
        work_done[j] = 2;
        pthread_cancel(kcluster_thr[j]);
    }


    // performance();
    *centroids = all_means.data();
    *data_point_cluster = points;
    *num_iterations = iterations-1;
}
