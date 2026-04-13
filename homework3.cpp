#include<iostream>
#include<string>
using namespace std;
class CBuilding{
protected:
    int floors;
    int rooms;
    double area;
public:
    CBuilding(int f=0,int r=0,double a=0.0)
        :floors(f),rooms(r),area(a){}
    
    void showInfo() const{
        cout<<"楼房信息："<<endl;
        cout<<"层数："<<floors<<endl;
        cout<<"房间数："<<rooms<<endl;
        cout<<"总面积："<<area<<"平方米："<<endl;
    }
};

class CHousing :public CBuilding {
private:
    int bedrooms;
    int bathrooms;
public:
    CHousing(int f=0,int r=0,double a=0.0,int bdr=0,int bth=0)
        :CBuilding(f,r,a),bedrooms(bdr),bathrooms(bth) {}
    void showInfo() const{
        cout<<"====住宅楼信息===="<<endl;
        CBuilding::showInfo();
        cout<<"卧室数："<<bedrooms<<endl;
        cout<<"浴室数："<<bathrooms<<endl;
        cout<<"================="<<endl<<endl;
    }
};

class COfficBuilding:public CBuilding{
private:
    int fireExtinguisher;
    int phone;
public:
    COfficBuilding(int f=0,int r=0,double a=0.0,int fe=0,int p=0)
        :CBuilding(f,r,a),fireExtinguisher(fe),phone(p){}
    void showInfo() const{
        cout<<"====办公楼信息===="<<endl;
        CBuilding::showInfo();
        cout<<"灭火器数："<<fireExtinguisher<<endl;
        cout<<"灭火器数："<<phone<<endl;
        cout<<"================="<<endl<<endl;
    }
};

int main(){
    CHousing house(5,20,1500.0,15,8);
    COfficBuilding office(10,50,4000.0,30,25);

    house.showInfo();
    office.showInfo();

    return 0;
}