#include<iostream>
#include<opencv2/opencv.hpp>
#include"Graphics.cpp"
#include<vector>

using namespace cv;
using namespace std;

#pragma once

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
            isGate = gate;
            walls = wal;
        }
        MazeBlockQuery(int id, bool gate){
            blockId = id;
            isGate = gate;
        }
        MazeBlockQuery(int id, bool s, bool e){
            blockId = id;
            start = s;
            end = e;
        }
        MazeBlockQuery(int id, const vector<Direction> &wal){
            blockId = id;
            walls = wal;
        }
};

class MazeBlock{
    public :
        // Wall Info
        bool wallTop;
        bool wallBottom;
        bool wallLeft;
        bool wallRight;
        // Other Info
        bool gateHere;
        bool start;
        bool end;
    
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

class Block{
    public : 
        Coordinates currentLocation;
        int previousBlockIndex;
        int distance;
        // Constructors
        Block(Coordinates cl){
            currentLocation = cl;
            previousBlockIndex = -1;
            distance = 1;
        }
        Block(Coordinates cl, int pl, int d){
            currentLocation = cl;
            previousBlockIndex = pl;
            distance = d;
        }
        Block(Coordinates cl, int pl){
            currentLocation = cl;
            previousBlockIndex = pl;
            distance = 1;
        }
        Block(){
            previousBlockIndex = -1;
            distance = 1;
        }
};

class Maze{
    public : 
        MazeBlock matrix[4][4];
        Coordinates start = Coordinates(0, 0);
        Coordinates end = Coordinates(0, 0);
        void updateBlock(MazeBlockQuery query, int dimensions){
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
                        if(y!=3) matrix[y + 1][x].modifyWalls(Direction::top);
                        break;
                    case Direction::top :
                        if(y!=0) matrix[y - 1][x].modifyWalls(Direction::bottom);
                        break;
                }
            }
            // updating gate status
            matrix[y][x].setGateValue(query.isGate);
            // updating start/finish status
            if(query.end){                
                end.updateCoordinates(x, y, coordinateType::maze, dimensions);
            }else if(query.start){
                start.updateCoordinates(x, y, coordinateType::maze, dimensions);
            }
        }
        Maze(){}
        Maze(vector<MazeBlockQuery> &modifications, int dimensions){
            for(int i = 0; i < 4; i++)
                for(int j = 0; j < 4; j++){
                    matrix[i][j] = MazeBlock();
                    if(i == 0){
                        //cout << "wall top : " << j << "," << i << endl;
                        matrix[i][j].wallTop = true;
                    }else if(i == 3){
                        //cout << "wall bottom : " << j << "," << i << endl;
                        matrix[i][j].wallBottom = true;
                    }
                    if(j == 0){
                        //cout << "wall left : " << j << "," << i << endl;
                        matrix[i][j].wallLeft = true;
                    }else if(j == 3){
                        //cout << "wall right : " << j << "," << i << endl;
                        matrix[i][j].wallRight = true;
                    }
                }
            for(MazeBlockQuery mod : modifications){
                updateBlock(mod, dimensions);
            }
        }
        Mat drawFrame(Mat initialFrame){
            Mat frame = initialFrame.clone();
            Graphics g = Graphics();
            int dim = frame.size().height;
            g.drawRectangle(frame, Coordinates(0, 0, euler, dim), dim, dim, cv::Scalar(255, 255, 255), -1 );
            // Drawing the general grid
            for(int i = 0; i < 4; i++)
                for(int j = 0; j < 4; j++){
                    Coordinates center = Coordinates(i, j, coordinateType::maze, dim);
                    g.drawRectangle(frame, center, dim/4, dim/4, cv::Scalar(207, 194, 186), 15);
                    //g.drawCircle(frame, center, Scalar(255, 0, 0), 5, -1);
                }
            // Drawing the gates
            for(int i = 0; i < 4; i++)
                for(int j = 0; j < 4; j++){
                    Coordinates center = Coordinates(i, j, coordinateType::maze, dim);
                    if(matrix[j][i].gateHere) g.drawRectangle(frame, center, dim/4, dim/4, cv::Scalar(255, 0, 0), 15);
                }
            // Drawing the walls
            for(int x = 0; x < 4; x++)
                for(int y = 0; y < 4; y++){
                    Coordinates center = Coordinates(x, y, coordinateType::maze, dim);
                    Coordinates topLeft = center.addXandY(-1 * dim/8, 1 * dim/8);
                    Coordinates topRight = center.addXandY(1 * dim/8, 1 * dim/8);
                    Coordinates bottomLeft = center.addXandY(-1 * dim/8, -1 * dim/8);
                    Coordinates bottomRight = center.addXandY(1 * dim/8, -1 * dim/8);
                    
                    if(matrix[y][x].wallBottom){
                        g.drawLine(frame, bottomLeft, bottomRight, cv::Scalar(0, 75, 150), 15);
                    }
                    if(matrix[y][x].wallRight){
                        g.drawLine(frame, bottomRight, topRight, cv::Scalar(0, 75, 150), 15);
                    }
                    if(matrix[y][x].wallLeft){
                        g.drawLine(frame, bottomLeft, bottomLeft, cv::Scalar(0, 75, 150), 15);
                    }
                    if(matrix[y][x].wallTop){
                        g.drawLine(frame, topLeft, topRight, cv::Scalar(0, 75, 150), 15);
                    }
                }
            // Drawing Outer Borders
            g.drawRectangle(frame, Coordinates(0, 0, euler, dim), dim, dim, cv::Scalar(207, 194, 186), 25 );
            // Drawing Start and Finish
            g.drawCircle(frame, start, Scalar(0, 200, 0), 20, -1);
            g.drawCircle(frame, end, Scalar(0, 0, 255), 20, -1);
            return frame;
        }
        Mat drawNode(Mat initialFrame, Block path, vector<Block> explored){
            Mat frame = initialFrame.clone();
            Graphics::drawLine(frame, path.currentLocation, (explored[path.previousBlockIndex].currentLocation), cv::Scalar(0, 255, 0), 15);
            return frame;
        }
        static vector<Direction> generateWallsList(bool top, bool bottom, bool left, bool right){
            vector<Direction> answerVector;
            if(right) answerVector.push_back(Direction::right);
            if(left) answerVector.push_back(Direction::left);
            if(top) answerVector.push_back(Direction::top);
            if(bottom) answerVector.push_back(Direction::bottom);
            return answerVector;
        }
        // Search Funtions Methods
        vector<Direction> getPossibleMoves(Coordinates loc){
            vector<Direction> movesList;
            MazeBlock currentBlock = matrix[loc.getY(coordinateType::maze)][loc.getX(coordinateType::maze)];
            if(currentBlock.wallTop == false) movesList.push_back(Direction::top);
            if(currentBlock.wallBottom == false) movesList.push_back(Direction::bottom);
            if(currentBlock.wallRight == false) movesList.push_back(Direction::right);
            if(currentBlock.wallLeft == false) movesList.push_back(Direction::left);
            return movesList;
        }
        Coordinates transpose(Coordinates loc, Direction moveDirection){
            if(moveDirection == Direction::top) return Coordinates(loc.getX(coordinateType::maze), loc.getY(coordinateType::maze) - 1, coordinateType::maze, loc.dimensions);
            if(moveDirection == Direction::bottom){
                Coordinates answer = Coordinates(loc.getX(coordinateType::maze), loc.getY(coordinateType::maze) + 1, coordinateType::maze, loc.dimensions);
                return answer;
            }
            if(moveDirection == Direction::left) return Coordinates(loc.getX(coordinateType::maze) - 1, loc.getY(coordinateType::maze), coordinateType::maze, loc.dimensions);
            return Coordinates(loc.getX(coordinateType::maze) + 1, loc.getY(coordinateType::maze), coordinateType::maze, loc.dimensions);
        }   
};