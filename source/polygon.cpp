//
// Created by 周亮 on 2018/12/7.
//多边形填充相关的实现
#include "../head/cg.h"


Polygon::Polygon(vector<Point> &polygonPoints, Point seedPoint, int width, int number, int point_size):Screen(width,number,point_size) {
    //vector赋值是深拷贝
    this->polygonPoints = polygonPoints;
    this->seedPoint = seedPoint;

    //构造线
    auto it = this->polygonPoints.begin() + 1;
    auto end = this->polygonPoints.end();
    while (it != end){
        this->lines.emplace_back(Line(*(it-1),*it,this->width,this->number,this->point_size));
        it++;
    }
    this->lines.emplace_back(Line(*(this->polygonPoints.end()-1),*(this->polygonPoints.begin()),this->width,this->number,this->point_size));

    //初始化坐标
    for(int i=0 ;i<this->number*2 ;i++){
        this->coordinates.emplace_back(vector<int>(this->number*2,0));
    }
}

void Polygon::draw() {
    //先画线
    auto it=lines.begin();
    while (it != lines.end()){
        (*it).drawAndMark(this->coordinates,1);
        it++;
    }

     this->activeEdge();

    //glColor3f(1.0,0,1.0);
    //fill(seedPoint);
}

Polygon::~Polygon() {}

void Polygon::fill(Point point) {
    //0表示背景色, 1表示边界色
    int flag = getFlag(point);
    if(flag != 1){
        if(flag !=2){
            this->drawPoint(point);
            setFlag(point.x,point.y,2);
        }
        //访问四邻接点
        if(getFlag(point.x-1,point.y) == 0){
            fill(Point(point.x - 1 , point.y));
        }
        if(getFlag(point.x+1,point.y) == 0) {
            fill(Point(point.x + 1 , point.y));
        }
        if(getFlag(point.x,point.y-1) == 0) {
            fill(Point(point.x , point.y -1));
        }
        if(getFlag(point.x , point.y+1) == 0) {
            fill(Point(point.x , point.y + 1));
        }
        //8邻接点
        if(type == 8){
            if(getFlag(point.x-1,point.y+1) == 0){
                fill(Point(point.x - 1 , point.y+1));
            }
            if(getFlag(point.x+1,point.y-1) == 0) {
                fill(Point(point.x + 1 , point.y-1));
            }
            if(getFlag(point.x+1,point.y-1) == 0) {
                fill(Point(point.x+1 , point.y -1));
            }
            if(getFlag(point.x-1 , point.y+1) == 0) {
                fill(Point(point.x , point.y + 1));
            }
        }
    }

}

int Polygon::getFlag(Point point) {
    int x = point.x +this->number;
    int y = this->number - point.y;
    if(x>=0 && x<coordinates.size() && y>=0 && y<coordinates[x].size()){
        return coordinates[x][y];
    }
    return 1;
}

void Polygon::setFlag(int x, int y, int flag) {
     x =x +this->number;
     y = this->number - y;
    if(x>=0 && x<coordinates.size() && y>=0 && y<coordinates[x].size()){
        coordinates[x][y]=flag;
    }
}

int Polygon::getFlag(int x, int y) {
    x = x +this->number;
    y = this->number - y;
    if(x>=0 && x<coordinates.size() && y>=0 && y<coordinates[x].size()){
        return coordinates[x][y];
    }
    return 1;
}

map<int,vector<EdgeNode>> Polygon::getET(map<int,vector<EdgeNode>>& ET){
    for(int i=0;i < lines.size(); i++){
        //左边线
        int left = (i+lines.size()-1)%lines.size();
        //右边线
        int right = (i+1)%lines.size();

        //与x平行的线不考虑
        if(lines[i].start.y == lines[i].end.y) continue;

        //一条线一个边
        EdgeNode tmp;
        tmp.k = lines[i].k;
        int y;
        if(lines[i].start.y > lines[i].end.y){
            y = lines[i].end.y;
            tmp.x = lines[i].end.x;
            tmp.y = y;

            Line line1 = lines[i];
            Line lin2 = lines[left];
            int flag=(lines[i].start.y-lines[i].end.y)*(lines[left].start.y-lines[left].end.y);
            //比较 left 和 i
            if(flag<=0){
                //同侧
                tmp.ymax = lines[i].start.y;
            } else{
                //异侧
                tmp.ymax = lines[i].start.y-1;
            }
        } else{
            y = lines[i].start.y;
            tmp.x = lines[i].start.x;
            tmp.y = y;
            //比较i 和 right
            if((lines[i].start.y-lines[i].end.y)*(lines[right].start.y-lines[right].end.y)<=0){
                //同侧
                tmp.ymax = lines[i].end.y;
            } else{
                //异侧
                tmp.ymax = lines[i].end.y-1;
            }
        }

        auto it = ET.find(y);
        vector<EdgeNode> vector1;
        if(it == ET.end()){
            //不存在
            ET[y] = vector1;
        }
        ET[y].push_back(tmp);
    }

    //ET排序
    auto it = ET.begin();
    while (it !=ET.end()){
        sort(it->second.begin(),it->second.end());
        it++;
    }

    return ET;
}

void printfET(const map<int,vector<EdgeNode>>& ET){

    auto it = ET.begin();
    while (it !=ET.end()){
        cout<<it->first<<":";
        vector<EdgeNode> vector1 = it->second;
        auto it1 = vector1.begin();
        while (it1 != vector1.end()){
            cout<<"("<< (*it1).x<<","<<(*it1).ymax<<","<<(*it1).k<<") ";
            it1++;
        }
        cout<<endl;
        it++;
    }

}

void Polygon::activeEdge() {
    //用vector是为了提高STL排序的效率, 用map是为了最大化减少内存
    //创建一个边表采用map数据结构 map<int,vector<EdgeNode>>
    map<int,vector<EdgeNode>> ET;//边表
    getET(ET);

    printfET(ET);

    glColor3f(1.0,0,1.0);
    //初始化第一个有效边表置null,置y为最小的起始点,有效边表 vector<EdgeNode>
    vector<EdgeNode> AET;
    int indexy=ET.begin()->first;
    //do-while循环，知道有效边表变为null,
    do{

        //循环体 1.根据y合并边表 2.填充 3.计算修正有效边表 4.循环跳出依据(有效边表是不是为null)
        if(ET.find(indexy)!=ET.end()){
            //有，合并
            AET.insert(AET.end(),ET[indexy].begin(),ET[indexy].end());
        }
        sort(AET.begin(),AET.end());
        bool f = false;
        float count=(*AET.begin()).x;
        auto it = AET.begin();
        while (it !=AET.end()){
            if(f){
                int begin = (int) (count+0.5);
                int end =(int) ((*it).x+0.5);
                //画点
                for(int i=begin;i<end;i++){
                    if(getFlag(i,indexy)==0){
                        drawPoint(Point(i,indexy));
                    }
                }
            }
            f = !f;

            count = (*it).x;
            if(indexy == (*it).ymax){
                it = AET.erase(it);
            } else{
                //更新x
                (*it).x +=(*it).k;
                it++;
            }
        }
        indexy++;
    }while(AET.size()!=0);
}




