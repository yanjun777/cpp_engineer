
//内链 inline 发生在编译阶段
// 省略调用开销！ 

#include "func_obj.h"
#include <iostream>

int main(){
    int a = 20,b = 30;
    bool res = myCompare(a,b,myGreater<int>); 
    std::cout<<"res: "<<res<<std::endl;

}