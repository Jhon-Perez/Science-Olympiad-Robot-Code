#include<iostream>
#include<opencv2/opencv.hpp>
#include"include/Graphics.cpp"
#include"include/PathGeneration.cpp"
#include"include/Maze.cpp"
#include<tuple>
#include<vector>
#include<unordered_map>

using namespace std;
using namespace cv;

double dimensions = 800;

void getBestPath(Maze mainMaze){
    //! Initializing Image and timing
    clock_t start, end;
    start = clock();
    Mat frame = Mat::zeros(dimensions, dimensions, CV_8UC3);
    PathGeneration engine = PathGeneration(mainMaze);

    //! Declaring all Algo Sub-Paths
    vector<subPathId> subPaths;
    for(int start = 0; start <= mainMaze.gates.size(); start++){
        for(int end = 0; end <= mainMaze.gates.size(); end++)
            if(end != start || start + end == 0){
                Coordinates startCoordinate;
                Coordinates endCoordinate;
                if(start == 0){
                    startCoordinate = mainMaze.start;
                }else{
                    startCoordinate = mainMaze.gates[start - 1];
                }

                if(end == 0){
                    endCoordinate = mainMaze.end;
                }else{
                    endCoordinate = mainMaze.gates[end - 1];
                }

                subPaths.push_back(subPathId(start - 1, end - 1, startCoordinate, endCoordinate));
            }
    }
    for(subPathId id : subPaths) id.print();

    //! Defining all Algo Sub-Paths
    //? gate 2 gate
    vector<Coordinates> startingPoints;
    for(int target = 0; target < mainMaze.gates.size(); target++){
        startingPoints.clear();
        startingPoints.push_back(mainMaze.start);
        for(int startingPoint = 0; startingPoint < mainMaze.gates.size(); startingPoint++){
            if(startingPoint != target){
                startingPoints.push_back(mainMaze.gates[startingPoint]);
            }
        }
        
        // Run BFS algorithm
        engine.reset(startingPoints, mainMaze.gates[target]);
        engine.BFS(mainMaze);
        // Update Respective Paths
        engine.finalNodes[0];
        cout << "started parsing" << endl;
        for(int i = 0; i < engine.finalNodes.size(); i++){
            Path tempPath = engine.generatePath(engine.finalNodes[i], mainMaze);
            for(int j = 0; j < subPaths.size(); j++){
                subPaths[j].addPath(tempPath);
            }
        }
    }
    //? Gate 2 Finish
    startingPoints.clear();
    startingPoints.push_back(mainMaze.start);
    for(int startingPoint = 0; startingPoint < mainMaze.gates.size(); startingPoint++){
        startingPoints.push_back(mainMaze.gates[startingPoint]);
    }
    // Run BFS algorithm
    engine.reset(startingPoints, mainMaze.end);
    engine.BFS(mainMaze);
    // Update Respective Paths
    engine.finalNodes[0];
    cout << "started parsing" << endl;
    for(int i = 0; i < engine.finalNodes.size(); i++){
        Path tempPath = engine.generatePath(engine.finalNodes[i], mainMaze);
        for(int j = 0; j < subPaths.size(); j++){
            subPaths[j].addPath(tempPath);
        }
    }
    //! Combining Paths
    //! Displaying Output
    end = clock();
    frame = mainMaze.drawFrame(frame);

    for(subPathId subPath : subPaths){
        for(Path p : subPath.paths){
            for(int i = 1; i < p.locationList.size(); i++){
                Graphics::drawLine(frame, p.locationList[i], p.locationList[i -1], cv::Scalar(0, 255, 0), 10);    
                imshow("test frame", frame);
                int key = (waitKey(0) & 0xFF);
            } 
            frame = mainMaze.drawFrame(frame);
        }
    }
    
 
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken by program is : " << fixed 
         << time_taken << setprecision(5);
    cout << " sec " << endl;
}

int main(){
    //! Declaring Maze and Other Variables
    Maze mainMaze = Maze(dimensions);
    
    //? Setting Checkpoints
    mainMaze.addStart(1, 3);
    mainMaze.addEnd(1, 0);

    //? Add Gates
    mainMaze.addGate(2, 0);
    mainMaze.addGate(0, 2);
    mainMaze.addGate(3, 2);

    //? Add Walls
    mainMaze.addWalls(1, 0, false, true, true, false);
    mainMaze.addWalls(2, 0, false, true, false, true);
    mainMaze.addWalls(0, 2, true, false, false, false);
    mainMaze.addWalls(1, 2, false, true, true, false);
    mainMaze.addWalls(2, 2, true, false, false, true);
    mainMaze.addWalls(3, 2, false, false, true, false);

    //? Generate Path
    getBestPath(mainMaze);    
    return 1;
}