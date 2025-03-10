#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>

#include "dcel.hpp"
 
using namespace std;

bool AreOnOppSide(Point p,Point q, map<int,int> &left,map<int,int> &right)
{
	if((left[p.key]==1 && right[q.key]==1) || (left[q.key]==1 && right[p.key]==1) )
		return true;
	else
		return false;
}

bool ComparePoint_t(Point p, Point q) 
{ 
	return (p.y > q.y) || (p.y==q.y && p.x < q.x); 
}

void triangulatePolygon(vector<Point> vertices_ccw_t)
{
	map<int,int> GLOBAL_INDEX;

	int n_t=vertices_ccw_t.size();
	vector<Point> vertices_sorted_toy_t(n_t);
	for(int i=0;i<n_t;i++)
	{
		GLOBAL_INDEX[i]=vertices_ccw_t[i].key;
		vertices_sorted_toy_t[i].key=i;
		vertices_ccw_t[i].key=i;
		vertices_sorted_toy_t[i].x=vertices_ccw_t[i].x;
		vertices_sorted_toy_t[i].y=vertices_ccw_t[i].y;
		vertices_sorted_toy_t[i].key=vertices_ccw_t[i].key;
		
	}
	map<int,int> right,left;
	for(int i=1;i<n_t;i++)
	{
		if(vertices_sorted_toy_t[i].y<vertices_sorted_toy_t[i-1].y)//im not sure about = condition
			left[vertices_sorted_toy_t[i].key]=1;
		else if(vertices_sorted_toy_t[i].y>vertices_sorted_toy_t[i-1].y)
		{
			right[vertices_sorted_toy_t[i].key]=1;
		}		
	}
    sort(vertices_sorted_toy_t.begin(), vertices_sorted_toy_t.end(), ComparePoint_t);
	cout<<"order of vertices is\n";
	for(auto p: vertices_sorted_toy_t )
		cout<<p.key<<" "<<p.x<<" "<<p.y<<"\n";
	
	stack<Point> st_t;
	st_t.push(vertices_sorted_toy_t[0]);
	st_t.push(vertices_sorted_toy_t[1]);
	for(int j=2;j<n_t-1;j++)
	{
		if(AreOnOppSide(vertices_sorted_toy_t[j],st_t.top(),left,right))
		{
			cout<<"they are on opp side "<<st_t.top().key<<" "<<vertices_sorted_toy_t[j].key<<"\n";
			while(st_t.size()>1)//ppop and make diagonals except the last one
			{
				Point temp_t = st_t.top();
				st_t.pop();
				diagonals.push_back({GLOBAL_INDEX[temp_t.key],GLOBAL_INDEX[vertices_sorted_toy_t[j].key]});
			}
			st_t.pop();// pop the last one
			st_t.push(vertices_sorted_toy_t[j-1]);
			st_t.push(vertices_sorted_toy_t[j]);
		}
		else// same side
		{
			// cout<<"they are on same side "<<st_t.top().key<<" "<<vertices_sorted_toy_t[j].key<<"\n";
			Point middle_temp_t=st_t.top();
			st_t.pop();
			bool isonleft;// variable to hold which side it is on
			if(left[vertices_sorted_toy_t[j].key]==1)
				isonleft=true;
			if(right[vertices_sorted_toy_t[j].key]==1)
				isonleft=false;
			while(!st_t.empty())
			{
				Point above_temp_t =st_t.top();
				int val=IsConvex(above_temp_t,middle_temp_t,vertices_sorted_toy_t[j]);
				cout<<val<<" this is is convex result";
				if(isonleft)//if point on left chain
				{
					if(val==1)//cross product is negative
					{
						diagonals.push_back({GLOBAL_INDEX[vertices_sorted_toy_t[j].key],GLOBAL_INDEX[above_temp_t.key]});
					}
					else
						break;
				}
				else//if point is on right chain
				{
					if(val==0)// cross product is the positve
					{
						diagonals.push_back({GLOBAL_INDEX[vertices_sorted_toy_t[j].key],GLOBAL_INDEX[above_temp_t.key]});
					}
					else
						break;
				}
				middle_temp_t=above_temp_t;
				st_t.pop();
			}
			st_t.push(middle_temp_t);
			st_t.push(vertices_sorted_toy_t[j]);
		}		
	}
	//Add diagonals from un to all stack vertices except the first and the last one.
	st_t.pop();
	while(st_t.size()>1)
	{
		Point random_top=st_t.top();
		diagonals.push_back({GLOBAL_INDEX[random_top.key],GLOBAL_INDEX[vertices_sorted_toy_t[n_t-1].key]});
		st_t.pop();
	}
	 st_t.pop();

	cout<<"added diagonals\n";
	for(auto d: diagonals)
		cout<<d.first<<" "<<d.second<<"\n";
}

void Make_Monotone(vector<Point> &vertices){
    int n = vertices.size();
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

	DCEL(n, n, vertices, diagonals);

	// isolate polygons 
    for(int i=0;i<ft.size();i++)
    {
        vector<Point> polygon;
        class face_table temp=ft[i];
        if(temp.outer_component==NULL)
            continue;
        polygon.push_back(*(temp.outer_component->origin));
        int indexOfBeginEgde = search_half_edge_table(temp.outer_component, het);
        while(het[indexOfBeginEgde].next!=temp.outer_component)
        {
            polygon.push_back(*(het[indexOfBeginEgde].next->origin));
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
	
	for(auto polygon: POLYGONS)
	{
		cout<<"triangulating a polygon\n";
		triangulatePolygon(polygon);
	}
}