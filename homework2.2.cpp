#include<iostream>
using namespace std;

class Time
{
private:
    int hour;
    int minute;
    int second;
public:
    Time()
    {
        hour=0;
        minute=0;
        second=0;
    }
    Time(int h,int m,int s)
    {
        hour=h;
        minute=m;
        second=s;
    }
    void Print()
    {
        cout<<hour<<":"<<minute<<":"<<second<<endl;
    }
};

int main()
{
    Time t1;
    t1.Print();

    Time t2(11,59,59);
    t2.Print();
    return 0;
}