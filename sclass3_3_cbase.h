#include<iostream>
using namespace std;

class CBase
{
public:
    CBase(){m=0;n=0;}
    void SetValue(int a,int b);
    void CalcuValue();
protected:
    int m;
private:
    int n;
};

class CDerived:public CBase
{
private:
    int j;
    int k;
public:
    CDerived(){}
    void SetJ();
    void SetK();
    void DisplayVal() const;
};