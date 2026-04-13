#ifndef _SCLASS2_13_SAMPLE_H_
#define _SCLASS2_13_SAMPLE_H_
#include<iostream>
using namespace std;

class CSample
{
public:
    CSample(){};
    CSample(int a);
    CSample Add(CSample s1,CSample s2);
    int GetN();
private:
    int n;
};
#endif