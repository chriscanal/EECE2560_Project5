// main.cpp
// Problem Set 5a                                 Chris Canal
// Team cansym                                    canal.c@husky.neu.edu
//                                                Sarada Symonds
//                                                symonds.s@husky.neu.edu
//
// Main program file for homework 5a.
// Project 5

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "Graph.h"
#include <list>
#include <queue>
#include <unordered_map>

using namespace std;



class maze
{
   public:
      maze(ifstream &fin);
      void print(int,int,int,int);
      bool isLegal(int i, int j);

      void setMap(int i, int j, int n);
      int getMap(int i, int j) const;
      void mapMazeToGraph(graph &g);
	  bool findPathRecursive(graph &g, int id);
      void findPathNonRecursive(graph &g);
      void print(graph &g, int id);
      bool isPath(graph &g, int n, int cols);
      void breadthFirst(graph& g);
      void findShortestPath2(graph &g);

      void stackToSolutionMap(std::stack<int> solutionStack);
      void printPathOnMaze();
      void setSolutionMap(int i, int j);
      void queueToSolutionMap(std::queue<int> solutionQueue);

   private:
      int rows; // number of rows in the maze
      int cols; // number of columns in the maze

      matrix<bool> value;
      matrix<int> solutionMap;
      matrix<int> map;      // Mapping from maze (i,j) values to node index values
};

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
   fin >> rows;
   fin >> cols;

   char x;

   value.resize(rows,cols);
   for (int i = 0; i <= rows-1; i++)
      for (int j = 0; j <= cols-1; j++)
      {
	 fin >> x;
	 if (x == 'O')
            value[i][j] = true;
	 else
	    value[i][j] = false;
      }

   map.resize(rows,cols);
   solutionMap.resize(rows,cols);
    for (int i = 0; i <= rows-1; i++){
        for (int j = 0; j <= cols-1; j++){
            solutionMap[i][j] = false;
        }
    }
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
   cout << endl;

   if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
      throw rangeError("Bad value in maze::print");

   if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
      throw rangeError("Bad value in maze::print");

   for (int i = 0; i <= rows-1; i++)
   {
      for (int j = 0; j <= cols-1; j++)
      {
	 if (i == goalI && j == goalJ)
	    cout << "*";
	 else
	    if (i == currI && j == currJ)
	       cout << "+";
	    else
	       if (value[i][j])
		  cout << " ";
	       else
		  cout << "X";
      }
      cout << endl;
   }
   cout << endl;
}


bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze.
{
   if (i < 0 || i > rows || j < 0 || j > cols)
      throw rangeError("Bad value in maze::isLegal");

   return value[i][j];
}



// -------------------- Functions that cansym wrote --------------------
void maze::stackToSolutionMap(std::stack<int> solutionStack){
    int current = 0;
    int i;
    int j;
    while (!solutionStack.empty()){
        current = solutionStack.top();
        i = current/cols;
        j = current%cols;
        solutionMap[i][j] = true;
        solutionStack.pop();
    }
}

void maze::queueToSolutionMap(std::queue<int> solutionQueue){
    int current = 0;
    int i;
    int j;
    while (!solutionQueue.empty()){
        current = solutionQueue.front();
        i = current/cols;
        j = current%cols;
        solutionMap[i][j] = true;
        solutionQueue.pop();
    }
}

void maze::setSolutionMap(int i, int j){
    solutionMap[i][j] = true;
}

void maze::printPathOnMaze(){
    {
        cout << "\n";
       for (int i = 0; i <= rows-1; i++)
       {
          for (int j = 0; j <= cols-1; j++){
              if (solutionMap[i][j]){
                   cout << "*";
              } else if (value[i][j]){
                  cout << " ";
              }else{
                  cout << "X";
              }

          }
          cout << endl;
       }
       cout << endl;
    }
}

