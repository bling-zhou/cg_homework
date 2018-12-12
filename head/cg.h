//
// Created by 周亮 on 2018/12/6.
//
//计算机图形学的所有汇总头文件
#ifndef OPENCLTEST_CG_H
#define OPENCLTEST_CG_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <GLUT/glut.h>
#include <vector>
#include <map>
#include <float.h>
#include <limits.h>

using namespace std;

//点
struct Point{
    int x;
    int y;

    Point();
    Point(int x,int y);
    bool operator==(const Point& point)const{
        return x==point.x && y == point.y;
    }

    bool operator!=(const Point& point)const{
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

    //一个单位占用的长度
    int avg_length;

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
    //获得一点的color,x和y是坐标,color是输出
    void getPointColor(int x,int y,GLubyte* color);
    explicit Screen(int width=800,int number=10,int point_size=5);
    virtual ~Screen();

};

//直线class
class Line:public Screen{
private://成员

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
    //直线开始点的位置
    Point start;
    //直线结束点点为
    Point end;
    //需要使得最后的直线表达式变为 x = ky +b
    //方便后面计算
    float k;
    float b;

    void draw() override;

    /**
    * 画一条线，同时在直角坐标系中标出直线点的坐标
    * 这个是特别的给多边形填充使用,主要是因为没办法获得颜色，
    * 用coordinates来标记颜色
    * coordinates的坐标原点在左上角
    * @param coordinates 像素点的二位矩阵
    * @param flag 填充的标志
    */
    void drawAndMark(vector<vector<int>>& coordinates,int flag);

    /**
     * 带入 x = ky + b中求出x
     * @param y 输入的y值
     * @return 直线相交的x点 ,如果返回点是FLT_MAX的话，说明当前的直线是y= y0的,返回FLT_MIN是没有交点;
     */
    float  getx(int y);

public://方法
    Line(int startx,int starty,int endx,int endy,int width=800,int number=20,int point_size=5);
    Line(Point start,Point end,int width=800,int number=20,int point_size=5);
    ~Line() override;

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
    void draw() override;
public:
    //构造函数
    Circle(int x,int y,int r,int width=800,int number=20,int point_size=5);
    ~Circle() override;
};

/**
 * 这个结构体涉及到排序
 * 排序规则依次看 ymin k
 */
struct EdgeNode{
    float x;
    float k;
    int y;
    int ymax;

    bool operator< (const EdgeNode& edgeNode) const{
        if(this->x==edgeNode.x){
            return this->k < edgeNode.k;
        } else{
            return this->x < edgeNode.x;
        }
    }
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
    //记录坐标系
    vector<vector<int>> coordinates;

    /**
     * 有效边表法填充
     */
    void activeEdge();

    /**
     * 图像填充，4邻接点或8邻接点画法
     */
    void fill(Point point);

    /**
     * 获得点的标志
     * @param point
     * @return 标志
     */
    int getFlag(Point point);

    /**
     * 以窗口中点为原点点坐标系
     * @param x x坐标
     * @param y y坐标
     * @return 标志为
     */
    int getFlag(int x , int y);
    /**
     * set点点flag，这个点是以窗口中心为原点点
     * @param x x坐标
     * @param y y坐标
     * @param flag 设置点标志位
     */
    void setFlag(int x,int y,int flag);

    map<int,vector<EdgeNode>> getET(map<int,vector<EdgeNode>>& ET);

protected:
    void draw() override;
public:
    //构造函数
    Polygon(vector<Point>& polygonPoints,Point seedPoint,int width=800,int number=20,int point_size=5);
    ~Polygon() override;
};

#endif
//OPENCLTEST_CG_H
