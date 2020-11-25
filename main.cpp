#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <ctime>

#include "Monotone_and_Triangulate.hpp"

using namespace std;

int main(int argc, char** argv) {
	int n;
	cout << "Enter the number of vertices of the polygon" << endl;
	cin >> n;
	vector<Point> vertices(n);
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
	
	//DCEL(n, n, vertices, diagonals, argc, argv);
	
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

	auto current_time = std::chrono::system_clock::now();

	while(!pq.empty()){
		pair<Point,int> p=pq.top();
		pq.pop();
		int type = VertexType(vertices, p.second);
		bool err=false;
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
				auto i = Status.lower_bound(newEdge); //If helper(ej) = merge vertex, then add diagonal
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
		if(err){
			cout << "In error" << endl;
			break;
		}
	}
	auto end_time = std::chrono::system_clock::now();
	auto milli_secs = std::chrono::duration<double, std::milli>(end_time - current_time).count() * .001;
	cout << "Time taken for making the polygon y-monotone is " << milli_secs << " milli seconds." << endl;
	partitions = diagonals.size();

	current_time = std::chrono::system_clock::now();
	//Inserts all the diagonals that are to be inserted into the data structure D
	DCEL(n, n, vertices, diagonals);
	end_time = std::chrono::system_clock::now();
	milli_secs = std::chrono::duration<double, std::milli>(end_time - current_time).count() * .001;
	cout << "Time taken for intialising DCEL\n and inserting diagonals is " << milli_secs << " milli seconds." << endl;

	current_time = std::chrono::system_clock::now();
	//For all y-monotone polygons, triangulate each one of them
	for(auto polygon: POLYGONS)
	{
		cout<<"triangulating a polygon\n";
		triangulatePolygon(polygon);
	}
	end_time = std::chrono::system_clock::now();
	milli_secs = std::chrono::duration<double, std::milli>(end_time - current_time).count() * .001;
	cout << "Time taken for triangulating each y-monotone polygon is " << milli_secs << " milli seconds." << endl;

	//Plot the polygon and all the triangulated regions
	glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (900, 900);
    glutInitWindowPosition (100, 0);
    glutCreateWindow ("Initial Polygon");
    init2D(0.0,0.0,0.0);
    glutDisplayFunc(display);
    glutMainLoop();
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

/*
10
3 5
2 4
2 3
1 2
2 1
3 0
4 1
5 2
4 3
4 4

*/


/*
5
5 5
4 4
3 3
5 1
6 2


*/

/*
11
4 6
3 5
3 4
2 3
3 2
2 1
3 0
3 -1
4 0
5 1
6 2
*/

/*
10
6 9
5 8
6 7
5 6
4 5
3 4
1 3
5 2
6 1
8 3


*/

//I shared a Jam file with the meeting: https://jamboard.google.com/d/1O7rzjazuwVcAMXXrsSKwbA6BqUz1kdSg_W-_uwnSGk0/edit?usp=meet_whiteboard

/*
15

21 79
23 47
21 34
15 28
20 20
25 13
28 24
31 19
36 34
42 26
44 76
36 61
33 47
27 73
30 44
*/

/*
28

14 76
13 67
16 58
18 54
18 50
16 45
13 44
10 50
7 68
2 69
3 57
7 47
10 43
13 37
9 34
9 29
14 25
16 16
17 25
17 29
19 30
23 35
25 40
25 46
24 53
23 58
20 61
17 67
*/
