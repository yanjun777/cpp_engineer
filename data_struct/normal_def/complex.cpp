#include<iostream>

class Complex{
public:
    Complex(int real = 0, int complex = 0)
        :real_(real),complex_(complex)
    {
        std::cout<<"Complex normal construct real:"<<real_<<" complex: "<<complex_<<std::endl;
    }
    Complex (const Complex& rhs){
        real_ = rhs.real_;
        complex_ = rhs.complex_;
        std::cout<<"Complex copy construct"<<std::endl;
    }
    // 记得写移动构造：


    ~Complex(){
        std::cout<<"~Complex deconstruct "<<real_<<" complex: "<<complex_<<std::endl;
    }
    // 最好还是引用返回！ 不仅符合语义还高效 
    Complex& operator=(const Complex& rhs){
        // if(*this == rhs){
        //     return *this; 
        // }
        real_ = rhs.real_;
        complex_ = rhs.complex_;
        std::cout<<"Complex operator="<<std::endl;
        return *this; 
    }
    // 优先赵成员方法 没有找全局
    Complex operator+ (const Complex& rhs){
        // RVO 编译器优化 
        Complex temp; 
        std::cout<<"address of temp: "<<&temp<<std::endl; 
        temp.real_ = real_ + rhs.real_; 
        temp.complex_ = complex_ + rhs.complex_;
        return temp; 
        // return Complex(this->real_ + rhs.real_,this->complex_ + rhs.complex_); 
    }
    // ++ 操作符重载 函数栈帧多一次拷贝开销（无论是否接受都会在主调函数生成临时变量）
    // 前置++ 返回值： 先返回当前对象的值 再对其进行++ 
    Complex operator++(int){
        return Complex(real_++,complex_++);
    }
    // 返回引用 开销更小
    // oop 多使用前置++ 
    Complex& operator++(){
        // return Complex(real_++,complex_++);
        real_ += 1 ;
        complex_ += 1;
        return *this; 
    }
    Complex operator--(int){
        return Complex(real_--,complex_--);
    }
    Complex& operator--(){
        real_ --;
        complex_ -- ;
        return *this; 
    }

    void show(){
        std::cout<<"real: "<<real_<<" complex: "<<complex_<<std::endl; 
    }
private:
    int real_ ;
    int complex_ ; 
    //友元函数的声明通常放在类的定义里，可以放在 public:、protected: 或 private: 区域中，效果完全一样
    friend Complex operator+(const Complex& lhs, const Complex& rhs); 
    friend std::ostream& operator<<(std::ostream& out, const Complex& src ); 
    friend std::istream& operator>>(std::istream& in, Complex& src);
};
// 思考一下什么时候可以用引用  什么时候可以用值 
// = 该不该用引用？不应该 链式调用的时候都指向同一对象了 a=b=c;
// + 应不应该？ a+b+c 不是很清楚  
Complex operator+(const Complex& lhs, const Complex& rhs){
    std::cout<<" global complex operator+ "<<std::endl;
    return Complex(lhs.real_+rhs.real_,lhs.complex_+rhs.complex_); 
}
// iostream 输入输出流 
std::ostream& operator<<(std::ostream& out, const Complex& src ){
    out<<"real: "<<src.real_<<" complex_: "<<src.complex_;
    return out; 
}
std::istream& operator>>(std::istream& in, Complex& src){
    std::cin>>src.real_>>src.complex_ ; 
    return in; 
}

int main(){
    Complex c1(10,20);
    Complex c2;
    // Complex c4 = c1 + c2; // 直接构造一次
    Complex c4 = c1 + 20; // 直接构造一次  //20会临时构造一下
    std::cout<<"address of c4: "<<&c4<<std::endl; 
    c4.show();
    std::cout<<"--------------------"<<std::endl;

    Complex c5 = 30 + c1;
    std::cout<<"--------------------"<<std::endl;
    std::cout<<c5<<std::endl;

    Complex c6; 
    std::cin>>c6; 
    std::cout<<"this is c6: "<<c6<<std::endl; 
    // c5++;
    // ++c5;
    // c5.show();
}

