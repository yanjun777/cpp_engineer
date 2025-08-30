

#include "seq_stack.h" 
#include <algorithm>
#include <iostream>


SeqStack::SeqStack():
    top_{-1},
    size_{0},
    stk_ptr_{nullptr} 
{
    // 0 1 2 4 8 动态扩容
    // 拷贝 then 析沟  
    
}
SeqStack::~SeqStack(){
    delete []stk_ptr_;
    stk_ptr_ = nullptr; 
}
void SeqStack::reserve(int size){
    // 初始化状态
    // 非初始状态：
    if(!stk_ptr_){
        size_ = size; 
        stk_ptr_ = new int[size_]; 
    }else{
        int *temp = stk_ptr_;  
        stk_ptr_ = new int[size]; 
        for(int i=0;i<std::min(size,size_);i++){
            stk_ptr_[i] = temp[i]; 
        }

        // 更新top_ size_ 
        top_ = std::min(top_,size-1);
        size_ = size; 
        delete []temp;
        temp = nullptr; 
        //行为将原数组释放 重新分配空间
        // size_ = size;
        // top_ = -1; 
        // delete []stk_ptr_; 
        // stk_ptr_ = new int[size_]; 
    }
}

void SeqStack::push(int x){
    // 可以一直push 
    // 首先考虑有没有初始化 && 有没有满 
    if(stk_ptr_ == nullptr){
        reserve(1); 
    }
    if(full()){
        //先扩容 再插入
        reserve(size_*2);
        stk_ptr_[top_+1] = x;
        top_++;
    }else{
        // 直接插入
        stk_ptr_[top_+1] = x;
        top_++;
    }
    return ; 
}

void SeqStack::pop(){
    // 判断 非空
    if(empty()) return; 
    top_ --; 
}
int SeqStack::top(){
    // 判断已经初始 && 非空 
    if(!stk_ptr_ || top_ == -1) return -1; 
    return stk_ptr_[top_];
} 
bool SeqStack::empty(){
    return top_ == -1;
}
bool SeqStack::full(){
    return top_ ==  size_-1; 
}     


int main(){

    SeqStack my_st; 
    my_st.push(0);
    my_st.push(0); 
    std::cout<<"hello-world"<<std::endl;

    return 0; 
}