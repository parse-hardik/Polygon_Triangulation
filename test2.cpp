Skip to content
Search or jump to…

Pull requests
Issues
Marketplace
Explore
 
@judyhopps24 
Learn Git and GitHub without any code!
Using the Hello World guide, you’ll start a branch, write comments, and open a pull request.


Mindjolt2406
/
Polygon-Triangulation
1
00
Code
Issues
Pull requests
Actions
Projects
Wiki
Security
Insights
Polygon-Triangulation/main.cpp
@Mindjolt2406
Mindjolt2406 Added GUI, Fixed bugs, made All steps work
Latest commit 98684c7 on Oct 4
 History
 1 contributor
409 lines (361 sloc)  11.7 KB
  
#ifndef MAIN
#define MAIN
#include<bits/stdc++.h>
// #include "PolygonPoint.h"
#include "dcel.h"
#endif

// g++ -std=c++17 -Wl,-stack_size -Wl,0x10000000 main.cpp
#define mt make_tuple
#define pu push_back
#define MOD 1000000007
#define EPS 1e-6
#define ll long long int
#define ld long double
#define fi first
#define se second
#define all(v) v.begin(),v.end()
#define pr(v) { for(int i=0;i<v.size();i++) { v[i]==INF? cout<<"INF " : cout<<v[i]<<" "; } cout<<endl;}
#define __ {ios::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);}

using namespace std;
template <int> ostream& operator<<(ostream& os, const vector<int>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { if(v[i]!=INF) os << v[i]; else os << "INF";if (i != v.size() - 1) os << ", "; } os << "]\n"; return os; } 
template <typename T> ostream& operator<<(ostream& os, const vector<int>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { os << v[i]; ;if (i != v.size() - 1) os << ", "; } os << "]\n"; return os; } 


ld sweepY = 0;
bool enableRotation = true;
vector<pair<PolygonPoint*, PolygonPoint*> > diagonalPoints;

void drawDiagonal(PolygonPoint *point1, PolygonPoint *point2)
{
  // diagonals.pu(Line(point1.getPoint(), point2.getPoint()));
  diagonalPoints.pu(mp(point1, point2));
}

struct pointComp
{
  bool operator()(const PolygonPoint * point1, const PolygonPoint* point2)
  {
    return *point1 < *point2;
  }
};

void generateInverseHelper(vector<PolygonPoint*> &points, map<PolygonPoint,Line> &inverseHelper)
{
  set<Line> setLines;

  for(auto pointer : points)
  {
    PolygonPoint point = *pointer;
    // Remove lines where this point is the lower point
    // Now, if there is a line which goes down
    // Start and end add lines before and then do
    // Merge and Split, check before
    // polygon on the left of regular, then add after. Otherwise add before. 

    // Updating the sweep line 
    sweepY = point.getY();

    if(point.isStartPoint())
    {
      Line tempLine = point.getNextLine();
      setLines.insert(tempLine);
      inverseHelper[point] = point.getNextLine();
      point.setLeftEdge(point.getNextLine());
    }
    else if(point.isEndPoint())
    {
      setLines.erase(point.getPrevLine());
      // No point, but for sake of completion
      inverseHelper[point] = point.getPrevLine();
      point.setLeftEdge(point.getPrevLine());
    }
    else if(point.isSplitPoint())
    {
      point.updateLeftEdge(setLines, inverseHelper);
      setLines.insert(point.getNextLine());
    }
    else if(point.isMergePoint())
    {
      setLines.erase(point.getPrevLine());
      point.updateLeftEdge(setLines, inverseHelper);
    }
    else
    {
      if(point.isRegularFacingLeft()) 
      {
        point.updateLeftEdge(setLines, inverseHelper);
      }
      else
      {
        setLines.erase(point.getPrevLine());
        inverseHelper[point] = point.getNextLine();
        point.setLeftEdge(point.getNextLine());
        setLines.insert(point.getNextLine());
      }
    }
  }
}

void printInverseHelper(map<PolygonPoint, Line> &inverseHelper)
{
  // Prints inverseHelper
  for(auto it : inverseHelper)
  {
    PolygonPoint point = it.first;
    Line line = it.second;
    point.rotate(-defaultAngle);
    line.deRotate();
    cerr << point.getPoint().to_string() << " " << line.to_string() << endl;
    point.rotate(defaultAngle);
    // line.rotate();
  }
}

