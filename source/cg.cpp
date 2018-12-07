//
// Created by 周亮 on 2018/12/6.
//
//屏幕的公共实现类
#include "../head/cg.h"

Screen::Screen(int width, int number, int point_size) {
    this->height = this->width = width;
    this->point_size = point_size;
    this->number = number;
    this->delta_hei = 1.0f / this->number;

}

void Screen::paint() {

    //背景的画法都是统一的
    this->drawBackground();
    this->draw();
}

void Screen::drawBackground() {

    glClearColor(1.0, 1.0, 1.0, 1.0);//这是设置背景色,必须要在glclear之前调用
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,0);
    for (int i = 1; i < this->number<<1 ; i ++ ) { //利用循环绘制网格
        glBegin(GL_LINES);
        glVertex2f(-1+i*delta_hei, -1);
        glVertex2f(-1+i*delta_hei, 1);
        glVertex2f(-1,-1+i*delta_hei);
        glVertex2f(1,-1+i*delta_hei);
        glEnd();
        glFlush();}
    glColor3f(1.0,0.0,0);
    glBegin(GL_LINES);          //绘制坐标系，便于观察
    glVertex2f(-1,0);
    glVertex2f(1,0);
    glVertex2f(0,-1);
    glVertex2f(0,1);
    glEnd();
    glFlush();

}

void Screen::drawPoint(const Point& point) {
    glBegin(GL_POINTS);
    glVertex2f(point.x * delta_hei, point.y *delta_hei);
    //glVertex2i(1.0,1.0);
    glEnd();
    glFlush();
}

Screen::~Screen() {
    cout<< "delete screen"<<endl;
}

Point::Point() {}

Point::Point(int x, int y) {
    this->x= x;
    this->y =y;
}
