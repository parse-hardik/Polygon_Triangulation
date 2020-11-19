#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>

#include "dcel.hpp"

using namespace std;

int findIter(set<pair<int,int>>::iterator &it, vector<set<pair<int,int>>::iterator> &helper){
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

	set<pair<int,int>> Status;
	vector<set<pair<int,int>>::iterator> helper;
	auto iter = Status.begin();
	auto i = Status.begin();
	unordered_map<int, int> helperIndex;

	while(!pq.empty()){
		pair<Point,int> p=pq.top();
		pq.pop();
		int type = VertexType(vertices, p.second);
		bool err=false;
		int ex = p.first.x, ey = p.first.y;
		cout << ex << " ";
		cout << ey << endl;
		cout << "Type is  " << type << endl;
		cout << "Status before is " ;
		for(auto x:Status)
			cout << x.first << " " << x.second << endl;
		switch(type){
			//Insert ei in T and set helper(ei) to vi.
			case 1:{ 					// Start Vertex
				iter = Status.insert(make_pair(p.second, (p.second+1)%n)).first; 
				helper.push_back(iter);
				helperIndex[helper.size()-1]=p.second;
				break;
				}
			case 2:	{					// End
				pair<int,int> temp = make_pair((p.second+n-1)%n, p.second);
				i = Status.find(temp);
				int index = helperIndex[findIter(i,helper)];
				if(VertexType(vertices, index)==-2)
					diagonals.push_back(make_pair(index,p.second));
				Status.erase(temp);
				break;
				}
			case -1:	{				//Split
				pair<int,int> temp = make_pair(p.second,p.second);
				i = Status.lower_bound(temp);
				cout << "In Split " << endl;
				if(i!=Status.begin())
					i--;
				else{
					cout << "Error" << endl;
					err=true;
					break;
				}
				int index = helperIndex[findIter(i,helper)];
				diagonals.push_back(make_pair(index,p.second));
				helper.push_back(i);
				helperIndex[helper.size()-1]=p.second;
				iter = Status.insert(make_pair(p.second, (p.second+1)%n)).first;
				helper.push_back(iter);
				helperIndex[helper.size()-1]=p.second;
				break;
				}
			case -2:{					//Merge
				pair<int,int> temp = make_pair((p.second+n-1)%n, p.second);
				i = Status.find(temp);
				int index = helperIndex[findIter(i,helper)];
				if(VertexType(vertices,index)==-2)
					diagonals.push_back(make_pair(index,p.second));
				Status.erase(i);
				//Find the edge directly to the left of merge vertex and insert diagonal if helper is also merge
				temp = make_pair(p.second,p.second);
				i = Status.lower_bound(temp);
				i--;
				index = helperIndex[findIter(i,helper)];
				if(VertexType(vertices,index)==-2)
					diagonals.push_back(make_pair(index,p.second));
				helper.push_back(iter);
				helperIndex[helper.size()-1]=p.second;
				break;
				}
			case 0:{
				if(vertices[p.second].y<vertices[(p.second+n-1)%n].y){
					pair<int,int> temp = make_pair((p.second+n-1)%n, p.second);
					i = Status.find(temp);
					int index = helperIndex[findIter(i,helper)];
					if(VertexType(vertices,index)==-2)
						diagonals.push_back(make_pair(index,p.second));
					Status.erase(i);
					//Insert the next edge in Status Data Structure and set helper of ei to vi
					temp = make_pair(p.second,(p.second+1)%n);
					iter = Status.insert(temp).first;
					helper.push_back(iter);
					helperIndex[helper.size()-1]=p.second;
				}
				else{
					pair<int,int> temp = make_pair(p.second, p.second);
					i = Status.lower_bound(temp);
					i--;
					int index = helperIndex[findIter(i,helper)];
					if(VertexType(vertices,index)==-2)
						diagonals.push_back(make_pair(index,p.second));
					helper.push_back(iter);
					helperIndex[helper.size()-1]=p.second;
				}
				break;
				}
		}
		cout << "Status after is " ;
		for(auto x:Status)
			cout << x.first << " " << x.second << endl;
		if(err){
			cout << "In error" << endl;
			break;
		}
	}
	cout << "size is " << diagonals.size() << endl;
	for(int i=0;i<diagonals.size();i++){
		cout << diagonals[i].first << " " << diagonals[i].second << endl;
	}
	DCEL(n, n+diagonals.size(), vertices, diagonals, argc, argv);
}

/*
15 30
10 20
10 10
15 15
20 13
20 25
*/