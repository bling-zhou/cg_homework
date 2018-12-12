//
// Created by 周亮 on 2018/12/5.
//
#include "../head/cg.h"


//子类的构造函数
Line::Line(int startx, int starty, int endx, int endy, int width, int number, int point_size): Screen(width,number,point_size){
    this->start.x = startx;
    this->start.y = starty;
    this->end.x = endx;
    this->end.y = endy;

    //计算k,这个是竖值的直线的表达式为 x = endx
    if(endy == starty){
        k = FLT_MAX;
    } else{
        k = 1.0f *(endx - startx) / (endy - starty);
        b = endx - k * endy;
    }
}
Line::Line(Point start, Point end, int width, int number, int point_size): Line(start.x,start.y,end.x,end.y,width,number,point_size) {}

void Line::draw() {
    cout<<"draw line:("<<start.x <<","<<start.y<<") ("<<end.x<<","<<end.y<<")\n";
    //临时变量，主要是后面可能会调换
    const Point tempStart = this->start;
    const Point tempEnd = this->end;

    //确保Xs 比 Xe 小
    if(start.x > end.x){
        std::swap(this->start,this->end);
    }

    //画出真正的直线做下对比
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
    glVertex2f(start.x * delta_hei , start.y * delta_hei);
    glVertex2f(end.x * delta_hei , end.y * delta_hei);
    glEnd();
    glFlush();


    //计算deltax和deltay
    deltax = end.x - start.x;
    deltay = end.y - start.y;
    //设置点尺寸
    glPointSize(point_size);
    //设置颜色
    glColor3f(0.0,0.0,0.0);

    //画开始点,点为黑色
    this->drawPoint(tempStart);

    //四种情况 k与0比较两种情况
    if(deltay >0 ){
        // k>0
        // k 需要在和 1 比较
        if(deltay <= deltax){
            // 0 < k <=1
            d = deltax - (deltay<<1);
            current.x = start.x;
            current.y = start.y;
            while (current != end){
                current = getNextPointPositive();
                this->drawPoint(current);
            }
        } else{
            // k >=1
            //根据对称来做
            std::swap(start.x,start.y);
            std::swap(end.x,end.y);
            std::swap(deltax,deltay);
            //同上面点基本一样
            d = deltax - (deltay<<1);
            current.x = start.x;
            current.y = start.y;
            while (current != end){
                Point temp = getNextPointPositive();
                std::swap(temp.x,temp.y);
                this->drawPoint(temp);
            }
        }
    } else{
        // k<=0
        // k 需要和 -1 进行比较
        if( abs(deltay) <= deltax){
            // -1<= k <=0
            d = -deltax - (deltay<<1);
            current.x = start.x;
            current.y = start.y;
            while (current != end){
                current = getNextPointNegative();
                drawPoint(current);
            }
        } else{
            // k < -1  y0 > y1 (负数的原因)
            // 算法是以 x0 < x1开始的
            std::swap(start,end);
            std::swap(start.x , start.y);
            std::swap(end.x , end.y);
            std::swap(deltax, deltay);
            deltax = -1 * deltax;
            deltay = -1 * deltay;
            current.x = start.x;
            current.y = start.y;
            d = -deltax - (deltay<<1);
            while (current != end){
                Point temp = getNextPointNegative();
                std::swap(temp.x , temp.y);
                drawPoint(temp);
            }
        }
    }

    //画结束点
    this->drawPoint(tempEnd);

    this->start = tempStart;
    this->end = tempEnd;

}

Point Line::getNextPointNegative() {
    if(d > 0){
        current.x = current.x + 1;
        current.y = current.y - 1;
        d = d - (deltax<<1) - (deltay<<1);
    } else{
        current.x = current.x + 1;
        d = d - (deltay<<1);
    }
    return current;
}


// 0< k <1
Point Line::getNextPointPositive() {
    if(d > 0){
        current.x = current.x + 1;
        d = d - (deltay<<1);
    } else{
        current.x = current.x + 1;
        current.y = current.y + 1;
        d = d - (deltay<<1) + (deltax<<1);
    }
    return current;
}

