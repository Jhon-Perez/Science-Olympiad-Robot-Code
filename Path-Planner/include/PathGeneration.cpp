#include<vector>
#include<queue>
#include"Maze.cpp"
#include<unordered_map>

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
        static void runHeuristic(){

        }
        static void optimizeLinearSpeed(){
            
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
                    if( guide.matrix[loc.getY(coordinateType::maze)][loc.getX(coordinateType::maze)].isGate ) gates++;
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
};

class PathGeneration{
    private:
        queue<Block> frontier;
        vector<Block> explored;
        unordered_map<int, int> startingCounter;
        vector<int> keys;
        Coordinates target;
        Maze outline;
    public:
        vector<Block> finalNodes;
        PathGeneration(vector<Coordinates> s, Coordinates t, Maze o){
            reset(s, t);
            outline = o;
        }
        PathGeneration(Maze o){

            outline = o;
        }
        void newStart(vector<Coordinates> s){
            reset(s, target);    
        }
        void reset(vector<Coordinates> s, Coordinates t){
            finalNodes.clear();
            while(frontier.empty() == false) frontier.pop();
            explored.clear();
            keys.clear();
            for(Coordinates startCoordinates : s){
                startCoordinates.print();
                frontier.push(Block(startCoordinates));
                startingCounter[frontier.size() - 1] = 0;
                keys.push_back(frontier.size() - 1);
            }
            target = t;
        }
        void reset(){
            finalNodes.clear();
            while(frontier.empty() == false) frontier.pop();
            explored.clear();
        }
        void BFS(const Maze &curM){
            // updating frontier
            if(frontier.size() == 0) {
                cout << "FINISH : Insufficent Frontier!" << endl;
                return;
            }
            Block currentBlock = frontier.front();
            frontier.pop();
            explored.push_back(currentBlock);
            // Defining the base case
            if(currentBlock.currentLocation.x == target.x && currentBlock.currentLocation.y == target.y && startingCounter[currentBlock.startIndex] < 5){
                finalNodes.push_back(currentBlock);
            }
            // Checking for instances
            bool pathsFound = true;
            for(int key : keys){
                if(startingCounter[key] < 5){
                    pathsFound = false;
                    break;
                }
            }
            if(pathsFound) return;
            // Pushing new nodes to frontier
            vector<Direction> movesList =  outline.getPossibleMoves(currentBlock.currentLocation);
            for(Direction move : movesList){
                Coordinates transposedLocation = outline.transpose((currentBlock.currentLocation), move);
                // checking explored
                bool flag = true;
                for(int i = 0; i < explored.size(); i++){
                    Block ref = explored[i];
                    if(ref.currentLocation.x == transposedLocation.x && ref.currentLocation.y == transposedLocation.y){
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    Block transposedNode;
                    if(currentBlock.startIndex == -1) transposedNode = Block(transposedLocation, explored.size() - 1, explored.size() - 1);
                    else transposedNode = Block(transposedLocation, explored.size() - 1);
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
                        Block transposedNode;
                        if(currentBlock.startIndex == -1) transposedNode = Block(transposedLocation, explored.size() - 1, explored.size() - 1);
                        else transposedNode = Block(transposedLocation, explored.size() - 1);
                        frontier.push(transposedNode);
                    }
                }
            }
            return BFS(curM);
        }
        Path generatePath(const Block &node, const Maze &curMaze){
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

class subPathId{
    public : 
        int start;
        int end;
        Coordinates startCoordinate;
        Coordinates endCoordinate;
        vector<Path> paths;
        subPathId(){
            start = -1;
            end = -1;
        }
        subPathId(int s, int e, Coordinates sc, Coordinates ec){
            start = s;
            end = e;
            startCoordinate = sc;
            endCoordinate = ec;
        }
        void addPath(Path tempPath){
            if(tempPath.locationList.size() != 0 && tempPath.locationList[0].equals(startCoordinate) && tempPath.locationList[tempPath.locationList.size() - 1].equals(endCoordinate)){
                paths.push_back(tempPath);
            }
        }
        void print(){
            if(start > -1){
                cout << "G" << start << "->";
            }else if(start == -1){
                cout << "S->";
            }
            if(end > -1){
                cout << "G" << end << endl;
            }else if(end == -1){
                cout << "F" << endl;
            }
        }
        bool operator == (const subPathId &c){ 
            return start == c.start && end == c.end;
        } 
        bool equals(subPathId other){
            return other.start == start && other.end == end;
        }
        bool equals(int s, int e){
            return s == start && end == e;
        }
};