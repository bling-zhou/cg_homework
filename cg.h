//
// Created by 周亮 on 2018/12/6.
//
//计算机图形学的所有汇总头文件
#ifndef OPENCLTEST_CG_H
#define OPENCLTEST_CG_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <GLUT/GLUT.h>
#include <vector>
using namespace std;

//点
struct Point{
    int x;
    int y;

    Point();
    Point(int x,int y);
    bool operator==(Point point){
        return x==point.x && y == point.y;
    }
    bool operator!=(Point point){
        return !this->operator==(point);
    }
};

//屏幕，后面的直线，圆都都继承这个方法
//纯虚class
class Screen{
private://方法

    //画背景网格
    void drawBackground();

protected://属性
    //屏幕高度
    int height;
    //屏幕宽度
    int width;
    //点的大小
    int point_size;
    //用来控制坐标的密度
    int number;
    //坐标点的格子宽度,用来标记坐标位置
    float delta_hei;

protected://方法
    //具体的画图方法,由子类实现
    virtual void draw()=0;


    /**
     * 在指定点位置画出点
     * @param point 指定点点位置
     */
    void drawPoint(const Point& point);
public:
    //对外提供的公共方法
    void paint();
    Screen(int width=800,int number=10,int point_size=5);
    virtual ~Screen();

};

//直线class
class Line:public Screen{
private://成员
    //直线开始点的位置
    Point start;
    //直线结束点点为
    Point end;
    //当前花点直线点的位置
    Point current;
    //用来判断下一个点的位置
    int d=0;
    //两个x之间点距离，两个y之间点距离
    int deltax;
    int deltay;

    /**
     * 在 0<= k <= 1的情况下获得下一个点
     * @return 下一个点
     */
    Point getNextPointPositive();

    /**
     * 在 -1<= k <0 的情况下获得下一个点
     * @return 下一个点
     */
    Point getNextPointNegative();

public:
    void draw();

public://方法
    Line(int startx,int starty,int endx,int endy,int width=800,int number=20,int point_size=5);
    Line(Point start,Point end,int width=800,int number=20,int point_size=5);
    ~Line();

};

//圆的class
class Circle:public Screen{
private:
    //圆心
    Point center;
    //半径
    int r;
    //采用1/8画圆的方法
    vector<Point> points;

    int d;

    /**
     * 根据points的最后一个点来获取下一个点,中点算法
     * @return 下一个点
     */
    Point getNextPoint();

    /**
     * 获得一个点的八个对称的点
     * @param point 初始化点
     * @param mirrorPoints 用来存放结果
     * @return 八分圆法点对称8个点对称
     */
    vector<Point> getAllMirrorPoint(Point& point,vector<Point>& mirrorPoints);
protected:
    virtual void draw();
public:
    //构造函数
    Circle(int x,int y,int r,int width=800,int number=20,int point_size=5);
    ~Circle();
};

class Polygon:public Screen{
private:
    //多边形的点，连续点
    vector<Point> polygonPoints;
    //多边形的边
    vector<Line> lines;
    //种子点
    Point seedPoint;
    //采用点填充方法，4填充或者8填充
    int type=4;
protected:
    virtual void draw();
public:
    //构造函数
    Polygon(vector<Point>& polygonPoints,Point seedPoint,int width=800,int number=20,int point_size=5);
    ~Polygon();
};

#endif //OPENCLTEST_CG_H