void triangulateMonotonePolygon(vector<PolygonPoint*> &polygon, vector<Line> &monotoneDiagonals)
{
  // Sort w.r.t decreasing y coordinate
  sort(polygon.begin(), polygon.end(), [](PolygonPoint* point1, PolygonPoint* point2)
  {
    return point1->getY() < point2->getY();
  });
  reverse(polygon.begin(),polygon.end());

  stack<PolygonPoint*> pointStack;

  set<pair<ld,ld> > leftChain;

  PolygonPoint* tempPoint = polygon[0];
  PolygonPoint* currPoint = tempPoint;

  while(*tempPoint != *polygon.back())
  {
    leftChain.insert(tempPoint->getPair());
    tempPoint = tempPoint->getNextPointer();
  }

  _;
  Vertex* vertex = new Vertex(currPoint->getPoint());
  bool chainIsLeft = vertex->lessPoint(currPoint->getNextPointer()->getPoint(), currPoint->getPrevPointer()->getPoint());
  chainIsLeft = !chainIsLeft;
   
  pointStack.push(polygon[0]);
  pointStack.push(polygon[1]);

  for(int i=2;i<(int)polygon.size()-1;i++)
  {
    PolygonPoint* currPointer = polygon[i];
    

    // Different chains
    if(leftChain.count(currPointer->getPair()) ^ leftChain.count(pointStack.top()->getPair()))
    {
      if(i == 2) t(currPointer->getPair());
      while(pointStack.size() > 1)  
      {
        PolygonPoint* stackTop = pointStack.top();
        Line tempDiagonal(stackTop->getPoint(),currPointer->getPoint());
        monotoneDiagonals.pu(tempDiagonal);
        pointStack.pop();
      }
      pointStack.pop();
      pointStack.push(polygon[i-1]);
      pointStack.push(currPointer);
    }
    else // Same chain
    {
      pair<ld,ld> tempPair = mp(690,250);
      bool isLeft = true;
      if(chainIsLeft ^ leftChain.count(currPointer->getPair())) isLeft = false;

      PolygonPoint* tempMiddlePointer = pointStack.top();
      pointStack.pop();
      while(!pointStack.empty())
      {
        PolygonPoint* tempAbovePointer = pointStack.top();
        // Some checking
        Line tempDiagonal(currPointer->getPoint(), tempAbovePointer->getPoint());
        if(isLeft)
        {
          if(tempDiagonal.on_left(tempMiddlePointer->getPoint()))
          {
            // Add Diagonal
            monotoneDiagonals.pu(tempDiagonal);
          }
          else break;
        }
        else
        {
          if(tempDiagonal.on_right(tempMiddlePointer->getPoint()))
          {
            // Add Diagonal
            monotoneDiagonals.pu(tempDiagonal);
          }
          else break;
        }

        tempMiddlePointer = tempAbovePointer;
        pointStack.pop();
      }
      pointStack.push(tempMiddlePointer);
      pointStack.push(currPointer);
    }
  }

  pointStack.pop();
  while(pointStack.size() > 1)
  {
    PolygonPoint* stackTop = pointStack.top();
    Line tempDiagonal(stackTop->getPoint(),(polygon.back())->getPoint());
    monotoneDiagonals.pu(tempDiagonal);
    pointStack.pop();
  }
  pointStack.pop();
}

void triangulateMonotonePolygons(vector<vector<PolygonPoint*> > &polygons, vector<vector<Line> > &diagonals, int numPoints)
{
  bool fullPolygon = false;
  for(auto monotonePolygon : polygons)
  {
    vector<Line> diagonal;
    if(monotonePolygon.size() == numPoints && !fullPolygon) {fullPolygon = true;continue;}
    triangulateMonotonePolygon(monotonePolygon, diagonal);

    diagonals.pu(diagonal);
  }
}

