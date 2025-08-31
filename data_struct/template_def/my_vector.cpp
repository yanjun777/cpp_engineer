#include "my_vector.h"
#include <cstdlib>
#include <iostream>
#include <vector>

#if 0
void testMyVec(){
    vector<int> my_vec(10);  
    for(int i=0;i<10;i++){
        my_vec[i] = (rand() % 100); 
    }
    for(int i=0;i<10;i++){
        std::cout<<"elem "<<i<<" "<<my_vec[i]<<std::endl;
    }
}

void testMyVec_1(){

    vector<Test> my_vec(2);  
    // my_vec.push_back(Test()); 
    Test t;
    my_vec.push_back(t);
    my_vec.pop_back();
    std::cout<<"----for-----"<<std::endl;
    for(int i=0;i<10;i++){
        my_vec.push_back(t);
    }
    std::cout<<"----for-----"<<std::endl;
    std::cout<<"----copy-----"<<std::endl;
    vector<Test>my_vec1 = my_vec;
    std::cout<<"----copy-----"<<std::endl;
    std::cout<<"ending"<<std::endl; 
}
void testMyVec_2(){
    vector<int> vec; 
    for(int i = 0; i < 20; ++i){
        vec.push_back(rand() % 100 + 1);
    }
    vector<int>::iterator it = vec.begin(); 
    for(; it != vec.end(); it++){
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl;

    for(int i = 0; i < 20; ++i){
        std::cout<<vec[i]<<" ";
    }
    std::cout<<std::endl;

}
#endif 

void testMyVec_3(){
    vector<int> vec; // 改为空vector，避免默认构造的0元素
    for(int i = 0; i < 5; ++i){
        vec.push_back(rand() % 100 + 1);
    }
    for(auto elem : vec){
        std::cout<<elem<<" ";
    }
    std::cout<<std::endl;
#if 1
    // 在偶数前插入比偶数小1的数
    std::cout<<"-----------"<<std::endl;
    // 使用索引遍历，避免迭代器失效问题
    for(int i = 0; i < vec.size(); ++i){
        if((vec[i] % 2) == 0){
            vec.insert(vec.begin() + i, vec[i] - 1);
            ++i; // 跳过刚插入的元素
        }
    }
    for(auto elem : vec){
            std::cout<<elem<<" ";
        }
        std::cout<<std::endl;
        std::cout<<"-----------"<<std::endl;
#endif 
    
#if 0
    //删除偶数
    std::cout<<"-----------"<<std::endl;
    auto it_2 = vec.begin();
    while(it_2 != vec.end()){
        if(*it_2 % 2 == 0 ){
            vec.erase(it_2); 
        }else{
            ++it_2; 
        }
    }
    for(auto elem : vec){
            std::cout<<elem<<" ";
        }
    std::cout<<std::endl;
    std::cout<<"-----------"<<std::endl;
#endif 
    
}

void testMyVec_4(){
    vector<int> vec;
    for(int i = 0; i < 5; ++i){
        vec.push_back(rand() % 100 + 1);
    }
    std::cout << "原始vector: ";
    for(auto elem : vec){
        std::cout<<elem<<" ";
    }
    std::cout<<std::endl;
    
    // 使用你原来的逻辑：在偶数前插入比偶数小1的数
    std::cout<<"使用迭代器插入偶数前的数:"<<std::endl;
    auto it_1 = vec.begin();
    for(; it_1 != vec.end(); ++it_1){
        if((*it_1 % 2) == 0){
            it_1 = vec.insert(it_1,(*it_1) - 1); 
            ++it_1; 
        }
    }
    
    std::cout<<"插入后的vector: ";
    for(auto elem : vec){
        std::cout<<elem<<" ";
    }
    std::cout<<std::endl;
}


int main(){
    std::cout << "=== 测试1：使用索引遍历 ===" << std::endl;
    testMyVec_3();
    
    std::cout << "\n=== 测试2：使用迭代器遍历 ===" << std::endl;
    testMyVec_4();

    return 0; 
}