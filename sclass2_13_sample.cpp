#include "sclass2_13_sample.h"
CSample::CSample(int a)
{
    n=a;
}
CSample CSample::Add(CSample s1,CSample s2)
{
    this->n=s1.n+s2.n;
    return *this;
}
int CSample::GetN()
{
    return n;
}