void maze::mapMazeToGraph(graph &g)
// Create a graph g that represents the legal moves in the maze m.
{
    int id = 0;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            node * n = new node();
            n->setId(id);
            g.addNode(*n);
			setMap(i, j, id);
            id++;
		}
	}

	for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {


			//adds edge to top
            if(i != 0) {
                if(value[i - 1][j] == true) {
                    g.addEdge(map[i][j], map[i][j] - cols, 1);
                }
            }

            //adds edge to right
            if(j != cols - 1) {
                if(value[i][j + 1] == true) {
                    g.addEdge(map[i][j], map[i][j] + 1, 1);
                }
            }

            //adds edge to bottom
            if(i != rows - 1) {
                if(value[i + 1][j] == true) {
                    g.addEdge(map[i][j], map[i][j] + cols, 1);
                }
            }

            //adds edge for left
            if(j != 0) {
                if(value[i][j - 1] == true) {
                    g.addEdge(map[i][j], map[i][j] - 1, 1);
                }
            }
        }
    }
}


void maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n.
{
	map[i][j] = n;
}

int maze ::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{
	return map[i][j];
}

int setDirection(int i, int id, int cols){
	if(i == 0) {
		return id - cols;
	} else if(i == 1) {
		return id + 1;
	} else if(i == 2) {
		return id + cols;
	} else {
		return id - 1;
	}
}

bool maze::findPathRecursive(graph &g, int id) {
    //check for path going up
    int dst;
    if(id == g.numNodes() - 1) {
        g.mark(id);
		cout << "\n";
        return true;
    } else {
        for(int i = 0; i < 4; i++)
        //checks the four directions
        {
            dst = setDirection(i, id, cols);
            if(!(dst < 0 | dst >= g.numNodes())) {
                if(!g.isVisited(dst)) {
                    g.visit(id);
                    if(g.isEdge(id, dst)) {
                        if(findPathRecursive(g, dst)) {
                            g.mark(id);
                            return true;
                        }
                    }

                }

            }

        }
        return false;
    }

}

void maze::print(graph &g, int id) {
    int dst;
    int dir;
    if(id == g.numNodes() - 1) {
		cout << "End of path :)\n";
    } else {
        for(int i = 0; i < 4; i++)
        //checks the four directions
        {
            dst = setDirection(i, id, cols);
            if(dst >= 0 && dst < g.numNodes()) {
                if(!g.isVisited(dst) && g.isMarked(dst)) {
                    g.visit(id);
                    dir = dst - id;
                    if(dir == -cols) {
                        cout << "Go up\n";
                        print(g, dst);
                        return;
                    } else if(dir == cols) {
                        cout << "Go down\n";
                        print(g, dst);
                        return;
                    } else if(dir == 1) {
                        cout << "Go right \n";
                        print(g, dst);
                        return;
                    } else if(dir == -1){
                        cout << "go left\n";
                        print(g, dst);
                        return;
                    } else {
                        cout << "bad direction\n";
                    }

                }

            } else if (id != 0 && i == 4) {
                cout << "\nno path\n";
            }

        }
    }
}


void printStack(std::stack<int> theStack){
    int id = 0;
    cout << "{ ";
    while (theStack.size() > 0){
        id = theStack.top();
        theStack.pop();
        cout << id;
        if (theStack.size() == 0){
        } else {
            cout << ", ";
        }
    }
    cout << " }\n";
}

void printQueue(std::queue<int> theQueue){
    int id = 0;
    cout << "{ ";
    while (theQueue.size() > 0){
        id = theQueue.front();
        theQueue.pop();
        cout << id;
        if (theQueue.size() == 0){
        } else {
            cout << ", ";
        }
    }
    cout << " }\n";
}

void maze::findPathNonRecursive(graph &g)
{
    int id = 0;
    int n;
    std::stack<int> pathStack;
    std::stack<int> visitingStack;
    std::stack<int> popNumbers;
    visitingStack.push(id);
    n = visitingStack.top();
    while(!visitingStack.empty()) {
        if(n == g.numNodes() - 1) {
            pathStack.push(n);
            break;
        }
        //cout << "\n n: " << n;
        if(!g.isVisited(n)) {
            g.visit(n);
            if(n + 1 <= (g.numNodes() - 1) && (n + 1) >= 0) {
                if(g.isEdge(n, n + 1) && !g.isVisited(n + 1)) {
                    visitingStack.push(n + 1);
                }
            }
            if(n + cols <= (g.numNodes() - 1) && (n + cols) >= 0) {
                if(g.isEdge(n, n + cols) && !g.isVisited(n + cols)) {
                    visitingStack.push(n + cols);
                }
            }
            if((n - 1) <= (g.numNodes() - 1) && (n - 1) >= 0) {
                if(g.isEdge(n, n - 1) && !g.isVisited(n - 1)) {
                    visitingStack.push(n - 1);
                }
            }
            if((n - cols) <= (g.numNodes() - 1) && (n - cols) >= 0) {
                if(g.isEdge(n, n - cols) && !g.isVisited(n - cols)) {
                    visitingStack.push(n - cols);
                }
            }

        }
        pathStack.push(n);
        if(isPath(g, n, cols)) {
            n = visitingStack.top();
            visitingStack.pop();

        } else {
            while(!isPath(g, n, cols)) {
                pathStack.pop();
                n = pathStack.top();

            }
        }
    }
    stackToSolutionMap(pathStack);
    while(!pathStack.empty()) {
        id = pathStack.top();
        g.mark(id);
        pathStack.pop();
    }
}

