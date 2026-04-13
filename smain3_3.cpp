#include"sclass3_3_cbase.h"
int main()
{
    CDerived d;
    d.SetValue(10,20);
    d.CalcuValue();
    d.SetJ();
    d.SetK();
    d.DisplayVal();
    return 0;
}