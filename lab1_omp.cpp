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
vector<float> means;
vector<float> all_means;
int points_size;
int means_size;

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
        index = (float(i)/num)*points_size;
        means.push_back(points[4*index]);
        means.push_back(points[4*index+1]);
        means.push_back(points[4*index+2]);
        // cout << means[3*i] << " " << means[3*i+1] << " " << means[3*i+2] << endl;
    }
}

float distance(int x1, int y1, int z1, int x2, int y2, int z2){
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2)); 
}


void find_clusters(){
    float min_dist = INT_MAX;
    int cluster_num = 0;
    float dist;
    #pragma omp parallel for private(dist, min_dist, cluster_num)
    for(int i = 0; i < points_size; i++){
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
    val = (means[3*clusterID] == sumx/num) && (means[3*clusterID+1] == sumy/num) && (means[3*clusterID+2] == sumz/num);
    means[3*clusterID] = sumx / num;
    means[3*clusterID+1] = sumy / num;
    means[3*clusterID+2] = sumz / num;
    return val;
}

void performance(){
    double perf = 0; int j;
    for(int i = 0; i < points_size; i++){
        j = points[4*i+3];
        perf += distance(points[4*i], points[4*i+1], points[4*i+1], means[3*j], means[3*j+1], means[3*j+2]);
    }
    cerr << "Performance : " << perf << endl;
}

void kmeans_omp(int numThreads, int N, int K, int* data_points, int** data_point_cluster, float** centroids, int* num_iterations){
    
    points = new int[N*4];
    points_size = N;
    means_size = K;

    omp_set_num_threads(numThreads);
    
    readData(data_points, points_size);
    init_means(means_size);

    int iterations = 0;
    bool complete = false;
    while(!complete && iterations < N){
        find_clusters();
        complete = true;
        all_means.insert(std::end(all_means), means.begin(), means.end());
        // #pragma omp parallel for shared(complete)
        for(int j = 0; j < means_size; j++){
            complete = complete && update_cluster(j);
        }
        iterations++;
    }
    performance();
    *centroids = all_means.data();
    *data_point_cluster = points;
    *num_iterations = iterations-1;
}
