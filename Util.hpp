#include<bits/stdc++.h>
#include <GL/glut.h> 

using namespace std;

class Point
{
public:
    /* data */
    int key;
    float x , y;
};

class Edge{
	public:
		Point p1,p2;
		int index;
		bool operator<(const Edge & other) const;
};

class half_edge
{
public:
    int origin_v , end_v; 
    class Point * origin , * end;
    class half_edge * twin;
	//class half_edge *
};

class face
{
public:
    int key = -1;
};

class vertex_table
{
public:
    class Point *v;
    class half_edge *e;
};

class face_table
{
public:
    class face *face = NULL;
    vector<half_edge *> inner_components;
    class half_edge *outer_component = NULL;
    float area =-1;
};

class half_edge_table
{
public:
    class half_edge *half_edge , *next , *prev;
    class face *incident_face = NULL;
};

#define INF 10000 

void init2D(float r, float g, float b)
{
	glClearColor(r,g,b,0.0);  
	glMatrixMode (GL_PROJECTION);
	gluOrtho2D (0.0, 200.0, 0.0, 200.0);
}

vector<half_edge_table> het;
vector<half_edge> he;
vector<Point> vertices;
vector<vertex_table> vt;
vector<face_table> ft;
vector<pair<int,int> > diagonals;

int partitions;

void setArguments(vector<half_edge_table> &het1 , vector<half_edge> &h, 
vector<Point> &vert, vector<vertex_table> &vertab, vector<face_table> &ftab){
	het = het1;
	he = h;
	vertices = vert;
	vt = vertab;
	ft=ftab;
}

void display(void){  
	cout<<"in displplay\n";
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(3);
	int n = vertices.size();
		
		int count=0;
		for(auto diagonal: diagonals)
		{
			if(count<partitions)
				glColor3f(0, 1, 0); 
			else
				glColor3f(0, 0, 1); 
			int x0=vertices[diagonal.first].x*2;
			int y0=vertices[diagonal.first].y*2;
			int x1=vertices[diagonal.second].x*2;
			int y1=vertices[diagonal.second].y*2;
			glBegin(GL_LINES);
				glVertex2i(x0,y0);
				glVertex2i(x1,y1);
			glEnd();
			count++;
		}

		for(int i=0;i<vertices.size();i++){
			glColor3f(1, 0, 0); 
			int x0=vertices[i].x*2;
			int y0=vertices[i].y*2;
			int x1=vertices[(i+1)%n].x*2;
			int y1=vertices[(i+1)%n].y*2;
			glBegin(GL_LINES);
				glVertex2i(x0,y0);
				glVertex2i(x1,y1);
			glEnd();
		}

	glFlush();	
}
