#include<iostream>
#include<opencv2/opencv.hpp>
#include"include/Graphics.cpp"
#include"include/Maze.cpp"
#include<tuple>
#include<vector>

using namespace std;
using namespace cv;

double dimensions = 400;

int main(){
    Mat frame = Mat::zeros(dimensions, dimensions, CV_8UC3);
    int x = 0;
    int y = 0;

    Coordinates drawCoord = Coordinates(x, y);
    Graphics drawer = Graphics();

    drawer.drawCircle(frame, drawCoord, Scalar(0, 255, 0));
    
    imshow("test frame", frame);

    int key = (waitKey(0) & 0xFF);
    if (key == 'q'){
        destroyAllWindows();
    }
    return 1;
}