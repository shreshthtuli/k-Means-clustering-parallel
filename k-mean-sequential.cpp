#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<climits>
#include<algorithm>
#include<cmath>

using namespace std;

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
    ifstream file;
    string line;
    file.open(filename);
    int x, y, z;
    while(file >> x >> y >> z){
        Point p;
        p.x = x;
        p.y = y;
        p.z = z;
        max_val = max({max_val, x, y, z});
        min_val = min({min_val, x, y, z});
        points.push_back(&p);
    }
}

void init_means(int num){
    for(int i = 0; i < num; i++){
        Point p;
        p.x = min_val + rand()%(max_val - min_val);
        p.y = min_val + rand()%(max_val - min_val);
        p.z = min_val + rand()%(max_val - min_val);
        means.push_back(&p);
    }
}

float distance(Point a, Point b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)); 
}


void find_cluster(Point p){
    float min_dist = INT_MAX;
    int cluster_num = 0;
    double dist;
    for(int i = 0; i < means.size(); i++){
        dist = distance(p, *means[i]);
        if(min_dist > dist){
            min_dist = dist;
            cluster_num = i;
        }
    }
    p.clusterID = cluster_num;
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
        cout << "Point " << i << " : (" << points[i]->x << ", " << points[i]->y << ", " << points[i]->z << ") -> " << points[i]->clusterID << endl;
    }
    cout << endl;
}

int main(int argc, char** argv){
    readData("points.dat");
    print_points();
    exit(0);
    init_means(5);

    for(int i = 0; i < 100; i++){
        cout << "Iteration "  << i << "\n";
        for(int j = 0; j < points.size(); j++){
            find_cluster(*points[j]);
        }
        for(int j = 0; j < means.size(); j++){
            update_cluster(j);
        }
        print_points();
    }
    return 0;
}