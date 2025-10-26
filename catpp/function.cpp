#include<functional>
#include<iostream>
using namespace std; 


struct Linear{
    Linear():
    k_(0.f), b_(0.f) 
    {

    }
    Linear(float k, float b):
    k_(k), b_(b) 
    {

    }
    float calculate(float x){
        return k_*x+b_; 
    }
    ~Linear(){

    }
    float k_;
    float b_; 
};

// 函数模板 vs 类模板 函数模板自动类型推演，类模板需要输入类型参数！  
// bind 函数模板 （函数！） 作用是：将参数绑定到函数 场景：线程池中设计任务队列


template<typename Fty>
// 主模版 不需要类型参数列表
class myFunction{

};

//主模板定义不能带模板参数列表 <...> 
// 偏特化！ 必须要提前提供一个主模板
template<typename R,typename ...Args>
class myFunction<R(Args...)>{
public:
    using PFUNC = R(*)(Args...); 
    // 这是提供的函数指针！ 
    // 普通函数 成员函数 ... 
    // 不能接成员函数：R (Class::*)(Args...)  需要再写一个partitial template 
    // 不能接有捕获的lambda 无捕获的会隐式转换为指针！ 
    myFunction(PFUNC pfunc):
    _pfunc(pfunc)
    {

    }
    R operator()(Args ...arg){
        return _pfunc(arg...);
    }
private: 
// 需要接这个类型 是在定义function 类型 写<>的时候！ 
    PFUNC _pfunc; 
};

// 完全特化 直接规定类型
template<>
class myFunction<int(*)(int,int)>{
public:
    int operator()(int a,int b){
        return a+b;
    }
};



int main(){
    // function 接受成员函数<按照函数的返回值和类型声明，但是第一个参数是类的引用>
    // 定义的时候使用 = &YourClass::MemFunc  
    std::function<float(Linear&,float)> func = &Linear::calculate;  
    float x = 4.3f;
    Linear myfunc(2.0f,3.0f);  
    // 使用function 重载的() 运算符，调用calculate 函数！ 
    auto res = func(myfunc,x);
    std::cout<<"calculate: "<<res<<std::endl; 

    // function 去接受一个成员变量
    // 本质上()重载返回的是一个lambda 函数！ 
    std::function<float(Linear&)> memFunc = &Linear::k_; 
    std::cout<<"k_: "<<memFunc(myfunc)<<std::endl; 
    return 0; 
}