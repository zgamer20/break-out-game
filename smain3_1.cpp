#include"sclass3_1_A.h"
int main()
{
    B ObjB;
    ObjB.SetValue(1,2,3,4,5);
    ObjB.Display();
    cout<<"z="<<ObjB.z<<endl;
    //cout<<"y="<<ObjB.y<<endl;	基类保护数据,不可通过对象访问
    //cout<<"m="<<ObjB.m<<endl; 派生类保护数据,不可通过对象访问
    //cout<<"x="<<ObjB.x<< endl; 基类私有数据,派生类不可访问
    //cout<<"n="<<ObjB.n<<endl;	//派生类私有数据,不可通过对象访问
    return 0;
}