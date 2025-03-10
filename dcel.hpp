#include <bits/stdc++.h>
#include "Util.hpp"
#include <vector>

using namespace std;
vector<vector<Point>> POLYGONS;

//A utility function to Compare 2 points
class ComparePoint { 
	public:
    bool operator()(pair<Point,int> const& p1, pair<Point,int> const& p2) { 
        return (p1.first.y < p2.first.y) || (p1.first.y==p2.first.y && p1.first.x>p2.first.x);
    } 
}; 

//Checks if the following 3 points make a clockwise turn or not
//Accepts input in the following format
//p1->next Point
//p2->prev Point
//p3->curr Point
bool IsConvex(const Point& p1, const Point& p2, const Point& p3) {
    int tmp = (p3.y-p2.y)*(p2.x-p1.x) - (p2.y-p1.y)*(p3.x-p2.x);
	if(tmp>0) return 1; 	//counter-clockwise
	return 0;               //clockwise
}

//This structure helps in construction of the Status and helper Data Structures.
//Compares the edges based on their y and then x coordinates
struct EdgeCompare
{
    bool operator() (const Edge& e1, const Edge& other) const{
        if(other.p1.y == other.p2.y) {
            if(e1.p1.y == e1.p2.y) {
                if(e1.p1.y < other.p1.y) return true;
                else return false;
            }
            if(IsConvex(e1.p1,e1.p2,other.p1)) return true;
            else return false;
        } else if(e1.p1.y == e1.p2.y) {
            if(IsConvex(other.p1,other.p2,e1.p1)) return false;
            else return true;	
        } else if(e1.p1.y < other.p1.y) {
            if(IsConvex(other.p1,other.p2,e1.p1)) return false;
            else return true;			
        } else {
            if(IsConvex(e1.p1,e1.p2,other.p1)) return true;
            else return false;
        }
    }
};

bool Edge::operator < (const Edge & other) const {
    if(other.p1.y == other.p2.y) {
		if(p1.y == p2.y) {
			if(p1.y < other.p1.y) return true;
			else return false;
		}
		if(IsConvex(p1,p2,other.p1)) return true;
		else return false;
	} else if(p1.y == p2.y) {
		if(IsConvex(other.p1,other.p2,p1)) return false;
		else return true;	
	} else if(p1.y < other.p1.y) {
		if(IsConvex(other.p1,other.p2,p1)) return false;
		else return true;			
	} else {
		if(IsConvex(p1,p2,other.p1)) return true;
		else return false;
	}
}

//A utility function to check the type of the given vertex
int VertexType(vector<Point> &vertices, int i){
    int n=vertices.size();
    if(vertices[(i+n-1)%n].y < vertices[i].y && vertices[(i+1)%n].y < vertices[i].y){
        if(IsConvex(vertices[(i+1)%n], vertices[(i+n-1)%n], vertices[i]))
            return 1;   //Start Vertex
        return -1;      //Split Vertex
    }
    else if(vertices[(i+n-1)%n].y > vertices[i].y && vertices[(i+1)%n].y > vertices[i].y){
        if(IsConvex(vertices[(i+1)%n], vertices[(i+n-1)%n], vertices[i]))
            return 2;   //End Vertex
        return -2;      //Merge Vertex
    }
    return 0;   //Regular Vertex
} 

// Given three colinear points p, q, r, the function checks if 
// point q lies on line segment 'pr' 
bool onSegment(Point p, Point q, Point r) 
{ 
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && 
			q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) 
		return true; 
	return false; 
} 

// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(Point p, Point q, Point r) 
{ 
	float val = (q.y - p.y) * (r.x - q.x) - 
			(q.x - p.x) * (r.y - q.y); 

	if (val == 0) return 0; // colinear 
	return (val > 0)? 1: 2; // clock or counterclock wise 
} 

// The function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
bool doIntersect(Point p1, Point q1, Point p2, Point q2) 
{ 
	// Find the four orientations needed for general and 
	// special cases 
	float o1 = orientation(p1, q1, p2); 
	float o2 = orientation(p1, q1, q2); 
	float o3 = orientation(p2, q2, p1); 
	float o4 = orientation(p2, q2, q1); 

	// General case 
	if (o1 != o2 && o3 != o4) 
		return true; 

	// Special Cases 
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(p1, p2, q1)) return true; 

	// p1, q1 and p2 are colinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(p1, q2, q1)) return true; 

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(p2, p1, q2)) return true; 

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(p2, q1, q2)) return true; 

	return false; // Doesn't fall in any of the above cases 
} 

