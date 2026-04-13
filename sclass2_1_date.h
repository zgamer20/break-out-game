#include<iostream>
using namespace std;
class Date
{
public:
    void SetDate(int y,int m,int d)
    {
        year=y;
        month=m;
        day=d;
    }

    bool IsLeapYear()
    {
        return (year%4==0&&year%100!=0)||(year%400==0);
    }

    void Print()
    {
        cout<<year<<"年"<<month<<"月"<<day<<"日"<<endl;
    }

private:
    int year;
    int month;
    int day;
};