bool maze::isPath(graph &g, int n, int cols) {
    int dst;
    bool p = false;
    for(int i = 0; i < 4; i++) {
        if(i == 0) {
            dst = n + 1;
        } else if(i == 1) {
            dst = n + cols;
        } else if(i == 2) {
            dst = n - 1;
        } else {
            dst = n - cols;
        }
        if (dst < g.numNodes() && dst >= 0) {
            if(g.isEdge(n, dst) && !g.isVisited(dst)) {
                p = true;
            }
        }

    }
    return p;

}

void maze::breadthFirst(graph& g) {
    std::queue<std::queue<int> > pathQueue;
    std::queue<int> initPath;
    initPath.push(0);
    pathQueue.push(initPath);
    int currId = 0;
    int dst;
    std::queue<int> currPath;
    while(currId != g.numNodes() - 1) {
        currPath = pathQueue.front();
        pathQueue.pop();
        currId = currPath.back();
        g.visit(currId);
        if(currId == g.numNodes() - 1) {
            int idMark;
            while(currPath.size() != 0) {
                idMark = currPath.front();
                g.mark(idMark);
                currPath.pop();

            }
            break;
        }
        for(int i = 0; i < 4; i++) {
            dst = setDirection(i, currId, cols);
            if(!(dst < 0 | dst >= g.numNodes()) && !g.isVisited(dst)) {
                if(g.isEdge(currId, dst)) {
                    if(i == 0) {
                        currPath.push(dst);
                        pathQueue.push(currPath);
                    } else {
                        std::queue<int> newPath;
                        newPath = currPath;
                        newPath.push(dst);
                        pathQueue.push(newPath);
                    }
                }
            }
        }
    }
}

void maze::findShortestPath2(graph &g) {
    g.setEdgeWeights(1);
    std::unordered_map<int,std::queue<int> > nodeMap;
    int k = 0;
    int currId = 0;
    int dst;
    std::queue<int> * initPath;
    std::queue<int> * emptyPath;
    std::queue<int> * newPath;
    while(k < g.numNodes()) {
        if(k == 0) {
            initPath = new std::queue<int>;
            initPath->push(k);
            nodeMap[k] = *initPath;
        } else {
            emptyPath = new std::queue<int>;
            nodeMap[k] = *emptyPath;
        }
        k++;
    }
    while(!g.allEdgesVisited() && currId < g.numNodes()) {
        //cout << "\nvisiting node: " << currId;
        if (currId < g.numNodes()){
            g.visit(currId);
        }
        for(int i = 0; i < 4; i++) {
            dst = setDirection(i, currId, cols);
            cout << "\n-------------------------------------";
            cout << "\nSet Direction:" << i;
            if(dst >= 0 && dst < g.numNodes() ) {
                cout << "\nDirection " << i << " in bounds";
                if(g.isEdge(currId, dst)) {
                    cout << "\nDestination " << dst << " is valid from dst from node " << currId;
                    newPath = new std::queue<int>;
                    *newPath = nodeMap[currId];
                    newPath->push(dst);
                    cout << "\nPath Que:\n";
                    printQueue(*newPath);
                    cout << "\n";
                    queueToSolutionMap(*newPath);
                    cout << "\n";
                    printPathOnMaze();
                    cout << "\n";
                    if(g.isVisited(dst)) {
                        if( nodeMap[dst].size() > newPath->size() ) {
                            nodeMap[dst] = *newPath;
                        }
                    } else  {
                        nodeMap[dst] = *newPath;
                    }
                    cout << "\nVisiting node " << dst;
                    cout << "\nCurrent Id " << currId;
                    g.visit(dst);
                    g.visit(currId, dst);
                    cout << "\n-------------------------------------";
                }
            }
        }
        currId++;
    }
    for(int i = 0; i < g.numNodes() - 1; i++) {
        cout << "path to node: " << i << "/path size: " << nodeMap[i].size() << endl;
    }
    std::queue<int> finalPath;


    finalPath = nodeMap[(g.numNodes() - 1)];
    queueToSolutionMap(finalPath);
    int cur;
    while(finalPath.size() != 0) {

        cur = finalPath.front();
        cout << "\nvisiting node: " << cur;
        finalPath.pop();
        g.mark(cur);
    }


}

