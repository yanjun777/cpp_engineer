
#include<iostream>
using namespace std;


int Testfunc(string str){
    cout<<"this is a test function"<<endl;
    cout<<"str:"<<str<<endl;
    return 0;
}

class Myclass{
public:
    using CFunc = int (Myclass::*)(string); 
    Myclass(){
        func_ptr_ = &Myclass::Testfunc;   
    }
    ~Myclass(){
    }
    int Testfunc(string str){
        cout<<"this is a member function"<<endl;
        cout<<"str:"<<str<<endl;
        return 0;
    }
    void Call(){
        (this->*func_ptr_)("Calling member function"); 
    }

private:
    CFunc func_ptr_ {nullptr}; 
};


int main() {
    {
        // --------------测试2：成员函数---------------
        //1.声明类型 int (Myclass::*)(string)   
        //2.赋值 成员函数指针赋值时   &MyClass::TestFunc 
        //3.调用 (对象.*成员函数指针)(参数列表)  或者 (对象指针->*成员函数指针)(参数列表)
        using CFunc= int(Myclass::*)(string);
        CFunc func_ptr= &Myclass::Testfunc; 
        Myclass mycls; 
        (mycls.*func_ptr)("func_ptr"); 

        int(Myclass::*func_ptr2)(string);
        func_ptr2 = &Myclass::Testfunc;  
        (mycls.*func_ptr2)("func_ptr2");
    }

    {
        // --------------测试3：类内部调用---------------
        //1.声明类型 int (Myclass::*)(string) 
        //2.赋值 成员函数指针赋值时   &MyClass::TestFunc 
        //3.调用 (对象.*成员函数指针)(参数列表)  或者 (对象指针->*成员函数指针)(参数列表)
        Myclass mycls;
        mycls.Call();
    }

    {
        using FuncType = int (*)(string); 
        // --------------测试1：函数指针---------------
        int (*func_ptr)(string);
        func_ptr = Testfunc; 
        func_ptr("hello world");
        FuncType func_ptr2;
        func_ptr2 = Testfunc;
        func_ptr2("hello world 2"); 
    }

    return 0;
}