// Returns true if the point p lies inside the polygon[] with n vertices 
bool isInside(Point polygon[], int n, Point p) 
{ 
	// There must be at least 3 vertices in polygon[] 
	if (n < 3) return false; 

	// Create a point for line segment from p to infinite 
	Point extreme = {INF, p.y}; 

	// Count intersections of the above line with sides of polygon 
	int count = 0, i = 0; 
	do
	{ 
		int next = (i+1)%n; 

		// Check if the line segment from 'p' to 'extreme' intersects 
		// with the line segment from 'polygon[i]' to 'polygon[next]' 
		if (doIntersect(polygon[i], polygon[next], p, extreme)) 
		{ 
			// If the point 'p' is colinear with line segment 'i-next', 
			// then check if it lies on segment. If it lies, return true, 
			// otherwise false 
			if (orientation(polygon[i], p, polygon[next]) == 0) 
			return onSegment(polygon[i], p, polygon[next]); 

			count++; 
		} 
		i = next; 
	} while (i != 0); 

	// Return true if count is odd, false otherwise 
	return count&1; // Same as (count%2 == 1) 
} 

void print_half_edge(vector<half_edge> &a , vector<Point> &v , int l)
{
    for(int i = 0 ; i < l ; i++)
    {
        int s , e;
        half_edge *temp;
        s = a[i].origin_v; e = a[i].end_v;
        temp = a[i].twin;
        cout << "edge :" << s <<"-" << e <<" ";
        cout << "coordinates : (" << v[s].x <<" , " << v[s].y <<") , (" << v[e].x <<" , " << v[e].y <<") ";
        cout << "Twin edge :" << temp->origin_v <<"-" << temp->end_v <<"\n";
    }
}

int search_half_edge(int s , int e , vector<half_edge> &h)
{
    int l = h.size();
    for(int i = 0 ; i < l ; i++)
    {
        if(h[i].origin_v == s && h[i].end_v == e){
            return i;
        }
    }
    cout << "Error in serching half edge" <<"\n";
    return -1;
}

int search_half_edge_table(half_edge *half_edge , vector<half_edge_table> half_edge_table)
{
    int l = half_edge_table.size();
    for(int i = 0; i < l ; i++)
    {
        if(half_edge_table[i].half_edge == half_edge)
            return i;
    }
    return 0;
}

void fill_vertex_table(vector<vertex_table> &ver_tab , int l , vector<vector <float> > adj , vector<half_edge> &h , vector<Point> &v)
{
    for(int i = 0 ; i < l ; i++)
    {
        int s = v[i].key;
        int e = adj[s][0]; // Assuming no stand alone vertex
        ver_tab[i].v = &v[i];
        ver_tab[i].e = &h[search_half_edge(s , e , h)];
    }
}

float angle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    x1 = x1 - x2; y1 = y1-y2;
    x3 = x3 - x2; y3 = y3-y2;
    float dot = x1*x3 + y1*y3 ;    
    float det = x1*y3 - y1*x3 ; 
	float result = atan2(det, dot); 
	return ((result < 0) ? (result * 180 / 3.141592 ) + 360 : (result * 180 / 3.141592 ) );
}


int next_half_edge(int current, vector<vector <float> > adj, vector <Point> vertex , vector<half_edge> &half_edge)
{
    int s , e;
    float max_angle = 0, temp_angle;
    int next_vertex;
    s = half_edge[current].origin_v; e = half_edge[current].end_v;
    float x1 , x2 , y1 , y2;
    next_vertex = s;
    x1 = vertex[s].x; y1 = vertex[s].y;
    x2 = vertex[e].x; y2 = vertex[e].y;
    for(auto k:adj[e])
    {
        if(k == e)
            continue;
        else
        {
            temp_angle = angle(x1 , y1 , x2 , y2 , vertex[k].x , vertex[k].y);
            if(max_angle < temp_angle)
            {
                max_angle = temp_angle;
                next_vertex = k;
            }
        }
    }
    return search_half_edge(e , next_vertex , half_edge);
}

