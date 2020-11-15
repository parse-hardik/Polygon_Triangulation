#include <iostream>
#include <vector>

using namespace std;

struct Point{
	int x,y;
};

int main(int argc, char** argv) {
	int n;
	cout << "Enter the number of vertices of the polygon" << endl;
	cin >> n;
	vector<Point> vertices(n);
	cout << "Enter all vertices in counter-clockwise manner" << endl;
	for(int i=0;i<n;i++)
		cin >> vertices[i].x >> vertices[i].y;
}
