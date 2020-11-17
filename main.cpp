#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>

#include "dcel.hpp"

using namespace std;

int main(int argc, char** argv) {
	int n;
	cout << "Enter the number of vertices of the polygon" << endl;
	cin >> n;
	vector<Point> vertices(n);
	vector<pair<int,int>> diagonals;
	cout << "Enter all vertices in counter-clockwise manner" << endl;
	for(int i=0;i<n;i++){
		cin >> vertices[i].x >> vertices[i].y;
		vertices[i].key=i;
	}
	/* 	Initializes the DCEL data structure with a simple closed Polygon
		It has 3 tables, namely, 
		Vertex Table
		Half-Edge Table
		Face Table
	*/
	DCEL(n, n, vertices, diagonals, argc, argv);
	
	priority_queue<pair<Point,int>, vector<pair<Point,int>>, ComparePoint> pq;
	for(int i=0;i<vertices.size();i++){
		pq.push(make_pair(vertices[i],i));
	}

	set<pair<int,int>> Status;
	set<pair<int,int>>::iterator iter;
	unordered_map<set<pair<int,int>>::iterator, int> helper;

	while(!pq.empty()){
		pair<Point,int> p=pq.top();
		pq.pop();
		int type = VertexType(vertices, p.second);
		switch(type){
			//Insert ei in T and set helper(ei) to vi.
			case 1: 					// Start Vertex
				iter = Status.insert(make_pair(p.second, (p.second+1)%n)); 
				helper[iter]=p.second;
				break;
			case 2:
				pair<int,int> temp = make_pair(p.second, (p.second-1)%n);
				set<pair<int,int>>::iterator i = Status.find(temp);
				if(VertexType(helper[i])==-2)
					diagonals.push_back(make_pair(helper[i],p.second));
				Status.erase(temp);
				break;
			case -1:
				pair<int,int> temp = make_pair(p.second,p.second);
				set<pair<int,int>>::iterator i = Status.lower_bound(temp);
				i--;
				diagonals.push_back(make_pair(helper[i],p.second));
				helper[i]=p.second;
				iter = Status.insert(make_pair((p.second-1)%n, p.second));
				helper[iter]=p.second;
		}
	}
}
