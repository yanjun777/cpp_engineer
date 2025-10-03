#include<iostream>
using namespace std;

// 动态多态：继承并且重写了基类虚函数的情况下，需要在程序运行时使用基类指针指向派生类对象，调用某个虚函数产生！
// 没有成员变量 实例化仍按会分配 8字节，cpp需要使用地址去区分对象，如果没有内存分配无法区分对象。  
class base1{
public:
    // 1字节 
};
// 虚表指针(对象首地址) --> 虚函数表存储在rodate 只读数据段，存储着虚函数的地址（每一个类都公用一张虚函数表，子类和父类不同）
class base2{
public:
    virtual void foo(){
        std::cout<<"base2 foo"<<endl;
    }
    virtual void show(){
        std::cout<<"base2 show"<<endl; 
    }
};
class base3{
public:
    virtual void foo(){
        std::cout<<"base2 foo"<<endl;
    }
    virtual void show(){
        std::cout<<"base2 show"<<endl; 
    }
};
// 什么也不重写：仍然会有虚表指针 --> derived1的虚函数表 里面存储的虚函数地址与 base2 一致 
class derived1:public base2{

};
// 重写虚函数 虚表指针 --> derived1的虚函数表 里面存储的虚函数地址为derived2 重写后的虚函数地址  
class derived2:public base2{
    virtual void foo(){
        std::cout<<"derived2 foo"<<endl;
    }
    virtual void show(){
        std::cout<<"derived2 show"<<endl; 
    }
};
// 多重继承： 多个虚表指针，具体存储函数地址看是否进行了重写
class derived3:public base2, public base3{

};

// 静态多态
// 重载 同一个作用域下，函数名相同 形参的类型顺序不同
// 类模板 函数模板 变量的类型也作为一个类or函数的参数，对于某个功能可以不区分类型实现！ 
int main(){
    std::cout<<"bas1 size "<<sizeof(base1)<<endl; 
    std::cout<<"bas2 size "<<sizeof(base2)<<endl; 
    std::cout<<"deri1 size "<<sizeof(derived1)<<endl; 
    std::cout<<"deri2 size "<<sizeof(derived2)<<endl;
    std::cout<<"deri3 size "<<sizeof(derived3)<<endl; 
    
    base2 ba2; 
    base2 * pb2 = &ba2;
    base2 * pb2_1 = new derived2; 
    pb2->show();
    pb2_1->show(); 

    return 0; 

}

