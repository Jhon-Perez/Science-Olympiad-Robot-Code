#include<opencv2/opencv.hpp>
#include<tuple>

#pragma once

using namespace std;
using namespace cv;

enum coordinateType{
    raw = 0, 
    euler = 1, 
    maze = 2
};

class Coordinates{
    private : 
        int x, y;
        int dimensions;
    public : 
        
        // Setter Methods
        void updateCoordinates(int tempX, int tempY, coordinateType type){
            if(type == coordinateType::raw){
                // converting to raw
                x = dimensions/8 + dimensions/4 * tempX;
                y = dimensions/8 + dimensions/4 * tempY;
                // converting to euler
                x = x - dimensions/2;
                y = dimensions/2 - y;
            }else if(type == coordinateType::maze){
                x = tempX - dimensions/2;
                y = dimensions/2 - tempY;
            }else{
                x = tempX;
                y = tempY;
            }
        }
        void convertFromRaw(int tempX, int tempY){
            x = tempX - dimensions/2;
            y = dimensions/2 - tempY;
        }
        // Getter Methods
        int getX(coordinateType type = coordinateType::raw){
            if(type == coordinateType::maze){
                int xRaw = x + dimensions/2;
                return (xRaw - dimensions/8) * 4/dimensions;
            }else if(type == coordinateType::raw){
                return x + dimensions/2;
            }else{
                return x;
            }
        }
        int getY(coordinateType type = coordinateType::raw){
            if(type == coordinateType::maze){
                int yRaw = dimensions/2 - y;
                return (yRaw - dimensions/8) * 4/dimensions;
            }else if(type == coordinateType::raw){
                return dimensions/2 - y;
            }else{
                return y;
            }
        }
        tuple<int, int> getXAndY(coordinateType type = coordinateType::raw){
            return make_tuple(getX(type), getY(type));
        }
        // Other Methods
        Coordinates addX(int deltaX){
            return Coordinates(x + deltaX, y);
        }
        Coordinates addY(int deltaY){
            return Coordinates(x, y + deltaY);
        }
        Coordinates addXandY(int deltaX, int deltaY){
            return Coordinates(x + deltaX, y + deltaY);
        }
        Coordinates(int tempX, int tempY, coordinateType type = coordinateType::euler){
            if(type == coordinateType::raw){
                convertFromRaw(tempX, tempY) ;
            }else if(type == coordinateType::maze){

            }else{
                x = tempX;
                y = tempY;   
            }
        }
};


class Graphics{
    public : 
        void drawLine(Mat frame, Coordinates p1, Coordinates p2, Scalar color, int thickness){
            line(frame, Point(p1.getX(), p1.getY()), Point(p2.getX(), p2.getY()), color, thickness);
        }
        Mat drawCircle(Mat frame, Coordinates center, Scalar color, int radius = 2, int thickness = -1){
            circle(frame, Point(center.getX(), center.getY()), radius, color, thickness);
            return frame;
        }
        void drawRectangle(Mat frame, Coordinates center, int width, int height, Scalar color, int thickness = -1){
            Coordinates p1 = center.addXandY(width/2, height/2);
            Coordinates p2 = center.addXandY(-1 * width/2, -1 * height/2);
            rectangle(frame, Point(p1.getX(), p1.getY()), Point(p2.getX(), p2.getY()), color, thickness);
        }
};