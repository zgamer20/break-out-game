//虚基类举例（一般用于菱形继承？）
#include<iostream>
using namespace std;

class Furniture // 声明公共基类
{
protected:
    float weight;
public:
    Furniture(float w):weight(w){}
    void SetWeight(float w){weight=w;}
    float GetWeight() const {return weight;}
};

class Bed:virtual public Furniture
{
public:
    Bed(float w):Furniture(w){}//初始化公共基类数据成员
    void Sleep(){cout<<"开始睡觉"<<endl;}
};

class Sofa:virtual public Furniture
{
public:
    Sofa(float w):Furniture(w){}
    void WatchTV(){cout<<"正在看电视"<<endl;}
};

class SofaBed:public Sofa,public Bed
{
public:
    SofaBed(float w):Furniture(w),Sofa(w),Bed(w){}
    void FoldOut(){cout<<"可以折叠打开"<<endl;}
};

int main()
{
    SofaBed sb(50.5);
    sb.SetWeight(51.9);
    cout<<"家具重"<<sb.GetWeight()<<"千克"<<endl;
    sb.WatchTV();
    sb.FoldOut();
    sb.Sleep();

    return 0;
}