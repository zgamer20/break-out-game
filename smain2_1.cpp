#include"sclass2_1_date.h"
int main()
{
    Date d1;
    d1.SetDate(2012,11,11);
    d1.Print();
    if(d1.IsLeapYear()) cout<<"今年是闰年"<<endl;
    else cout<<"今年不是闰年"<<endl;

    return 0;
}