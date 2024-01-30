#include<vector>
#include<queue>
#include"Maze.cpp"

using namespace std;

#pragma once
class Path{
    private:
        int smth;
        Maze guide;
        vector<Coordinates> locationList;
        vector<Direction> movesList;
        int gates = 0;
    public: 
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
        void heuristic(){
        }
        void optimizeLinearSpeed(){
        }
};

class PathGeneration{
    private:
        queue<Block> frontier;
        vector<Block*> explored;
        Coordinates target;
        Maze outline;
    public:
        vector<Block> finalNodes;
        PathGeneration(Coordinates s, Coordinates t, Maze o){
            finalNodes.clear();
            while(frontier.empty() == false) frontier.pop();
            explored.clear();
            frontier.push(Block(&s));
            target = t;
            outline = o;
        }
        void BFS(){
            cout << "working " << finalNodes.size() << endl;
            if(frontier.size() == 0) return;
            // updating frontier
            Block currentBlock = frontier.back();
            frontier.pop();
            explored.push_back(&currentBlock);
            // Defining the base case
            if(((*currentBlock.currentLocation) == target && finalNodes.size() == 5)){
                return;
            }
            // Pushing new nodes to frontier
            vector<Direction> movesList =  outline.getPossibleMoves(*currentBlock.currentLocation);
            for(Direction move : movesList){
                Coordinates transposedLocation = outline.transpose(*currentBlock.currentLocation, move);\
                // checking explored
                bool flag = true;
                for(Block* ref : explored){
                    if(*((*ref).currentLocation) == transposedLocation){
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    Block transposedNode = Block(&transposedLocation, &currentBlock);
                    frontier.push(transposedNode);
                }
            }
            return BFS();
        }
        void generatePath(Block node, Path ref, int level = 0){
            ref.addCoordinate(*node.currentLocation);
            if(node.previousBlock == NULL) return;
            generatePath(*node.previousBlock, ref, level + 1);
            if(level == 0) ref.reverse();
            return;
        }
};