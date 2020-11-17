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

class half_edge
{
public:
    int origin_v , end_v; 
    class Point * origin , * end;
    class half_edge * twin;
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

void setArguments(vector<half_edge_table> &het1 , vector<half_edge> &h, vector<Point> &vert, vector<vertex_table> &vertab){
	het = het1;
	he = h;
	vertices = vert;
	vt = vertab;
}

void display(void){  
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(3);
		int l = het.size();
		class half_edge_table temp;
		for(int i=0;i<l;i++){
			temp=het[i];
			glColor3f(1, 0, 0); 
			int x0=vertices[temp.half_edge->origin_v].x*5;
			int y0=vertices[temp.half_edge->origin_v].y*5;
			int x1=vertices[temp.half_edge->end_v].x*5;
			int y1=vertices[temp.half_edge->end_v].y*5;
			glBegin(GL_LINES);
				glVertex2i(x0,y0);
				glVertex2i(x1,y1);
			glEnd();
		}

	glFlush();	
}
