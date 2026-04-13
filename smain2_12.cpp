#include"sclass2_12_cpoint.h"
int main()
{
    CPoint*p1=new CPoint(1,1);
    p1->Move(10,10);
    cout<<"("<<p1->x<<","<<p1->y<<")"<<endl;
    delete p1;
    return 0;
}