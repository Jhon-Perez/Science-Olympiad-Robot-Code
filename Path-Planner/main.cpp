#include<iostream>
#include<opencv2/opencv.hpp>
#include"include/Graphics.cpp"
#include"include/Maze.cpp"
#include<tuple>
#include<vector>

using namespace std;
using namespace cv;

double dimensions = 800;

int main(){
    Mat frame = Mat::zeros(dimensions, dimensions, CV_8UC3);
    vector<MazeBlockQuery> makeVector;
    
    makeVector.push_back(MazeBlockQuery(1, Maze::generateWallsList(false, true, false, true)));
    makeVector.push_back(MazeBlockQuery(2, true, Maze::generateWallsList(false, false, false, true)));
    makeVector.push_back(MazeBlockQuery(8, true, Maze::generateWallsList(true, false, false, false)));
    makeVector.push_back(MazeBlockQuery(9, Maze::generateWallsList(false, true, false, true)));
    makeVector.push_back(MazeBlockQuery(10, Maze::generateWallsList(true, false, false, false)));
    makeVector.push_back(MazeBlockQuery(11, true, Maze::generateWallsList(false, true, false, false)));
    
    Maze mainMaze = Maze(makeVector);
    
    
    frame = mainMaze.drawFrame(frame);
    
    imshow("test frame", frame);

    int key = (waitKey(0) & 0xFF);
    if (key == 'q'){
        destroyAllWindows();
    }
    return 1;
}