float area_poly(vector<int> key , vector<Point> vertex)
{
    float x2 ,y2;
    float signedArea = 0;
    int l = key.size();
    for(int i = 0; i < l ; i++)
    {
        float x1 = vertex[key[i]].x;
        float y1 = vertex[key[i]].y;
        if(i == l-1){
            x2 = vertex[key[0]].x;
            y2 = vertex[key[0]].y;
            signedArea += (x1 * y2 - x2 * y1);
        }
        else{
            x2 = vertex[key[i+1]].x;
            y2 = vertex[key[i+1]].y;
            signedArea += (x1 * y2 - x2 * y1);
        }
    }
    return abs(signedArea/2);
}

void fill_half_edge_face(vector<half_edge_table> &half_edge_table ,vector<face> &face , vector<Point> vertex , vector<face_table> &face_table)
{
    int l = half_edge_table.size();
    int face_key = 0;
    int head = 0;
    float x2 ,y2;
    float signedArea = 0;
    for(int i = 0; i < l ; i++)
    {
        float x1 = vertex[half_edge_table[i].half_edge->origin_v].x;
        float y1 = vertex[half_edge_table[i].half_edge->origin_v].y;
        if(half_edge_table[i].next == half_edge_table[head].half_edge){
            x2 = vertex[half_edge_table[head].half_edge->origin_v].x;
            y2 = vertex[half_edge_table[head].half_edge->origin_v].y;
            signedArea += (x1 * y2 - x2 * y1);
            if(signedArea > 0) // Assuming no standalone edge
            {
                face_table[face_key].area = abs(signedArea/2);
                face[face_key].key = face_key;
                face_table[face_key].face = &face[face_key];
                face_table[face_key].outer_component = half_edge_table[head].half_edge;
                for(int j = head ; j <= i ;j++)
                    half_edge_table[j].incident_face = &face[face_key];
                face_key++;
            }
            signedArea = 0;
            head = i+1;
        }
        else{
            x2 = vertex[half_edge_table[i].half_edge->end_v].x;
            y2 = vertex[half_edge_table[i].half_edge->end_v].y;
            signedArea += (x1 * y2 - x2 * y1);
        }
    }
    face[face_key].key = -1;
    face_table[face_key].face = &face[face_key];
}

void fill_half_edge_table(vector<half_edge_table> &half_edge_table , vector<half_edge> &half_edge , vector<bool> &unvisited_half_edge, 
vector <Point> vertex , vector<vector <float> > adj , vector<face> &face , vector<face_table> &face_table)
{
    int l = unvisited_half_edge.size();
    int current, next , previous;
    int head , j = 0 , head_index;
    for(int i = 0 ; i < l ; i++)
    {
        if(unvisited_half_edge[i] == 0)
        {
            unvisited_half_edge[i] = 1;
            head = i;
            head_index = j;
            half_edge_table[j].half_edge = &half_edge[i];
            next = next_half_edge(i , adj , vertex , half_edge);
            half_edge_table[j].next = &half_edge[next];
            j++;
            previous = i; current = next;
            while(1)
            {
                unvisited_half_edge[current] = 1;
                half_edge_table[j].half_edge = &half_edge[current];
                next = next_half_edge(current , adj , vertex , half_edge);
                half_edge_table[j].next = &half_edge[next];
                half_edge_table[j].prev = &half_edge[previous];
                j++;
                previous = current; current = next;
                if(next == head){
                    break;
                }
            }
            half_edge_table[head_index].prev = &half_edge[previous];
        }
    }
    fill_half_edge_face(half_edge_table , face , vertex , face_table);
}

bool check_if_point_is_inside(int ver , vector<int> key , vector<Point> &vertex)
{
    float x , y;
    int n = key.size(); 
	Point polygon1[n];
    for(int i = 0 ; i < n ; i++)
    {
        polygon1[i] = {vertex[key[i]].x , vertex[key[i]].y};
    }
	Point p = {vertex[ver].x , vertex[ver].y}; 
	return isInside(polygon1, n, p); 
}

