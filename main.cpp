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
      void printPath(graph &g, int id);

   private:
      int rows; // number of rows in the maze
      int cols; // number of columns in the maze

      matrix<bool> value;
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

void maze::printPath(graph &g, int id) {
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
                        printPath(g, dst);
                        return;
                    } else if(dir == cols) {
                        cout << "Go down\n";
                        printPath(g, dst);
                        return;
                    } else if(dir == 1) {
                        cout << "Go right \n";
                        printPath(g, dst);
                        return;
                    } else if(dir == -1){
                        cout << "go left\n";
                        printPath(g, dst);
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

/*
bool maze::findPathNonRecursive(graph &g){
    int pathLength = 0;
    bool mazeExitFound = false;
    int dst;
    int dir;
    int id = 0;
	while (!mazeExitFound){
        if(id == g.numNodes() - 1) {
    		cout << "End of path :)\n";
            mazeExitFound = true;
        } else {
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
		pathLength++;
    }
}
*/

/*
void maze::findPathNonRecursive(graph &g)
{
    int id = 0;
    int dst;
    // Mark all the vertices as not visited
    bool *visited = new bool[(cols*rows)];
    for(int i = 0; i < (cols*rows); i++)
        visited[i] = false;

    // Create a queue for BFS
    list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[id] = true;
    queue.push_back(id);

    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        id = queue.front();
        cout << id << " ";
        queue.pop_front();

        // Get all adjacent vertices of the dequeued vertex id
        // If a adjacent has not been visited, then mark it visited
        // and enqueue it

        for(int j = 0; j < 4; j++)
        //checks the four directions
        {
            dst = setDirection(j, id, cols);
            if(!(dst < 0 | dst >= g.numNodes())) {
                if(!g.isVisited(dst)) {
                    g.visit(id);
                    if(g.isEdge(id, dst)) {
                        if(!visited[*i])
                        {
                            visited[*i] = true;
                            queue.push_back(*i);
                        }
                        if(id == g.numNodes() - 1) {
                            g.mark(id);
                    		cout << "\nFound the end";
                        }
                    }
                }
            }
        }
    }
}
*/
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

void maze::findPathNonRecursive(graph &g)
{
    int maxDepth = 1;
    int destinationNode = 0;
    int id = 0;
    bool pathValid;
    std::stack<int> pathStack;
    std::stack<int> visitingStack;
    std::stack<int> popNumbers;
    int depthSinceBranch = 1;
    visitingStack.push(id);
    while (id != (g.numNodes() - 1)){
        id = 0;
        visitingStack.push(id);
        while (!visitingStack.empty()){
            cout << "\n\n---------------NEW NODE---------------\n";
            id = visitingStack.top();
            pathValid = false;
            cout << "\nCurrent node visiting: " << id;
            pathStack.push(id);
            cout << "\n\npathStack thus far: ";
            printStack(pathStack);
            cout << "\nvisitingStack thus far: ";
            printStack(visitingStack);
            visitingStack.pop();
            for(int i = 0; i < 4; i++)
            //checks the four directions
            {
                cout << "\n----Iteration: " << i << "----";
                destinationNode = setDirection(i, id, cols);
                if(destinationNode > 0 && destinationNode <= g.numNodes()) {
                    cout << "\nDestination is within bounds";
                    if(!g.isVisited(destinationNode))
                    {
                        cout << "\nThe " << destinationNode << " node has not been visited";
                        g.visit(id);
                        if(g.isEdge(id, destinationNode))
                        {
                            cout << "\nThere is an edge";
                            if (pathStack.size() <= maxDepth){
                                cout << "\nPushed node: " << destinationNode;
                                visitingStack.push(destinationNode);
                                if (pathValid){
                                    popNumbers.push(depthSinceBranch);
                                } else {
                                    depthSinceBranch++;
                                }
                                pathValid = true;
                            } else if (i == 3){
                                cout << "\nThe maxDepth has been reached";
                            } else {
                                cout << "\nThe maxDepth has been reached";
                            }
                        } else if (i == 3){
                            cout << "\nThere is no edge to this node";
                        } else {
                            cout << "\nThere is no edge to this node";
                        }
                    } else if (i == 3){
                        cout << "\nThe node has been visited";
                    } else {
                        cout << "\nThe node has been visited";
                    }
                } else {
                    cout << "\nDestination Out of Bounds";
                }
            }
            if (!pathValid){
                int pops = popNumbers.size();
                for (int x = 0; x < pops ; x++ ){
                    cout << "\n-- PATHSTACK POP --";
                    popNumbers.pop();
                }
                popNumbers.pop();
            }
        }
        maxDepth++;
        cout << "\n\n================= NEW MAX DEPTH =================n";
        cout << "\n\nThe maxDepth is " << maxDepth << "\n\n";
        while (!pathStack.empty()){
            pathStack.pop();
        }
        while (!visitingStack.empty()){
            visitingStack.pop();
        }
        g.clearVisit();
    }
}


int main()
{
   char x;
   ifstream fin;

   // Read the maze from the file.
   string fileName = "maze3.txt";

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
		 cout << g;
		 m.print(0,0,2,2);
         g.clearVisit();
		 m.findPathNonRecursive(g);
         g.clearVisit();
         m.printPath(g, 0);

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


}
