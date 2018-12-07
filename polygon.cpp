//
// Created by 周亮 on 2018/12/7.
//多边形填充相关的实现
#include "cg.h"
Polygon::Polygon(vector<Point> &polygonPoints, Point seedPoint, int width, int number, int point_size):Screen(width,number,point_size) {
    //vector赋值是深拷贝
    this->polygonPoints = polygonPoints;
    this->seedPoint = seedPoint;

    //构造线
    vector<Point>::iterator it = this->polygonPoints.begin()+1;
    vector<Point>::iterator end = this->polygonPoints.end();
    while (it != end){
        this->lines.emplace_back(Line(*(it-1),*it,this->width,this->number,this->point_size));
        it++;
    }
    this->lines.emplace_back(Line(*(this->polygonPoints.begin()),*(this->polygonPoints.end()-1),this->width,this->number,this->point_size));
}

void Polygon::draw() {
    //先画线
    vector<Line>::iterator it=lines.begin();
    while (it != lines.end()){
        (*it).draw();
        it++;
    }
}

Polygon::~Polygon() {
    //cout<<"delete Polygon"<<endl;
}

