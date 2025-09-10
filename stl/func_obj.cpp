
//内链 inline 发生在编译阶段
// 省略调用开销！ 

#include "func_obj.h"
#include <iostream>

void test(){
    int a = 20,b = 30;
    bool res = myCompare(a,b,myGreater<int>); 
    std::cout<<"res: "<<res<<std::endl;
}
int add(int a,int b){
    return a+b;
}

void test2(){
    myFunction<int(int,int)> ptr1 = add; 
    int res = ptr1(10,20);
    std::cout<<"res:"<<res<<std::endl;
}

void test3(){
    auto func1 = []()->void {
        std::cout<<"hello_world"<<std::endl;
    };
    func1();
}
int main(){
    
    test3();
}