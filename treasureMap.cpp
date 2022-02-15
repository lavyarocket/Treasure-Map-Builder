#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{

base = baseim;
maze = mazeim;
start = s;

}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

RGBAPixel *pixel = im.getPixel(loc.first,loc.second);
pixel->r=2*((pixel->r)/4);
pixel->g=2*((pixel->g)/4);
pixel->b=2*((pixel->b)/4);


}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

RGBAPixel *pixel = im.getPixel(loc.first,loc.second);
d = d%64;
int r = d >> 4;
r = r & 0b00000011;
pixel->r = pixel->r & 0b11111100;
pixel->r = pixel->r + r;
int g = d >> 2;
g = g & 0b00000011;
pixel->g = pixel->g & 0b11111100;
pixel->g = pixel->g + g;
int b = d;
b = b & 0b00000011;
pixel->b = pixel->b & 0b11111100;
pixel->b = pixel->b + b;

}

PNG treasureMap::renderMap(){

PNG base_copy = base;
vector<vector<bool>> v( base_copy.height() , vector<bool> (base_copy.width(), 0)); 
vector<vector<int>> d( base_copy.height() , vector<int> (base_copy.width(), 0));  
Queue<pair<int,int>> q;
v[start.second][start.first] = true;
d[start.second][start.first] = 0;
setLOB(base_copy,start,0); 
q.enqueue(start); 
while(!q.isEmpty()) {
    pair<int,int> curr;
    curr = q.dequeue();
    vector<pair<int,int>> arr = neighbors(curr);
    for(int i=0;i<4;i++) {
        if(good(v,curr,arr[i])) {
            v[arr[i].second][arr[i].first] = true;
            d[arr[i].second][arr[i].first] = d[curr.second][curr.first] + 1;
            setLOB(base_copy,arr[i],d[arr[i].second][arr[i].first]);
            q.enqueue(arr[i]);
        }
    }
}
return base_copy;
}


PNG treasureMap::renderMaze(){

PNG base_copy = base;
RGBAPixel *pixel2 = maze.getPixel(start.first,start.second);
vector<vector<bool>> v( base_copy.height() , vector<bool> (base_copy.width(), 0)); 
Queue<pair<int,int>> q;
v[start.second][start.first] = true;
q.enqueue(start); 
while(!q.isEmpty()) {
    pair<int,int> curr;
    curr = q.dequeue();
    vector<pair<int,int>> arr = neighbors(curr);
    for(int i=0;i<4;i++) {
        if(good(v,curr,arr[i])) {
            v[arr[i].second][arr[i].first] = true;
            RGBAPixel *pixel1 = maze.getPixel(arr[i].first,arr[i].second);
            if (*pixel1 == *pixel2) {
                setGrey(base_copy,arr[i]);
            }
            q.enqueue(arr[i]);
        }
    }
}
for(int i = start.second-3; i <= start.second + 3 ; i++){
    for(int j = start.first-3; j<= start.first + 3; j++){
        if(j < base_copy.width() && i < base_copy.height() &&  j >= 0 &&  i >= 0) {
        RGBAPixel *pixel = base_copy.getPixel(j,i);
        pixel->r=255;
        pixel->g=0;
        pixel->b=0;
    }
    }
}
 return base_copy;

}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
if(next.first < base.width() && next.second < base.height() &&  next.first >= 0 &&  next.second >= 0) {
    if(v[next.second][next.first] == false) {
        RGBAPixel *pixel1 = maze.getPixel(curr.first,curr.second);
        RGBAPixel *pixel2 = maze.getPixel(next.first,next.second);
        if(*pixel1==*pixel2) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
} else {
    return false;
}
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
vector<pair<int,int>> arr;
arr.push_back(make_pair(curr.first-1,curr.second));
arr.push_back(make_pair(curr.first,curr.second-1));
arr.push_back(make_pair(curr.first+1,curr.second));
arr.push_back(make_pair(curr.first,curr.second+1));
return arr;
}

