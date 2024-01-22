#include<iostream>
#include<opencv2/opencv.hpp>
#include"Graphics.cpp"
#include<vector>

using namespace cv;
using namespace std;

/*
    Maze Class Initialization : 
    Gate ID's : 
    +----+----+----+----+
    | 00 | 01 | 02 | 03 |
    +----+----+----+----+
    | 04 | 05 | 06 | 07 |
    +----+----+----+----+
    | 08 | 09 | 10 | 11 |
    +----+----+----+----+
    | 12 | 13 | 14 | 15 |
    +----+----+----+----+
*/

enum Direction{
    top = 0, 
    bottom = 1, 
    left = 2, 
    right = 3
};

class MazeBlockQuery{
    public : 
        int blockId;
        vector<Direction> walls;
        bool isGate = false;
        bool start = false;
        bool end = false;
        MazeBlockQuery(int id, bool gate, const vector<Direction> &wal){
            blockId = id;
            gate = gate;
            walls = wal;
        }
};

class MazeBlock{
    private: 
        // Wall Info
        bool wallTop;
        bool wallBottom;
        bool wallLeft;
        bool wallRight;
        // Other Info
        bool gateHere;
        bool start;
        bool end;
    public :
        MazeBlock(bool top, bool bottom, bool left, bool right, bool gate, bool d, bool s, bool e){
            wallTop = top;
            wallBottom = bottom;
            wallLeft = left;
            wallRight = right;
            gateHere = gate;
            start = s;
            end = e;
        }
        MazeBlock(){
            wallTop = false;
            wallBottom = false;
            wallLeft = false;
            wallRight = false;
            gateHere = false;
            start = false;
            end = false;
        }
        void modifyWalls(Direction side){
            if(side == top) wallTop = true;
            else if(side == bottom) wallBottom = true;
            else if(side == Direction::right) wallRight = true;
            else if(side == Direction::left) wallLeft = true;
        }
        void setGateValue(bool gateVal){
            gateHere = gateVal;
        }
};

class Maze{
    public : 
        MazeBlock matrix[4][4];
        Coordinates start = Coordinates(0, 0);
        Coordinates end = Coordinates(0, 0);
        void updateBlock(MazeBlockQuery query){
            // updating walls
            int y = query.blockId/4;
            int x = query.blockId%4;
            for(Direction wall : query.walls){
                matrix[y][x].modifyWalls(wall);
                switch(wall){
                    case Direction::left: 
                        if(x!=0) matrix[y][x - 1].modifyWalls(Direction::right);
                        break;
                    case Direction::right :
                        if(x!=3) matrix[y][x + 1].modifyWalls(Direction::left);
                        break;
                    case Direction::bottom :
                        if(y!=0) matrix[y - 1][x].modifyWalls(Direction::top);
                        break;
                    case Direction::top :
                        if(y!=3) matrix[y + 1][x].modifyWalls(Direction::bottom);
                        break;
                }
            }
            // updating gate status
            matrix[y][x].setGateValue(query.isGate);
            // updating start/finish status
            if(query.end){
                end.updateCoordinates(x, y, coordinateType::maze);
            }else if(query.start){
                start.updateCoordinates(x, y, coordinateType::maze);
            }
        }
        Maze(vector<MazeBlockQuery> &modifications){
            for(int i = 0; i < 4; i++)
                for(int j = 0; j < 4; j++)
                    matrix[i][j] = MazeBlock();
            for(MazeBlockQuery mod : modifications){
            
                updateBlock(mod);
            }
        }
};