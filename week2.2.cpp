#include<iostream>
#include<vector>
using namespace std;

class GameObject
{
public:
    virtual void Update()=0;
    virtual void Draw()=0;
    virtual ~GameObject(){};
};

class Ball: public GameObject
{
private:
    float x,y;
    float radius;
public:
    Ball(float x=0,float y=0,float r=10):x(x),y(y),radius(r){}
    void Update() override{
        cout<<"球的坐标更新为("<<x<<","<<y<<")"<<endl;
    }
    void Draw() override{
        cout<<"绘制半径为"<<radius<<"的球"<<endl;
    }
};

class Paddle: public GameObject
{
private:
    int width;
    int height;
    int speed;
public:
    Paddle(int w=80,int h=20,int s=5):width(w),height(h),speed(s){}
    void Update(){
        cout<<"挡板以速度"<<speed<<"移动"<<endl;
    }
    void Draw(){
        cout<<"绘制尺寸"<<width<<"*"<<height<<"的挡板"<<endl;
    }
};

class Brick: public GameObject
{
private:
    int lives;
    string color;
public:
    Brick(int init=1,string c="pink"):lives(init),color(c){}
    void Update(){
        cout<<"砖块当前生命值："<<lives<<endl;
    }
    void Draw(){
        cout<<"绘制颜色为"<<color<<"的砖块"<<endl;
    }
};

int main()
{
    vector<GameObject*>gameObjects;
    gameObjects.push_back(new Ball(100,200,15));
    gameObjects.push_back(new Paddle(100,25,6));
    gameObjects.push_back(new Brick(2,"blue"));
    cout<<"=====游戏一帧更新与绘制====="<<endl;
    for(auto obj:gameObjects){
        obj->Update();
        obj->Draw();
        cout<<"=========="<<endl;
    }
    for(auto obj:gameObjects){
        delete obj;
    }
    gameObjects.clear();
    return 0;
}