int check_if_inside(vector<face_table> &face_table,vector<Point> &vertex  ,vector<int> key , vector<face> face ,
 vector<half_edge_table> half_edge_table)
{
    int face_index = -1;
    float self_area;
    int l = face_table.size();
    class face_table temp;
    class half_edge *half_edge , *temp2 , *head;
    float min_area = 100021.1;
    self_area = area_poly(key , vertex);
    for(int i = 0 ; i < l ; i++)
    {
        if(face_table[i].face == NULL)
            break;
        temp = face_table[i];
        half_edge = temp.outer_component;
        int index = search_half_edge_table(half_edge , half_edge_table);
        temp2 = half_edge_table[index].half_edge;
        head = temp2;
        vector<int> key2;
        while(1)
        {
            key2.push_back(temp2->origin_v);
            temp2 = half_edge_table[index].next;
            if(head == temp2)
                break;
            index++;
        }
        bool flag = 1;
        for(int k = 0 ; k < key.size() ; k++)
        {
            flag = flag & check_if_point_is_inside(key[k] , key2 , vertex);
            if(flag == 0)
                break;
        }
        if(flag)
        {
            float a = area_poly(key2 , vertex);
            if(min_area > a && self_area != a && self_area < a){
                min_area = a;
                face_index = i;
            }
        }
    }
    return face_index;
}

int search_outer_face(vector<face> face)
{
    for(int i = 0 ; i < face.size() ; i++)
    {
        if(face[i].key == -1)
            return i;
    }
    return 0;
}

void fill_face_table_inner_components(vector<face_table> &face_table , vector<half_edge> &half_edge 
, vector<half_edge_table> &half_edge_table , vector<face> &face , vector <Point> vertex)
{
    int face_index;
    int l = half_edge_table.size();
    class half_edge *temp ,*head;
    for(int i = 0 ; i < l ; i++)
    {
        if(half_edge_table[i].incident_face != NULL)
            continue;
        temp = half_edge_table[i].half_edge;
        head = temp;
        vector<int> key;
        while(1)
        {
            key.push_back(temp->origin_v);
            temp = half_edge_table[i].next;
            if(head == temp){
                face_index = check_if_inside(face_table , vertex , key , face , half_edge_table);
                if(face_index != -1)
                {
                    int index = search_half_edge_table(temp , half_edge_table);
                    face_table[face_index].inner_components.push_back(half_edge_table[index].half_edge);
                    while(1)
                    {
                        half_edge_table[index].incident_face = &face[face_index];
                        temp = half_edge_table[index].next;
                        if(temp == head)
                            break;
                        index++;
                    }
                }
                else
                {
                    face_index = search_outer_face(face);
                    int index = search_half_edge_table(temp , half_edge_table);
                    face_table[face_index].inner_components.push_back(half_edge_table[index].half_edge);
                    while(1)
                    {
                        half_edge_table[index].incident_face = &face[face_index];
                        temp = half_edge_table[index].next;
                        if(temp == head)
                            break;
                        index++;
                    }
                }
                
                break;
            }
            i++;
        }
        key.clear();
    }
}

void print_vertex_table(vector<vertex_table> &ver_tab , int l)
{
    Point * temp_v;
    half_edge * temp_e;
    cout << "\n" << "********** Vertex Table ***********"<< "\n";
    cout << "vertex " << " Coordinates " << "Incident Edge " <<"\n";
    for(int i = 0 ; i < l ; i++)
    {
        temp_v = ver_tab[i].v; 
        temp_e = ver_tab[i].e;
        cout <<temp_v->key <<"\t(" << temp_v->x <<" , " << temp_v->y <<") ";
        cout <<"\t" << temp_e->origin_v <<"-"<<temp_e->end_v <<"\n";
    }
}

