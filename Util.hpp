#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Point{
	int x,y;
};

struct ComparePoint { 
    bool operator()(pair<Point,int> const& p1, pair<Point,int> const& p2) { 
        if(p1.first.y > p2.first.y)
            return true;
        return p1.first.x < p2.first.x; 
    } 
}; 

struct Edge{
    Point p1,p2;
    int index;
};

bool IsConvex(Point& p1, Point& p2, Point& p3) {
	int tmp;
	tmp = (p3.y-p1.y)*(p2.x-p1.x)-(p3.x-p1.x)*(p2.y-p1.y);
	if(tmp>0) return 1;
	return 0;
}

int VertexType(vector<Point> &vertices, int i){
    int n=vertices.size();
    if(vertices[(i-1)%n].y < vertices[i].y && vertices[(i+1)%n].y < vertices[i].y){
        if(IsConvex(vertices[(i+1)%n], vertices[(i-1)%n], vertices[i]))
            return 1;   //Start Vertex
        return -1;      //Split Vertex
    }
    else if(vertices[(i-1)%n].y > vertices[i].y && vertices[(i+1)%n].y > vertices[i].y){
        if(IsConvex(vertices[(i+1)%n], vertices[(i-1)%n], vertices[i]))
            return 2;   //End Vertex
        return -2;      //Merge Vertex
    }
    return 0;   //Regular Vertex
}