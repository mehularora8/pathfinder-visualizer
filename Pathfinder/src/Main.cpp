/*
 * Pathfinding visualizer algorithm
 *
 * Program to visualize working of BFS and DFS and how they explore nodes.
 *
 * Made using Stanford University's libraries.
 *
 * TODO: Implement Djikstra - Add option to make djikstra grid
 *      Implement vector to keep track of path in bfs/dfs.
*/

//Console and IO
#include <iostream>
#include "console.h"
#include "simpio.h"
//Collections for implementations of algorithms
#include "strlib.h"
#include "queue.h"
#include "vector.h"
#include "priorityqueue.h"
#include "set.h"
#include "grid.h"
//Misc
#include <chrono> // For window sleep
#include <thread> // during working of code

using namespace std;

static const Grid<char> DEFAULT_PATH = {{'#', '.', '.', '.', '.', '.', 'T', '#'},
                                        {'#', '.', '.', '.', '.', '.', '.', '#'},
                                        {'#', '.', '#', '#', '#', '.', '.', '#'},
                                        {'#', '.', '.', '.', '#', '.', '.', '#'},
                                        {'#', '.', '#', '.', '#', '.', '.', '#'},
                                        {'.', 'S', '.', '.', '#', '.', '.', '#'},
                                        {'.', '#', '#', '.', '#', '.', '.', '#'},
                                        {'.', '#', '#', '#', '#', '.', '#', '#'}};

void outputGrid(Grid<char> g){
    cout << endl;
    for(int i = 0; i < g.numRows(); i++){
        for(int j = 0; j < g.numCols(); j++){
            cout << g[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

Grid<char> makegrid(){
    //Output instructions
    cout << "Use # to denote a blocked cell, a dot character (.) to denote open "
         << "cell, T to denote the target and a S to denote start." << endl;
    int rows = getInteger("Enter nRows: ");
    int columns = getInteger("Enter nColumns: ");
    //Initialize grid
    Grid<char> g(rows, columns);
    for(int i = 0; i < rows; i++){
        string row = getLine("Enter row " + integerToString(i + 1) + ":");
        while(row.length() != columns){
            row = getLine("Invalid row length. Enter row " + integerToString(i + 1) + ":");
        }
        for(int j = 0; j < columns; j++){
            g[i][j] = row[j];
        }
    }
    return g;
}

Vector<int> searchGrid(const Grid<char> g, char s){
    //Return value
    Vector<int> foundNode;
    //N(numRow * numCol) search grid for character 'S'
    for(int i = 0; i < g.numRows(); i++){
        for(int j = 0; j < g.numCols(); j++){
            if(g[i][j] == s){
                foundNode.add(i);
                foundNode.add(j);
                return foundNode;
            }
        }
    }
    return {};
}

bool bfs(Grid<char>& g, Vector<int> start){
    Queue<Vector<int>> q;
    Set<Vector<int>> set;
    //Enqueue starting element
    q.enqueue(start);

    //Explore while queue isn't empty
    while(!q.isEmpty()){
        Vector<int> currPoint = q.dequeue();
        int i = currPoint[0];
        int j = currPoint[1];

        //Success condition
        if(g[i][j] == 'T'){
            return true;
        }
        //Failure condition
        if(set.contains(currPoint)){
            continue;
        }
        //Maintain path
        if(g[i][j] != 'S'){
            g[i][j] = 'x';
        }
        clearConsole();
        outputGrid(g);
        //If neighbors have open space and are inbounds, enqueue to explore
        for(int x = -1; x <= 1; x++){
            for(int y = -1; y <= 1; y++){
                //Both abs(x), abs(y) should not be > 0 since diagonal moves are not allowed.
                if(!(abs(x) == abs(y))){
                    if(g.inBounds(i + x, j + y) && g[i + x][j + y] != '#'){
                        Vector<int> newPoint;
                        newPoint.add(i + x);
                        newPoint.add(j + y);
                        q.enqueue(newPoint);
                    }
                }
            }
        }
        //Add visited point to seen set
        set.add(currPoint);
        //Sleep to see path taken.
        this_thread::sleep_for(chrono::milliseconds(400));
    }

    return false;
}

bool dfs(Grid<char>& g, Vector<int> start, Set<Vector<int>>& seen){

    this_thread::sleep_for(chrono::milliseconds(200));
    int i = start[0];
    int j = start[1];

    //Success condition
    if(g[i][j] == 'T'){
        return true;
    }

    //Failure condition
    if(seen.contains(start)){
        return false;
    }

    if(g[i][j] != 'S'){
        g[i][j] = 'x';
    }
    clearConsole();
    outputGrid(g);

    seen.add(start);
    //Check if all possible options are inbounds and are not walls
    //Backtracking recurse
    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            //Both abs(x), abs(y) should not be > 0 since diagonal moves are not allowed.
            if(!(abs(x) == abs(y))){
                if(g.inBounds(i + x, j + y) && g[i + x][j + y] != '#'){
                    Vector<int> newPoint;
                    newPoint.add(i + x);
                    newPoint.add(j + y);
                    if (dfs(g, newPoint, seen)){
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

int main() {
    cout << "Pathfinding Visualizer" << endl;

    while(1){
        cout << endl;
        cout << "1. Explore path using BFS." << endl;
        cout << "2. Explore path using DFS." << endl;
        cout << "4. Quit" << endl;
        int choice = getInteger("Enter choice: ");
        if (choice == 4){
            //Quit
            break;
        }
        Grid<char> path;

        int second_choice = getInteger("1. Use default grid, 2. Make your own grid");
        if(second_choice == 1){
            path = DEFAULT_PATH;
        } else if(second_choice == 2){
            path = makegrid();
        } else {
            cout << "Enter valid option.";
            continue;
        }

        Vector<int> start = searchGrid(path, 'S');
        if(start.size() != 2){
            cout << "Invalid grid. Must have one and only one starting point (S)" << endl;
            continue;
        }

        if (choice == 1){
            //bfs. Returns shortest path
            bool returnvalue = bfs(path, start);
            if(returnvalue){
                cout << "Solution was found. Crosses(x) indicates the nodes explored." << endl;
            } else {
                cout << "No solution found." << endl;
            }
        } else if (choice == 2){
            Set<Vector<int>> seen;

            bool returnvalue = dfs(path, start, seen);
            if(returnvalue){
                cout << "Solution was found. Crosses (x) indicate the nodes explored." << endl;
            } else {
                cout << "No solution found." << endl;
            }
            //dfs
        } else {
            cout << "Enter valid choice." << endl;
            continue;
        }
    }
    return 0;
}
