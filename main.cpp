#include <GLUT/GLUT.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "head/cg.h"
using namespace std;

const int windowsSize=800;

void drawLine(){
    //测试有8个
    /**
     * 1. 水平的 (-1,8) (10, 8)
     * 2. 垂直的 (7,-2) (7, 8)
     * 3. k = 1 (-10,-10) (8,8)
     * 4. k = -1 (10,-10) (-8,8)
     * 5. 0< k <1 (-9,-7) (10,9) => k=0.84
     * 6. k > 1 (2,16) (-3,-15) => k=6.2
     * 7. 0< k < -1 (-2,1) (9,-6) => k-0.63
     * 8. k < -1 (-2,8) (8,-20) => k=-2.8
     */
  Screen* line = new Line(-2,8,8,-20,windowsSize,20);
  line->paint();
  delete line;
}

void drawCircle(){
    Screen* circle = new Circle(0,0,10,windowsSize,20);
    circle->paint();
    delete circle;
}

void drawPolygon(){
    vector<Point> points = {Point(5,5),Point(3, -2),Point(-2,1)};
    Point seed(0,0);
    Screen* polygon = new Polygon(points,seed,windowsSize,10);
    polygon->paint();
    //delete polygon;
}


int main(int argc,char **argv) {
    glutInit(&argc,argv);
    glutInitWindowSize(windowsSize,windowsSize);
    glutCreateWindow("CG_HOME_WORK!");
    glutDisplayFunc(drawPolygon);
    glutMainLoop();
}