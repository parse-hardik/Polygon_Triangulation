#include <iostream>
#include <bits/stdc++.h>

#include "dcel.hpp"

//Checks if the 2 points are on opposite chains or not
//Used in the triangulate code
bool AreOnOppSide(Point p,Point q, map<int,int> &left,map<int,int> &right)
{
	if((left[p.key]==1 && right[q.key]==1) || (left[q.key]==1 && right[p.key]==1) )
		return true;
	else
		return false;
}

//Compares two points based on their y-coordinates
bool ComparePoint_t(Point p, Point q) 
{ 
	return (p.y > q.y) || (p.y==q.y && p.x < q.x); 
}

//Takes all the vertices sorted according to y and then x coordinate
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
	// cout<<"order of vertices is\n";
	// for(auto p: vertices_sorted_toy_t )
	// 	cout<<p.key<<" "<<p.x<<" "<<p.y<<"\n";
	
	stack<Point> st_t;
	st_t.push(vertices_sorted_toy_t[0]);
	st_t.push(vertices_sorted_toy_t[1]);
	for(int j=2;j<n_t-1;j++)
	{
		if(AreOnOppSide(vertices_sorted_toy_t[j],st_t.top(),left,right))
		{
			// cout<<"they are on opp side "<<st_t.top().key<<" "<<vertices_sorted_toy_t[j].key<<"\n";
			while(st_t.size()>1)//pop and make diagonals except the last one
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
				// cout<<val<<" this is is convex result";
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