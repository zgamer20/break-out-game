#include"sclass2_11_rectangle.h"
int main()
{
    Rectangle*rp=new Rectangle(10,5);
    rp->Print();
    rp->SetSize(45,20);
    cout<<"长45宽20的长方形的面积是"<<rp->GetArea()<<endl;
    delete rp;
    return 0;
}