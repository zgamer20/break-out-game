#include"sclass2_8_cpoint.h"
CPoint::CPoint(int x1,int y1)
{
    x=x1;
    y=y1;
}//以创建好的对象
CPoint::CPoint(const CPoint&obj)//拷贝构造函数
{
    x=obj.x;
    y=obj.y;
}//正在创建的同类对象
int CPoint::GetX()
{
    return x;
}
int CPoint::GetY()
{
    return y;
}