void print_half_edge_table(vector<half_edge_table> &half_edge_table , vector<half_edge> &half_edge)
{
    int l = half_edge_table.size();
    class half_edge_table temp;
    cout << "\n" << "********** Half Edge Table **********"<< "\n";
    cout << "Half-edge " <<"Origin " << "Twin Incident_Face Next Previous" <<"\n";
    for(int i = 0;  i < l ; i++)
    {
        temp = half_edge_table[i];
        cout << temp.half_edge->origin_v <<"-" << temp.half_edge->end_v <<"\t   ";
        cout << temp.half_edge->origin_v <<"\t";
        cout << temp.half_edge->twin->origin_v <<"-" << temp.half_edge->twin->end_v <<"\t";
        if(temp.incident_face != NULL)
            cout << "   F"<<temp.incident_face->key<<"\t    ";
        else
            cout << "   NULL" <<"     ";
        cout << temp.next->origin_v <<"-" << temp.next->end_v <<"   ";
        cout << temp.prev->origin_v <<"-" << temp.prev->end_v <<"\n";
    }
}

void print_face_table(vector<face_table> face_table)
{
    int l = face_table.size();
    class face_table temp;
    cout << "*************** Face_Table **************" <<"\n";
    cout << "Face   Outer_component" <<"  Inner_Components" <<"\n";
    for(int i = 0 ; i < l ; i++)
    {
        temp = face_table[i];
        if(temp.face == NULL)
            break;
        if(temp.outer_component == NULL){
            cout <<"Finf" <<setw(3)<<"|"<<setw(8)<<"NULL" <<setw(9) << "|" << setw(5);
            int n = temp.inner_components.size();
            for(int j = 0 ; j < n ; j++)
                cout <<temp.inner_components[j]->origin_v <<"-"<<temp.inner_components[j]->end_v <<"  ";
            if(n == 0)
                cout <<setw(5)<< "NULL\t";    
            cout << "\n";
        }
        else{
            cout << "F" << temp.face->key <<setw(5)<<"|"<<setw(5) << temp.outer_component->origin_v <<"-"<<temp.outer_component->end_v;
            cout<<setw(10) <<"|" << setw(5);
            int n = temp.inner_components.size();
            for(int j = 0 ; j < n ; j++)
                cout << temp.inner_components[j]->origin_v <<"-"<<temp.inner_components[j]->end_v <<"\t";
            if(n == 0)
                cout << "    NULL";
            cout << "\n"; 
        }
    }
}

int search_face_table(int key, vector<face_table>& face_table)
{
    for (int i =0; i< face_table.size(); i++)
    {
        if(face_table[i].face->key == key)
        return i;
    }

    cout<<"Error in search_face_table"<<endl;
    return -1;
}

void print_neighbouring_faces(float x, float y, vector<half_edge> & half_edge_vector, vector<half_edge_table> &half_edge_table, vector<face_table> &face_table)
{   
    class half_edge *half_edge, *head, *temp;
    map <int,bool> m;
    int index = search_half_edge(x,y,half_edge_vector);
    half_edge =  &half_edge_vector[index];
    index = search_half_edge_table(half_edge, half_edge_table);
    int stored_index = index;
    int original_face = half_edge_table[stored_index].incident_face->key;
    if(original_face == -1) // For handling Outer boundary
    {
        int face_table_index = search_face_table(original_face, face_table);
        vector<class half_edge *> outer_component = face_table[face_table_index].inner_components;
        int index_inner;
        for(int k = 0 ; k < outer_component.size() ; k++){
            int outer_component_index = search_half_edge_table(outer_component[k], half_edge_table);
            index = outer_component_index;
            head = half_edge_table[outer_component_index].half_edge;
            temp = head;
            while(1)
            {
                temp = half_edge_table[outer_component_index].next;
                index_inner = search_half_edge_table(temp->twin , half_edge_table);
                m[half_edge_table[index_inner].incident_face->key] = 1;
                outer_component_index++;
                if (temp == head) 
                    break; 
            }
        }
        for (auto j: m)
        {
            cout<<"F"<<j.first<<endl;
        }
        return;
    }
    int face_table_index = search_face_table(original_face, face_table);
    class half_edge *outer_component = face_table[face_table_index].outer_component;
    int outer_component_index = search_half_edge_table(outer_component, half_edge_table);
    index = outer_component_index;
    head = half_edge_table[outer_component_index].half_edge;
    temp = head;
    bool flag = 0;
    while(1)
    {
        temp = half_edge_table[outer_component_index].half_edge;
        if(temp == half_edge_table[stored_index].half_edge){
            flag = 1;
            break;
        }
        outer_component_index++;
        if (temp == head) break; 
    }
    //cout << flag <<"\n";
    int indext;
    if(flag){ // For handaling outer edge
        head = half_edge_table[index].half_edge;
        temp = head;
        while(1)
        {   
            indext = search_half_edge_table (temp->twin, half_edge_table);
            m[half_edge_table[indext].incident_face->key] = 1;
            temp = half_edge_table[index].next;
            if (temp == head) 
                break;
            index++; 
        }
        vector<class half_edge *> inner_components = face_table[face_table_index].inner_components;

        for (int i=0; i<inner_components.size(); i++)
        {
            index = search_half_edge_table(inner_components[i]->twin, half_edge_table);
            m[half_edge_table[index].incident_face->key]=1;
        }
        for (auto j: m)
        {
            cout<<"F"<<j.first<<endl;
        }
    }
    else
    {
        print_neighbouring_faces(outer_component->origin_v,outer_component->end_v,half_edge_vector,half_edge_table,face_table);
    }
}

