#include<iostream>
using namespace std;
class GameObject
{
protected:
    int position;
public:
    GameObject():position(0){}
    GameObject(int pos):position(pos){}

    int getPosition() const {return position;}
    void setPosition(int pos){position=pos;}

    void Print()const
    {
        cout<<"Position:"<<position<<endl;
    }
};

class PhysicalObject:virtual public GameObject
{
private:
    int velocity;
    int radius;
public:
    PhysicalObject():velocity(0),radius(0){}
    PhysicalObject(int pos,int vel,int r):GameObject(pos),velocity(vel),radius(r){}
    
    int getVelocity() const{return velocity;}
    void setVelocity (int vel) {velocity=vel;}
    int getRadius() const{return radius;}
    void setRadius(int r){radius=r;}

    void Print() const
    {
        cout<<"Velocity:"<<velocity<<",Radius:"<<radius<<endl;
    }
};

class VisualObject:virtual public GameObject
{
private:
    int color;
    int visible;
public:
    VisualObject():color(0),visible(1){}
    VisualObject(int pos,int c,int vis):GameObject(pos),color(c),visible(vis){}

    int getColor() const {return color;}
    void setColor(int c) {color=c;}
    int isVisible() const {return visible;}
    void setVisible(int vis) {visible=vis;}

    void Print() const 
    {
        cout<<"Color:"<<color<<",Visible:"<<(visible?"Yes":"No")<<endl;
    }
};

class Ball:public PhysicalObject,public VisualObject
{
private:
    int scoreValue;
public:
    Ball():scoreValue(0){}
    Ball(int pos,int vel,int r,int c,int vis,int score)
        :GameObject(pos),
        PhysicalObject(pos,vel,r),
        VisualObject(pos,c,vis),
        scoreValue(score){}

    int getScoreValue() const { return scoreValue; }
    void setScoreValue(int score) { scoreValue = score; }

    void Print() const {
        cout<<"=== Ball Info ==="<<endl;
        PhysicalObject::Print();
        VisualObject::Print();
        cout<<"Score value:"<<scoreValueS<<endl;
    }
};

int main()
{
    Ball ball(100,5,10,20,1,10);
    ball.Print();

    return 0;
}