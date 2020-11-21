#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>

#include "dcel.hpp"

using namespace std;

int findIter(set<Edge>::iterator &it, vector<set<Edge>::iterator> &helper){
	for(int i=0;i<helper.size();i++){
		if(helper[i]==it)
			return i;
	}
	return -1;
}

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
	// DCEL(n, n, vertices, diagonals, argc, argv);
	
	cout << "Proceed to decomposing the polygon into y-MonoTone pieces?" << endl;
	cout << "Y for yes, else N" << endl;
	char ch;
	cin >> ch;
	if(ch=='N')
		return 0;

	priority_queue<pair<Point,int>, vector<pair<Point,int>>, ComparePoint> pq;
	for(int i=0;i<vertices.size();i++){
		pq.push(make_pair(vertices[i],i));
	}

	set<Edge> Status;
	// vector<set<Edge>::iterator> helper;
	map<Edge, int, EdgeCompare> helper;

	while(!pq.empty()){
		pair<Point,int> p=pq.top();
		pq.pop();
		int type = VertexType(vertices, p.second);
		bool err=false;
		int ex = p.first.x, ey = p.first.y;
		cout << ex << " ";
		cout << ey << endl;
		cout << "Type is  " << type << endl;
		cout << "Status before is " << endl;
		for(auto it=Status.begin();it!=Status.end();it++){
			Edge newEdge;
			newEdge.p1 = it->p1;
			newEdge.p2 = it->p2;
			int index = helper[newEdge];
			cout << "(" << it->p1.x << "," << it->p1.y  << ") " << "(" << it->p2.x << "," << it->p2.y << ") -> " << index<< endl;
		}
		switch(type){
			//Insert ei in T and set helper(ei) to vi.
			case 1:{ 					// Start Vertex
				Edge newEdge;
				newEdge.p1 = p.first;
				newEdge.p2 = vertices[(p.second+1)%n];
				auto iter = Status.insert(newEdge).first; 
				helper[newEdge]=p.second;
				break;
				}
			case 2:	{					// End
				Edge newEdge;
				newEdge.p1 =vertices[(p.second+n-1)%n];
				newEdge.p2 = p.first;
				int index = helper[newEdge];
				if(VertexType(vertices, index)==-2)
					diagonals.push_back(make_pair(index,p.second));
				Status.erase(newEdge);
				break;
				}
			case -1:	{				//Split
				Edge newEdge;
				newEdge.p1 =p.first;
				newEdge.p2 = p.first;
				auto i = Status.lower_bound(newEdge);
				if(i!=Status.begin())
					i--;
				else{
					cout << "Error" << endl;
					err=true;
					break;
				}
//				cout << "In Split \n" << i->first.p1.x << "," << i->first.p1.y << " to ";
//				cout << i->first.p2.x << "," << i->first.p2.y << endl;
				newEdge.p1 = i->p1;
				newEdge.p2 = i->p2;
				int index = helper[newEdge];
				// cout << "Diagonal is " << index << "to " << p.second << endl;
				diagonals.push_back(make_pair(index,p.second));
				helper[newEdge] = p.second;
				newEdge.p1 = p.first;
				newEdge.p2 = vertices[(p.second+1)%n];
				auto iter = Status.insert(newEdge).first;
				helper[newEdge] = p.second;
				break;
				}
			case -2:{					//Merge
				Edge newEdge;
				newEdge.p1 = vertices[(p.second+n-1)%n];
				newEdge.p2 = p.first;
				auto iter = Status.find(newEdge);
				int index = helper[newEdge];
				if(VertexType(vertices,index)==-2)
					diagonals.push_back(make_pair(index,p.second));
				Status.erase(iter);
				//Find the edge directly to the left of merge vertex and insert diagonal if helper is also merge
				newEdge.p1 = p.first;
				newEdge.p2 = p.first;
				auto i = Status.lower_bound(newEdge);
				if(i!=Status.begin())
					i--;
				else{
					cout << "Error" << endl;
					err=true;
					break;
				}
				newEdge.p1 = i->p1;
				newEdge.p2 = i->p2;
				index = helper[newEdge];
				if(VertexType(vertices,index)==-2)
					diagonals.push_back(make_pair(index,p.second));
				helper[newEdge] = p.second;
				break;
				}
			case 0:{
				if(vertices[p.second].y<vertices[(p.second+n-1)%n].y){
					Edge newEdge;
					newEdge.p1 = vertices[(p.second+n-1)%n];
					newEdge.p2 = p.first;
					auto i = Status.find(newEdge);
					int index = helper[newEdge];
					if(VertexType(vertices,index)==-2)
						diagonals.push_back(make_pair(index,p.second));
					Status.erase(i);
					//Insert the next edge in Status Data Structure and set helper of ei to vi
					newEdge.p2 = vertices[(p.second+1)%n];
					newEdge.p1 = p.first;
					Status.insert(newEdge).first;
					helper[newEdge] = p.second;
				}
				else{
					Edge newEdge;
					newEdge.p1 = p.first;
					newEdge.p2 = p.first;
					auto i = Status.lower_bound(newEdge);
					if(i!=Status.begin())
						i--;
					else{
						cout << "Error" << endl;
						err=true;
						break;
					}
					newEdge.p1 = i->p1;
					newEdge.p2 = i->p2;
					int index = helper[newEdge];
					if(VertexType(vertices,index)==-2)
						diagonals.push_back(make_pair(index,p.second));
					helper[newEdge] = p.second;
				}
				break;
				}
		}
		cout << "Status after is " << endl;
		for(auto it=Status.begin();it!=Status.end();it++){
			Edge newEdge;
			newEdge.p1 = it->p1;
			newEdge.p2 = it->p2;
			int index = helper[newEdge];
			cout << "(" << it->p1.x << "," << it->p1.y  << ") " << "(" << it->p2.x << "," << it->p2.y << ") -> " << index<< endl;
		}
		if(err){
			cout << "In error" << endl;
			break;
		}
	}
	cout << "size is " << diagonals.size() << endl;
	for(int i=0;i<diagonals.size();i++){
		cout << diagonals[i].first << " " << diagonals[i].second << endl;
	}
	// DCEL(n, n+diagonals.size(), vertices, diagonals, argc, argv);
}

/*
15 30
10 20
10 10
15 15
20 13
20 25
*/


/*
15 20
10 25
10 10
20 15
35 13
30 30
*/