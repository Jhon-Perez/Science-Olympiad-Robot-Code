#include<iostream>
#include<opencv2/opencv.hpp>
#include"include/Graphics.cpp"
#include"include/PathGeneration.cpp"
#include"include/Maze.cpp"
#include<tuple>
#include<vector>

using namespace std;
using namespace cv;

double dimensions = 800;

int main(){

    clock_t start, end;

    // Starting Time Function
    start = clock();
    
    // Runing Search Algorithm
    Mat frame = Mat::zeros(dimensions, dimensions, CV_8UC3);
    vector<MazeBlockQuery> makeVector;
    
    makeVector.push_back(MazeBlockQuery(1, false, true));
    makeVector.push_back(MazeBlockQuery(13, true, false));
    makeVector.push_back(MazeBlockQuery(1, Maze::generateWallsList(false, true, false, true)));
    makeVector.push_back(MazeBlockQuery(2, true, Maze::generateWallsList(false, false, false, true)));
    makeVector.push_back(MazeBlockQuery(8, true, Maze::generateWallsList(true, false, false, false)));
    makeVector.push_back(MazeBlockQuery(9, Maze::generateWallsList(false, true, false, true)));
    makeVector.push_back(MazeBlockQuery(10, Maze::generateWallsList(true, false, false, false)));
    makeVector.push_back(MazeBlockQuery(11, true, Maze::generateWallsList(false, true, false, false)));
    Maze mainMaze = Maze(makeVector, dimensions);
    PathGeneration engine = PathGeneration(mainMaze.start, mainMaze.end, mainMaze);
    engine.BFS(mainMaze);
    vector<Path> layer1Path;
    for(Block node : engine.finalNodes){
        Path tempPath = engine.generatePath(node, mainMaze);
        layer1Path.push_back(tempPath);
    }
    frame = mainMaze.drawFrame(frame);
    for(int j = 0; j < layer1Path.size(); j++){
        for(int i = 1; i < layer1Path[j].locationList.size(); i++){
            Graphics::drawLine(frame, layer1Path[j].locationList[i], layer1Path[j].locationList[i -1], cv::Scalar(0, 255, 0), 10);    
        }    
    }

    // Calculating Run Time
    end = clock();
 
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken by program is : " << fixed 
         << time_taken << setprecision(5);
    cout << " sec " << endl;
    
    imshow("test frame", frame);

    int key = (waitKey(0) & 0xFF);
    if (key == 'q'){
        destroyAllWindows();
    }
    return 1;
}