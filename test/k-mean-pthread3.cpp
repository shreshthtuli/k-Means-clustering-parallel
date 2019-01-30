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
#include <iomanip>

using namespace std;
pthread_mutex_t lock;

int cacheSize = 4;

int numThreads = 4;
int* sta = new int[numThreads];
int* sto = new int[numThreads];
int* work_done = new int[numThreads];

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
    int index;
    for(int i = 0; i < num; i++){
        Point* p = new Point;
        index = rand()%points.size();
        p->x = points.at(index)->x;
        p->y = points.at(index)->y;
        p->z = points.at(index)->z;
        // cout << p->x << " " <<  p->y << " " <<  p->z << endl;
        means.push_back(p);
    }
}

float distance(Point* a, Point* b){
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2) + pow(a->z - b->z, 2)); 
}


void* find_clusters(void *tid){
    int *id = (int*) tid;
    while(work_done[*id] < 2){
        float min_dist = INT_MAX;
        int cluster_num = 0;
        float dist;
        for(int i = sta[*id]; i < sto[*id]; i++){
            min_dist = INT_MAX;
            for(int j = 0; j < means.size(); j++){
                dist = distance(points[i], means[j]);
                if(min_dist > dist){
                    min_dist = dist;
                    cluster_num = j;
                }
            }
            
            points[i]->clusterID = cluster_num;
        }
        work_done[*id] = 1;
        while(work_done[*id] == 1){}
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
        cout << "Means " << i << " : (" << means.at(i)->x << ", " << means.at(i)->y << ", " << means.at(i)->z << ")\n";
    }
    cout << endl;
}

void print_points(){
    for(int i = 0; i < points.size(); i++){
        cout << "Point " << i << " : (" << points.at(i)->x << ", " << points.at(i)->y << ", " << points.at(i)->z << ") -> " << points[i]->clusterID << endl;
    }
    cout << endl;
}

void performance(){
    double perf = 0;
    vector<int> indices;
    #pragma omp parallel for firstprivate(indices) shared(perf)
    for(int i = 0; i < means.size(); i++){
        indices.clear();
        // Get all points of cluster i
        for(int j = 0; j < points.size(); j++){
            if(points.at(j)->clusterID == i)
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

int main(int argc, char** argv){
    int k = atoi(argv[1]);
    int n = atoi(argv[2]);
    stringstream ss;
    ss << "./data/" << k << "-" << n << ".dat"; 
    readData(ss.str());
    init_means(k);
    // print_means();
    stringstream ss1;
    ss1 << ".\\results\\" << k << ".txt";
    ofstream output;
    output.open(ss1.str(), ios::app);

    numThreads = atoi(argv[3]);
    sta = new int[numThreads];
    sto = new int[numThreads];
    work_done = new int[numThreads];
    double start;
    start = omp_get_wtime();

    pthread_t kcluster_thr[numThreads];
    int* tid = new int[numThreads];
    for(int i = 0; i < numThreads; i++){
        tid[i] = i;
        sta[i] = (float(i) / numThreads) * points.size();
        sto[i] = (i == (numThreads - 1)) ? points.size() : (float(i + 1) / numThreads) * points.size(); 
        work_done[i] = 0;
        pthread_create(&kcluster_thr[i], NULL, find_clusters, &tid[i]);
    }

    bool all_done = false;
    int num_false = 0;

    for(int i = 0; i < 100; i++){
        // cout << "Iteration "  << i << "\n";
        // print_points();

        // Wait till all thread finish fniding clusters
        num_false = 1000;
        while(num_false > 1){
            num_false = 0;
            for(int j = 0; j < numThreads; j++){
                if(work_done[j] != 1){
                    num_false++;
                }
            }
        }

        // Update means
        for(int j = 0; j < means.size(); j++){
            update_cluster(j);
        }
        // print_means();

        if(i == 50){
            break;
        }
        // Tell threads to work again
        for(int j = 0; j < numThreads; j++){
            work_done[j] = 0;
        }
    }

    // Tell all threads to stop
    for(int j = 0; j < numThreads; j++){
        work_done[j] = 2;
        pthread_cancel(kcluster_thr[j]);
    }

    output << std::fixed << std::setprecision(8) << omp_get_wtime() - start;
    output << endl;

    cout << std::fixed << std::setprecision(8) << omp_get_wtime() - start << endl;

    // print_means();
    // print_points();
    // performance();
    output.close();
    return 0;
}