int main()
{
   char x;
   ifstream fin;
   // Read the maze from the file.
   string fileName = "maze3.txt";
   cout << "\n OPENING: " << fileName << endl;

   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }

   try
   {
      graph g3;
      while (fin && fin.peek() != 'Z')
      {
         maze m3(fin);
		 m3.mapMazeToGraph(g3);
		 m3.print(0,0,1,0);
         g3.clearVisit();
         g3.clearMark();
         m3.findShortestPath2(g3);
         cout << "found shortest pth 2\n";
         g3.clearVisit();
         m3.print(g3, 0);
         m3.printPathOnMaze();
         /*cout << "Recursive Path Finder\n";
         cout << "*-------------------------------------------*\n";
		 m3.findPathRecursive(g3, 0);
         g3.clearVisit();
         m3.print(g3, 0);
         g3.clearMark();
         g3.clearVisit();
         cout << "\n\nNon-Recursive Path Finder (Depth First Search)\n";
         cout << "*-------------------------------------------*\n";
         m3.findPathNonRecursive(g3);
         m3.printPathOnMaze();
         g3.clearVisit();
         m3.print(g3, 0);*/


      }

   }
   catch (indexRangeError &ex)
   {
      cout << ex.what() << endl; exit(1);
   }
   catch (rangeError &ex)
   {
      cout << ex.what() << endl; exit(1);
   }
   fin.close();
/*
   // Read the maze from the file.
  fileName = "maze2.txt";
  cout << "NOW OPENING: " << fileName << endl;

   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }

   try
   {
      graph g2;
      while (fin && fin.peek() != 'Z')
      {
         maze m2(fin);
		 m2.mapMazeToGraph(g2);
		 m2.print(0,0,1,0);
         g2.clearVisit();
         g2.clearMark();
         cout << "Recursive Path Finder\n";
         cout << "*-------------------------------------------*\n";
		 m2.findPathRecursive(g2, 0);
         g2.clearVisit();
         m2.print(g2, 0);
         g2.clearMark();
         g2.clearVisit();
         cout << "\n\nNon-Recursive Path Finder (Depth First Search)\n";
         cout << "*-------------------------------------------*\n";
         m2.findPathNonRecursive(g2);
         m2.printPathOnMaze();
         g2.clearVisit();
         m2.print(g2, 0);


      }

   }
   catch (indexRangeError &ex)
   {
      cout << ex.what() << endl; exit(1);
   }
   catch (rangeError &ex)
   {
      cout << ex.what() << endl; exit(1);
   }
   fin.close();

   // Read the maze from the file.
  fileName = "maze3.txt";
  cout << "NOW OPENING: " << fileName << endl;

   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }

   try
   {
      graph g;
      while (fin && fin.peek() != 'Z')
      {
         maze m(fin);
		 m.mapMazeToGraph(g);
		 m.print(0,0,1,0);
         g.clearVisit();
         g.clearMark();
         cout << "Recursive Path Finder\n";
         cout << "*-------------------------------------------*\n";
		 m.findPathRecursive(g, 0);
         g.clearVisit();
         m.print(g, 0);
         g.clearMark();
         g.clearVisit();
         cout << "\n\nNon-Recursive Path Finder (Depth First Search)\n";
         cout << "*-------------------------------------------*\n";
         m.findPathNonRecursive(g);
         m.printPathOnMaze();
         g.clearVisit();
         m.print(g, 0);


      }

   }
   catch (indexRangeError &ex)
   {
      cout << ex.what() << endl; exit(1);
   }
   catch (rangeError &ex)
   {
      cout << ex.what() << endl; exit(1);
   }

*/

}