int main()
{
  __;
  int numPoints;
  cin >> numPoints;
  vector<PolygonPoint*> points;

  for(int i=0;i<numPoints;i++)
  {
    ld x,y;
    cin >> x >> y;

    PolygonPoint *p = new PolygonPoint(x,y);
    if(enableRotation) p->rotate();
    points.push_back(p);
  }

  for(int i=0;i<numPoints;i++)
  {
    points[i]->setNextPointer(points[(i+1) % numPoints]);
    points[i]->setPrevPointer(points[(i-1+numPoints)%numPoints]);
    points[i]->setType();
    // t(points[i]->to_string());
  }

  sort(points.begin(),points.end(),[](PolygonPoint* a, PolygonPoint* b) { return *a < *b; });
  reverse(points.begin(), points.end());

  set<Line> setLines;
  map<PolygonPoint, Line> inverseHelper;

  generateInverseHelper(points,inverseHelper);
  printInverseHelper(inverseHelper);

  map<pair<pair<ld,ld>, pair<ld,ld> > , PolygonPoint*> helper;
  for(auto pointer : points)
  {
    PolygonPoint point = *pointer;
    // cerr << "Processing point: " << point.to_string() << endl;
    // Updating the sweep line
    sweepY = point.getY();

    if(point.isStartPoint())
    {
      Line leftLine = point.getNextLine();
      setLines.insert(leftLine);
      helper[leftLine.getPair()] = pointer;
    }
    else if(point.isEndPoint())
    {
      Line prevLine = point.getPrevLine();
      if(helper[prevLine.getPair()]->isMergePoint())
      {
        // Draw Diagonal from point to helper[prevLine.getPair()]
        drawDiagonal(helper[prevLine.getPair()], pointer);
      }
      setLines.erase(point.getPrevLine());
    }
    else if(point.isSplitPoint())
    {
      Line leftLine = inverseHelper[point];
      // Draw Diagonal from point to helper[leftLine.getPair()]
      drawDiagonal(helper[leftLine.getPair()], pointer);
      helper[leftLine.getPair()] = pointer;
      Line nextLine = point.getNextLine();
      setLines.insert(nextLine);
      helper[nextLine.getPair()] = pointer;
    }
    else if(point.isMergePoint())
    {
      Line prevLine = point.getPrevLine();
      if(helper[prevLine.getPair()]->isMergePoint())
      {
        // Draw Diagonal from point to helper[prevLine.getPair()]
        drawDiagonal(helper[prevLine.getPair()], pointer);
      }
      setLines.erase(prevLine);
      Line leftLine = inverseHelper[point];
      if(helper[leftLine.getPair()]->isMergePoint())
      {
        // Draw a diagonal from pont to helper[leftLine.getPair()]
        drawDiagonal(helper[leftLine.getPair()], pointer);
      }
      helper[leftLine.getPair()] = pointer;
    }
    else
    {
      if(point.isRegularFacingLeft())
      {
        Line leftLine = inverseHelper[point];
        if(helper[leftLine.getPair()]->isMergePoint())
        {
          // Draw a diagonal between helper[leftLine.getPair()] and point
          drawDiagonal(helper[leftLine.getPair()], pointer);
        }
        helper[leftLine.getPair()] = pointer;
      }
      else
      {
        Line prevLine = point.getPrevLine();
        if(helper[prevLine.getPair()]->isMergePoint())
        {
          // Draw a diagonal between helper[prevLine] and point
          drawDiagonal(helper[prevLine.getPair()], pointer);
        }
        setLines.erase(prevLine);
        Line nextLine = point.getNextLine();
        helper[nextLine.getPair()] = pointer;
        setLines.insert(nextLine);
      }
    }

    // Debugging the helper function
    // cout << "SweepY: " << sweepY << " setLines.size(): " << setLines.size() << endl;
    // for(auto it : helper)
    // {
    //   cout << it.fi << " " << (it.second)->getPair() << endl;
    // }
  }

  // Print helper lines
  // for(auto it : helper)
  // {
  //   pair<pair<ld,ld> , pair<ld,ld> >  line = it.first;
  //   PolygonPoint point = it.second;
  //   t(line, point.getPair());
  // }

  vector<Line> sides,allLines;
  for(int i=0;i<numPoints;i++) sides.push_back(points[i]->getNextLine());

  // cout << diagonalPoints.size() + numPoints << endl;
  // cout << 2*numPoints - 3 << endl;
  // printOutputForPlot();
  cout << sides.size() << endl;
  for(auto line : sides)
  {

    allLines.pu(line);
    if(enableRotation) line.deRotate();
    auto pair = line.getPair();
    cout << pair.fi.fi << " " << pair.fi.se << " " << pair.se.fi << " " << pair.se.se << endl;
  }

  cout << diagonalPoints.size() << endl;
  for(auto line : diagonalPoints)
  {
    Line tempLine = Line((line.fi)->getPoint(),(line.se)->getPoint());
    allLines.pu(tempLine);

    if(enableRotation) tempLine.deRotate();
    auto pair = tempLine.getPair();
    cout << pair.fi.fi << " " << pair.fi.se << " " << pair.se.fi << " " << pair.se.se << endl;
  }

  vector<vector<PolygonPoint*> > polygons;
  assignLinesToPolygons(allLines,polygons);

  vector<vector<Line> > monotoneDiagonals;
  triangulateMonotonePolygons(polygons,monotoneDiagonals,numPoints);


  int monotoneDiagonalSize = 0;
  for(auto diagonals : monotoneDiagonals) if(diagonals.size()) monotoneDiagonalSize++;

  cout << monotoneDiagonalSize << endl;
  for(auto diagonals : monotoneDiagonals)
  {
    if(diagonals.size() == 0) continue;
    cout << diagonals.size() << endl;
    for(auto line : diagonals)
    {
      if(enableRotation) line.deRotate();

      auto pair = line.getPair();
      cout << pair.fi.fi << " " << pair.fi.se << " " << pair.se.fi << " " << pair.se.se << endl;
    }
  }
  return 0;
}