void DCEL(int nodes, int edges, vector<Point> &vertices, vector<pair<int,int>> &diagonals){
    int d = diagonals.size();
    vector<half_edge> h(2*edges + 2*d);
    vector<vertex_table> ver_tab(nodes);
    vector<half_edge_table> half_edge_table(2*edges + 2*d);
    vector<vector <float> > adj(nodes);
    vector<bool> unvisited_half_edge(2*edges+ 2*d);
    vector<face> face(nodes);
    vector<face_table> face_table(nodes);
    vector<pair<int,int>> allEnD;

    for(int i=0;i<edges;i++)
        allEnD.push_back({i,(i+1)%nodes});
    for(auto p: diagonals)
        allEnD.push_back({p.first,p.second});

    int i=0;
    for(auto p: allEnD)
    {
        int x=p.first;
        int y=p.second;
        adj[x].push_back(y);
        adj[y].push_back(x);
        h[2*i].origin_v = x;
        h[2*i].end_v = y;
        h[2*i].origin = &vertices[x];
        h[2*i].end = &vertices[y];
        h[2*i + 1].origin_v = y;
        h[2*i + 1].end_v = x;
        h[2*i + 1].origin = &vertices[y];
        h[2*i + 1].end = &vertices[x];
        h[2*i].twin = &h[2*i+1];
        h[2*i + 1].twin = &h[2*i];
        i++;
    }



    fill_vertex_table(ver_tab , nodes , adj , h , vertices);
    // cout <<"In dcel" << endl;
    fill_half_edge_table(half_edge_table , h , unvisited_half_edge , vertices , adj , face , face_table);
    fill_face_table_inner_components(face_table, h , half_edge_table , face , vertices);
    //print_half_edge(h , vertex , 2*edges);
    print_vertex_table(ver_tab , nodes);
    print_half_edge_table(half_edge_table , h);
    print_face_table(face_table);


    

    // isolate polygons 
    for(int i=0;i<face_table.size();i++)
    {
        vector<Point> polygon;
        class face_table temp=face_table[i];
        if(temp.outer_component==NULL)
            continue;
        polygon.push_back(*(temp.outer_component->origin));
        int indexOfBeginEgde = search_half_edge_table(temp.outer_component, half_edge_table);
		// cout<<indexOfBeginEgde<<" this si the eindex\n";
        while(half_edge_table[indexOfBeginEgde].next!=temp.outer_component)
        {
            polygon.push_back(*(half_edge_table[indexOfBeginEgde].next->origin));
			//cout<<" pushing  these are the points"<<half_edge_table[indexOfBeginEgde].next->origin_v<<" "<<half_edge_table[indexOfBeginEgde].next->end_v<<"\n";
            indexOfBeginEgde++;
        }
        POLYGONS.push_back(polygon);
    }
    for(auto polygon: POLYGONS)
    {
        cout<<"new polygon \n";
        for(auto p : polygon )
        {
            cout<<p.key<<" "<<p.x<<" "<<p.y<<" \n";
        }
    }
    
    setArguments(half_edge_table, h, vertices, ver_tab, face_table);
   
    return;
}
