#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {
mapImg = tm;
start = s;
vector<vector<bool>> v(mapImg.width(), vector<bool>(mapImg.height(), false));
short_path = vector<vector<int>>(mapImg.width(), vector<int>(mapImg.height(), -1));
first_found = vector<vector<pair<int,int>>>(mapImg.width(), vector<pair<int,int>>(mapImg.height(), make_pair(-1,-1)));
Queue<pair<int,int>> q;
v[start.first][start.second] = true;
short_path[start.first][start.second] = 0;
q.enqueue(start);
while (!q.isEmpty()) {
    pair<int,int> curr = q.dequeue();
    vector<pair<int,int>> arr = neighbors(curr);
    for (int i = 0; i < arr.size(); i++) {
        pair<int,int> p = arr[i];
        if (good(v, short_path, curr, p)) {
            v[p.first][p.second] = true;
            short_path[p.first][p.second] = short_path[curr.first][curr.second] + 1;
            first_found[p.first][p.second] = curr;
            q.enqueue(p);
        }
    }
}
int long_path = -1;
pair<int,int> treasure_loc;
treasure_loc.first = -1;
treasure_loc.second = -1;
for (int i = 0; i < mapImg.width(); i++) {
    for (int j = 0; j < mapImg.height(); j++) {
        if (short_path[i][j] >= long_path) {
            long_path = short_path[i][j];
            treasure_loc.first = i;
            treasure_loc.second = j;
        }
    }
}    
pathPts = vector<pair<int,int>>(long_path + 1, make_pair(0,0));
pathPts[long_path] = treasure_loc;
for (int i = long_path - 1; i >= 0; i--) {
    pair<int,int> curr = pathPts[i+1];
    pathPts[i] = first_found[curr.first][curr.second];
}
}

PNG decoder::renderSolution(){

PNG map_copy = mapImg;
for (int i = 0; i < pathLength(); i++) {
    int x = pathPts[i].first;
    int y = pathPts[i].second;
    RGBAPixel *pixel = map_copy.getPixel(x, y);
    pixel->r = 255;
    pixel->g = 0;
    pixel->b = 0;
}
return map_copy;
}

PNG decoder::renderMaze(){

PNG map_copy = mapImg;
for (int i = 0; i < map_copy.width(); i++) {
    for (int j = 0; j < map_copy.height(); j++) {
        if (first_found[i][j].first >= 0) {
            setGrey(map_copy, make_pair(i, j));
        }
    }
}  
for (int i = start.first - 3; i <= start.first + 3; i++) {
    for (int j = start.second - 3; j <= start.second + 3; j++) {
        if (i >= 0 && i < map_copy.width() && j >= 0 && j <= map_copy.height()) {
            RGBAPixel *pixel = map_copy.getPixel(i, j);
                pixel->r = 255;
                pixel->g = 0;
                pixel->b = 0;
        }
    }
}
return map_copy;
}

void decoder::setGrey(PNG & im, pair<int,int> loc){

RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
pixel->r = 2 * (pixel->r / 4);
pixel->g = 2 * (pixel->g / 4);
pixel->b = 2 * (pixel->b / 4);

}

pair<int,int> decoder::findSpot(){

return pathPts[pathPts.size() - 1];

}

int decoder::pathLength(){

    return pathPts.size();

}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){

if (!(next.first >= 0 && next.first < mapImg.width() && next.second < mapImg.height() && next.second >= 0)) {
    return false;
} else if (v[next.first][next.second]) {
    return false;
} else if (!compare(*(mapImg.getPixel(next.first, next.second)), d[curr.first][curr.second])) {
    return false;
}
return true;

}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {

   vector<pair<int,int>> arr; 
   pair<int,int> l;
   pair<int,int> b;
   pair<int,int> r;
   pair<int,int> a;
   l.first = curr.first - 1;
   l.second = curr.second;
   b.first = curr.first;
   b.second = curr.second + 1;
   r.first = curr.first + 1;
   r.second = curr.second;
   a.first = curr.first;
   a.second = curr.second - 1;
   arr.push_back(l);
   arr.push_back(b);
   arr.push_back(r);
   arr.push_back(a);
   return arr;

}


bool decoder::compare(RGBAPixel p, int d){

int q = (p.r % 4) * 16 + (p.g % 4) * 4 + (p.b % 4);
if (q == (d+1) % 64)
    return true;
return false;

}
