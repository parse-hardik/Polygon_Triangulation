#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>

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
	
	cout << "Proceed to decomposing the polygon into y-MonoTone pieces?" << endl;
	cout << "Y for yes, else N" << endl;
	char ch;
	cin >> ch;
	if(ch=='N')
		return 0;

	Make_Monotone(vertices);

	glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (700, 700);
    glutInitWindowPosition (100, 0);
    glutCreateWindow ("Final Triangulated Polygon");
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

