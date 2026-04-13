//class Circle
// {
// public:
//     void SetRadius(double r) {radius=r;}
//     double GetRadius() const {return radius;}
//     double GetArea() const {return PI*radius*radius;}
//     void Print() const
//     {
//         cout<<"半径为"<<radius<<"的圆的面积是"<<GetArea()<<endl;
//     }
// private:
//     double radius;
// };

class Cylinder : public Circle //派生类的声明：class 派生类名：继承方式 基类名1,继承方式 基类名2
{
public:
    void SetHeight(double h) {height=h;}//新增
    double getHeight() const {return height;}//新增
    double getArea() const
    {
        // Circle类已有此成员函数，但实现内容改变了
        //凡是基类中函数原型与派生类的相同但实现代码不同的成员函数，需要在派生类中重新定义
        return 2*PI*radius*radius+2*PI*radius*height;
    }
    double GetVolume() const{return PI*radius*radius*height;}
    void Print() const
    {
        cout<<"底半径为"<<radius<<"的圆柱体的表面积是"<<getArea()
        <<"体积是"<<getVolume()<<endl;
    }
private:
    double height;
};

// 当继承方式为private时即表示私有继承
// 在私有继承方式下，基类所有的非私有成员，在派生类中一律变成派生类的私有成员，派生类只能通过它的成员函数来访问，派生类对象不能访问；
// 基类的私有成员，在派生类中不可见，只能由基类的成员函数来访问
// cylinder.SetRadius(12.5);       // 访问非法，因为setRadius()
//      在派生类中已变成私有成员，派生类对象在类外不能访问它
// 	cout<< cylinder.GetRadius( )<<endl;// 访问非法，理由同上
// 	cout<< cylinder.radius <<endl;  // 错误，radius是基类私有成员
// 	cout<< cylinder. height <<endl; // 错误， 类外不可访问派生类
// 		的私有成员height