Line::~Line() {}

void Line::drawAndMark(vector<vector<int>>& coordinates, int flag) {

    //临时变量，主要是后面可能会调换
    const Point tempStart = this->start;
    const Point tempEnd = this->end;

    //确保Xs 比 Xe 小
    if(start.x > end.x){
        std::swap(this->start,this->end);
    }

    //画出真正的直线做下对比
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
    glVertex2f(start.x * delta_hei , start.y * delta_hei);
    glVertex2f(end.x * delta_hei , end.y * delta_hei);
    glEnd();
    glFlush();


    //计算deltax和deltay
    deltax = end.x - start.x;
    deltay = end.y - start.y;
    //设置点尺寸
    glPointSize(point_size);
    //设置颜色
    glColor3f(0.0,0.0,0.0);

    //画开始点,点为黑色
    this->drawPoint(tempStart);
    //设置flag
    if(coordinates.size() > tempStart.x+this->number && coordinates[tempStart.x+this->number].size() > this->number-tempStart.y){
        coordinates[tempStart.x+this->number][this->number-tempStart.y]=flag;
    }
    //四种情况 k与0比较两种情况
    if(deltay >0 ){
        // k>0
        // k 需要在和 1 比较
        if(deltay <= deltax){
            // 0 < k <=1
            d = deltax - (deltay<<1);
            current.x = start.x;
            current.y = start.y;
            while (current != end){
                current = getNextPointPositive();
                this->drawPoint(current);
                if(coordinates.size() > current.x+number && coordinates[current.x+number].size() > number-current.y){
                    coordinates[current.x+number][number-current.y]=flag;
                }
            }
        } else{
            // k >=1
            //根据对称来做
            std::swap(start.x,start.y);
            std::swap(end.x,end.y);
            std::swap(deltax,deltay);
            //同上面点基本一样
            d = deltax - (deltay<<1);
            current.x = start.x;
            current.y = start.y;
            while (current != end){
                Point temp = getNextPointPositive();
                std::swap(temp.x,temp.y);
                this->drawPoint(temp);
                if(coordinates.size() > temp.x+number && coordinates[temp.x+number].size() > number-temp.y){
                    coordinates[temp.x+number][number-temp.y]=flag;
                }
            }
        }
    } else{
        // k<=0
        // k 需要和 -1 进行比较
        if( abs(deltay) <= deltax){
            // -1<= k <=0
            d = -deltax - (deltay<<1);
            current.x = start.x;
            current.y = start.y;
            while (current != end){
                current = getNextPointNegative();
                drawPoint(current);
                if(coordinates.size() > current.x+number && coordinates[current.x+number].size() > number-current.y){
                    coordinates[current.x+number][number-current.y]=flag;
                }
            }
        } else{
            // k < -1  y0 > y1 (负数的原因)
            // 算法是以 x0 < x1开始的
            std::swap(start,end);
            std::swap(start.x , start.y);
            std::swap(end.x , end.y);
            std::swap(deltax, deltay);
            deltax = -1 * deltax;
            deltay = -1 * deltay;
            current.x = start.x;
            current.y = start.y;
            d = -deltax - (deltay<<1);
            while (current != end){
                Point temp = getNextPointNegative();
                std::swap(temp.x , temp.y);
                drawPoint(temp);
                if(coordinates.size() > temp.x+number && coordinates[temp.x+number].size() > number-temp.y){
                    coordinates[temp.x+number][number-temp.y]=flag;
                }
            }
        }
    }

    //画结束点
    this->drawPoint(tempEnd);

    if(coordinates.size() > tempEnd.x+number && coordinates[tempEnd.x+number].size() > number-tempEnd.y){
        coordinates[tempEnd.x+number][number-tempEnd.y]=flag;
    }
    this->start = tempStart;
    this->end = tempEnd;
}

float Line::getx(int y) {
    if(y< min(end.y,start.y) || y >max(end.y,start.y)){
        return  FLT_MIN;
    }

    if( k != FLT_MAX){
        return k * y + b;
    }
    return FLT_MAX;
}




