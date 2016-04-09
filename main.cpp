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

			cout << "\nThis is the " << map[i][j] << " time through the loop\n";

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
int hello = 0;

bool maze::findPathRecursive(graph &g, int id) {
    //check for path going up
	cout << "This is the " << hello << " recursion\n";
    if(id == g.numNodes() - 1) {
		cout << "\n" << id;
        return true;
    } else {
        for(int i = 0; i < 4; i++)
        //checks the four directions
        {
            g.visit(id, setDirection(i, id, cols));
            if(findPathRecursive(g, id)) {
                g.mark(id);
                return true;
            }
        }
        return false;
    }

}



//void maze::findPathNonRecursive(graph &g){
//	while (graphNotSolved){
//		push

//}

int main()
{
   char x;
   ifstream fin;

   // Read the maze from the file.
   string fileName = "maze1.txt";

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
		 m.findPathRecursive(g, 0);

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
