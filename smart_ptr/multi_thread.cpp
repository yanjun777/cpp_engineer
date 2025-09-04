
#include<iostream>
#include <memory>
#include <thread>

class A{
public:
    void testA(){
        std::cout<<"this is  testA"<<std::endl;
    }
};
// 各种智能指针！ 

// 删除器
// function + lambda 函数替代删除器
void myFunc(std::weak_ptr<A> ptr ){
    std::this_thread::sleep_for(std::chrono::seconds(2)); 
    // ptr->testA();
    std::shared_ptr<A> sptr= ptr.lock();
    if(sptr != nullptr){
        sptr->testA();
    }else{
        std::cout<<"resource destroy"<<std::endl;
    }
}

int main(){

   {
        std::shared_ptr<A> ptr(new A);
        std::thread th(myFunc,ptr); 
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        th.detach();
   }



}