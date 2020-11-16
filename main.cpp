#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>

#include "Util.hpp"

using namespace std;

int main(int argc, char** argv) {
	int n;
	cout << "Enter the number of vertices of the polygon" << endl;
	cin >> n;
	vector<Point> vertices(n);
	cout << "Enter all vertices in counter-clockwise manner" << endl;
	for(int i=0;i<n;i++)
		cin >> vertices[i].x >> vertices[i].y;
	priority_queue<pair<Point,int>, vector<pair<Point,int>>, ComparePoint> pq;
	unordered_map<int, vector<int>>m;
	for(int i=0;i<vertices.size();i++){
		pq.push(make_pair(vertices[i],i));
		m[i].push_back(i-1);
		m[i].push_back(i+1);
	}

	Edge newEdge;
	vector<int> helpers(n);
	set<Edge> edgeTree;
	set<Edge>::iterator *edgeTreeIterators;
	edgeTreeIterators = new set<Edge>::iterator[n];
	pair<set<Edge>::iterator,bool> edgeTreeRet;

	for(int i=0;i<n;i++) 
		edgeTreeIterators[i] = edgeTree.end();


	while(!pq.empty()){
		pair<Point,int> p=pq.top();
		pq.pop();
		int type = VertexType(vertices, p.second);
		switch(type){
			case 1: 
				newEdge.p1 = p.first;
				newEdge.p2 = vertices[(p.second-1)%n];
				newEdge.index = p.second;
				edgeTreeRet = edgeTree.insert(newEdge);
				edgeTreeIterators[p.second] = edgeTreeRet.first;
				helpers[newEdge.index]=p.second;
				break;
			case 2:
				// if(VertexType(helpers[]))
		}
	}
}
