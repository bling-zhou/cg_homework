//
// Created by 周亮 on 2018/12/7.
//画圆的实现
#include "cg.h"

Circle::Circle(int x, int y, int r, int width, int number, int point_size):Screen(width,number,point_size) {
    center.x = x;
    center.y = y;
    this->r = r;
}



const GLfloat Pi=3.1415926536f;

void Circle::draw() {
    Point current;
    current.x = 0;
    current.y = r;
    d = 1 - r;
    //画真正圆
    glColor3f(0.0,1.0,0.0);

    glBegin(GL_POLYGON);
    int n= 120;
    for(int i=0;i<n;i++)
        glVertex2f(center.x * delta_hei + r*delta_hei * cos(2*Pi/n*i),center.y * delta_hei + r* delta_hei * sin(2*Pi/n*i));
    glEnd();
    glFlush();


    points.push_back(current);
    while(current.x <= current.y){
        current = getNextPoint();
    }
    vector<Point> mirrorPoints(8);



    //设置颜色
    glColor3f(0.0,0.0,0.0);
    glPointSize(point_size);
    vector<Point>::iterator iterator = points.begin();
    vector<Point>::iterator end = points.end();
    for(;iterator != end; ++iterator){
        mirrorPoints = getAllMirrorPoint(*iterator,mirrorPoints);
        //画点
        for(int i=0 ; i<mirrorPoints.size();i++){
            current = mirrorPoints[i];
            current.x = current.x + center.x;
            current.y = current.y + center.y;
            drawPoint(current);
        }
    }
}

Point Circle::getNextPoint() {
    Point  last = points[points.size()-1];
    if( d >0 ){
        d = d + 2 * last.x -2 * last.y +5;
        last.x = last.x + 1;
        last.y = last.y -1;
    } else{
        d = d + 2 * last.x +3;
        last.x = last.x +1;
    }
    points.push_back(last);
    return last;
}

vector<Point> Circle::getAllMirrorPoint(Point &point, vector<Point> &mirrorPoints) {
    mirrorPoints.clear();
    Point temp = point;
    //1.自己
    mirrorPoints.push_back(temp);

    //2. y=x 对称
    std::swap(temp.x,temp.y);
    mirrorPoints.push_back(temp);

    //3. x轴
    temp.y = -temp.y;
    mirrorPoints.push_back(temp);

    //4. y= -x
    temp.x = -temp.x;
    temp.y = -temp.y;
    std::swap(temp.x,temp.y);
    mirrorPoints.push_back(temp);

    //5. y轴
    temp.x = -temp.x;
    mirrorPoints.push_back(temp);

    //6. y= x;
    std::swap(temp.x,temp.y);
    mirrorPoints.push_back(temp);

    //7. x轴
    temp.y = -temp.y;
    mirrorPoints.push_back(temp);

    //8. y = -x
    temp.x = -temp.x;
    temp.y = -temp.y;
    std::swap(temp.x,temp.y);
    mirrorPoints.push_back(temp);


    return mirrorPoints;
}

Circle::~Circle() {
   // cout<<"delete circle"<<endl;
}
