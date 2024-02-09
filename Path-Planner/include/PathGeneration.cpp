#include<vector>
#include<queue>
#include"Maze.cpp"

using namespace std;

#pragma once
class Path{
    private:
        int smth;
        Maze guide;
        vector<Direction> movesList;
        int gates = 0;
    public: 
        vector<Coordinates> locationList;
        Path(Maze g){
            guide = g;
        }
        void addCoordinate(Coordinates addition){
            locationList.push_back(addition);
            if(addition.x == locationList[locationList.size() - 2].x){
                if(addition.y == locationList[locationList.size() - 2].y + 1){
                    movesList.push_back(Direction::bottom);
                }else{
                    movesList.push_back(Direction::top);
                }
            }else{
                if(addition.x == locationList[locationList.size() - 2].x + 1){
                    movesList.push_back(Direction::right);
                }else{
                    movesList.push_back(Direction::left);
                }
            }
            gatesPassed();
        }
        void reverse(){
            // reformatting the location list
            int total = locationList.size()/2;
            for(int i = 0; i < total; i++){
                Coordinates temp = locationList[i];
                locationList[i] = locationList[locationList.size() - i - 1];
                locationList[locationList.size() - i - 1] = temp;
            }
            // reformatting the moves list
            movesList.clear();
            for(int i = 1; i < locationList.size(); i++){
                Coordinates currentLocation = locationList[i];
                Coordinates previousLocation = locationList[i - 1];
                if(currentLocation.x == previousLocation.x){
                    if(currentLocation.y == previousLocation.y + 1){
                        movesList.push_back(Direction::bottom);
                    }else{
                        movesList.push_back(Direction::top);
                    }
                }else{
                    if(currentLocation.x == previousLocation.x + 1){
                        movesList.push_back(Direction::right);
                    }else{
                        movesList.push_back(Direction::left);
                    }
                }
            }
            gatesPassed();
        }
        void gatesPassed(){
            gates = 0;
            bool gateMatrix[4][4] = {
                {false, false, false, false},
                {false, false, false, false},
                {false, false, false, false},
                {false, false, false, false},
            };
            for(Coordinates loc : locationList){
                if(gateMatrix[loc.getY(coordinateType::maze)][loc.getX(coordinateType::maze)] == false){
                    gateMatrix[loc.getY(coordinateType::maze)][loc.getX(coordinateType::maze)] = true;
                    if( guide.matrix[loc.getY(coordinateType::maze)][loc.getX(coordinateType::maze)].gateHere ) gates++;
                }
            }
        }
        void Concatenate(Path concPath){
            for(Coordinates loc : concPath.locationList){
                addCoordinate(loc);
            }
        }
        void print(){
            cout << endl;
            cout << " Path : ";
            for(int i = 0; i < movesList.size(); i++){
                Direction move = movesList[i];
                if(move == Direction::top) cout << " Top" << "(" << locationList[i].x << "," << locationList[i].y << ")"<< ",";
                else if(move == Direction::bottom) cout << " Bottom" << "(" << locationList[i].x << "," << locationList[i].y << ")"<< ",";
                else if(move == Direction::left) cout << " Left" << "(" << locationList[i].x << "," << locationList[i].y << ")"<< ",";
                else if(move == Direction::right) cout << " Right" << "(" << locationList[i].x << "," << locationList[i].y << ")"<< ",";
            }
            cout << endl;
        }
        void heuristic(){
        }
        void optimizeLinearSpeed(){
        }
};

class PathGeneration{
    private:
        queue<Block> frontier;
        vector<Block> explored;
        Coordinates target;
        Maze outline;
        int iteration = 0;
    public:
        vector<Block> finalNodes;
        PathGeneration(Coordinates s, Coordinates t, Maze o){
            finalNodes.clear();
            while(frontier.empty() == false) frontier.pop();
            explored.clear();
            frontier.push(Block(s));
            target = t;
            outline = o;
        }
        void BFS(Maze curM, int level = 0){
            iteration++;
            // updating frontier
            if(frontier.size() == 0) return;
            Block currentBlock = frontier.front();
            frontier.pop();
            explored.push_back(currentBlock);
           /* cout << "Current Block " << currentBlock.currentLocation.getX(maze) << " " << currentBlock.currentLocation.getY(maze) << " Dimensions : " << currentBlock.currentLocation.dimensions << " Explored : ";
            for(Block n : explored){
                cout << "(" << n.currentLocation.getX(maze) << "," << n.currentLocation.getY(maze) << ") ";
            }*/
            // Defining the base case
            if(currentBlock.currentLocation.x == target.x && currentBlock.currentLocation.y == target.y){
                finalNodes.push_back(currentBlock);
            }
            if(finalNodes.size() == 5) return;
            // Pushing new nodes to frontier
            vector<Direction> movesList =  outline.getPossibleMoves(currentBlock.currentLocation);
            //cout << "Moves List : " << movesList[0] << endl;
            //cout << " Transposed : ";
            for(Direction move : movesList){
                Coordinates transposedLocation = outline.transpose((currentBlock.currentLocation), move);
                // checking explored
                bool flag = true;
                for(int i = 0; i < explored.size(); i++){
                    Block ref = explored[i];
                   // cout << "|(" << ref.currentLocation.x << "(" <<  ref.currentLocation.getX(maze) <<")" << "," << ref.currentLocation.y << "(" <<  ref.currentLocation.getY(maze) << ")" <<  ")==(" << transposedLocation.x << "(" <<  transposedLocation.getX(maze) <<")" << "," << transposedLocation.y << "(" <<  transposedLocation.getY(maze) <<")"<< ")|";
                    if(ref.currentLocation.x == transposedLocation.x && ref.currentLocation.y == transposedLocation.y){
                        flag = false;
                        break;
                    }
                }
                //cout << " " << move << " -> (" << transposedLocation.getX(maze) << "," << transposedLocation.getY(maze) << ") Passed : " << flag << " ";
                if(flag){
                    Block transposedNode = Block(transposedLocation, explored.size() - 1);
                    frontier.push(transposedNode);
                }else{
                    flag = true;
                    Path checkPath =  generatePath(currentBlock, curM);
                    for(Coordinates point : checkPath.locationList){
                        if(point.x == transposedLocation.x && point.y == transposedLocation.y){
                            flag = false;
                            break;
                        }
                    }
                    if(flag){
                        Block transposedNode = Block(transposedLocation, explored.size() - 1);
                        frontier.push(transposedNode);
                    }
                }
            }
            /*cout << " frontier : ";
            queue<Block> copyQ = frontier;
            cout << endl;
            while(copyQ.empty() == false){
                cout << "(" << copyQ.front().currentLocation.getX(maze) << "," << copyQ.front().currentLocation.getY(maze)<< "), ";
                copyQ.pop();
            }
            cout << endl;
            cout << endl;*/
            return BFS(curM, level + 1);
        }
        Path generatePath(Block node, Maze curMaze, int level = 0){
            Block curNode = node;
            Path ref = Path(curMaze);
            ref.addCoordinate(curNode.currentLocation);
            while(curNode.previousBlockIndex != -1){
                curNode = explored[curNode.previousBlockIndex];
                ref.addCoordinate(curNode.currentLocation);    
            }
            ref.reverse();
            return ref;
        }
};