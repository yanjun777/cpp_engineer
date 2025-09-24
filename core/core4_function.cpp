
#include<iostream>
#include<functional>
using namespace std;
// function 和 bind 
// function 简化成员函数调用步骤
// bind     简化默认需要传入类对象地址
// bind 和 function 配合使用 

// 绑定之后非常灵活 代码不需要变动  函数指针可以为func mem_func lambda 

// 函数指针 定义 调用 
int Testfunc(string str){
    cout<<"this is a test function"<<str<<endl;
    return 0;
}

// 成员函数函数指针 定义 and 调用
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

private:
    CFunc func_ptr_ {nullptr}; 
};
// function 函数指针绑定 调用
// function bind 绑定调用 
class Data{
public:
    Data(){
        // 取地址&域名下::函数
        func_=&Data::TestFunc; 
        // bind 返回的对象是function ? 
        funb_= bind(&Data::TestFunc,this,std::placeholders::_1); 
    }
    void Call(string str){
        func_(*this,str);  
        funb_(str); 
    }
    int TestFunc(string str){

        cout<<"this is Class Member function TestFunc: "<<str<<endl; 
        return 0;
    }
private:
// 第一个参数是类对象的引用 
    function<int(Data&,string)> func_; 
    function<int(string)> funb_;
};

void TestBind(int x, int y, string str, int count ){
    cout<<x<<":"<<y<<":"<<str<<":"<<count<<endl; 
}

int main() {
    {
        using namespace placeholders; 
        // 用户传入的第一个/第二个参数  
        auto bfunc = std::bind(TestBind,100,200,std::placeholders::_1,_2);
        bfunc("testing bind",00); 

        //成员函数 转换为普通函数
        Data data;
        auto cfunc = std::bind(&Data::TestFunc,&data,_1);
    }

    {
        Data mydate;
        mydate.Call(" first shot ");

    }
    {
        function<int(string)> func_ptr = Testfunc;
        func_ptr("test");

    }


    return 0;
}