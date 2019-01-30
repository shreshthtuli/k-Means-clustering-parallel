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

using namespace std;

struct Point{
    int x; 
    int y; 
    int z;
    int clusterID;
};

int* points;
int* means;
int* all_means;
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
        index = rand()%points_size;
        means[3*i] = points[4*index];
        means[3*i+1] = points[4*index+1];
        means[3*i+2] = points[4*index+2];
    }
}

float distance(int a, int b){
    return sqrt(pow(points[4*a] - points[4*b], 2) + pow(points[4*a+1] - points[4*b+1], 2) + pow(points[4*a+2] - points[4*b+2], 2)); 
}


void find_clusters(){
    float min_dist = INT_MAX;
    int cluster_num = 0;
    float dist;
    for(int i = 0; i < points_size; i++){
        min_dist = INT_MAX;
        for(int j = 0; j < means_size; j++){
            dist = distance(points[i], means[j]);
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
    int num = 0;
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
    means[4*clusterID] = sumx / num;
    means[4*clusterID+1] = sumy / num;
    means[4*clusterID+2] = sumz / num;
    return (sumx == 0 && sumy == 0 && sumz == 0);
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
                sum += distance(points[x], points[y]);
            }
        }
        perf += (sum / (2 * indices.size()));
    }
    cout << "Performance : " << perf << endl;
}

void kmeans_sequential(int N, int K, int* data_points, int** data_point_cluster, int** centroids, int* num_iterations){
    
    points = new int[N*4];
    means = new int[K*3];
    points_size = N;
    means_size = K;
    
    readData(data_points, N);
    init_means(K);

    int iterations = 0;
    bool complete = false;
    while(!complete){
        find_clusters();
        complete = true;
        for(int j = 0; j < K; j++)
            complete = complete && update_cluster(j);
        iterations++;
    }
}
