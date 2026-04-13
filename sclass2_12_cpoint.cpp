//this指针指向正在调用某个类的成员函数的对象
//目的是为了识别函数所操纵的数据成员所属的对象
//这里的目的是为了区分"成员函数"和"参数"
//因为同类对象的存在
#include"sclass2_12_cpoint.h"
CPoint::CPoint(float x,float y)
{
    this->x=x;
    this->y=y;
}
void CPoint::Move(float x,float y)
{
    this->x=x;
    this->y=y;
}