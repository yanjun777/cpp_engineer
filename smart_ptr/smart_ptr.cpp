#include<iostream>

template<typename T>
class RefCnt{
public:
    RefCnt(T* ptr = nullptr):mptr_(ptr){
        if(mptr_ != nullptr){
            mcnt_ = 1;
        }
    }
    ~RefCnt(){

    }
    // add
    int addCnt(){
        return ++mcnt_;
    }
    // sub 
    int subCnt(){
        return --mcnt_;
    }
private:
    T* mptr_;
    int mcnt_;
};

template <typename T>
class Smart_ptr{
public:
    Smart_ptr(T *ptr = nullptr): mptr_(ptr){
        // 如果是空指针则不计数 
        mpRefCnt_ = new RefCnt<T>(mptr_); 
    }
    ~Smart_ptr(){
        // todo 
        if(0 == mpRefCnt_->subCnt()){
            delete mptr_;
            mptr_ = nullptr; 
            delete mpRefCnt_;
            mpRefCnt_ = nullptr;
        }
    }
    // 拷贝 + operatro= 
    Smart_ptr(const Smart_ptr& rhs){
        mptr_ = rhs.mptr_;
        mpRefCnt_ = rhs.mpRefCnt_; 
        // 空就不要加！初始化的时候约定好！
        // 俩构造 + RefCnt 的构造
        if(mptr_){
            mpRefCnt_->addCnt();
        }
    }
    Smart_ptr& operator=(const Smart_ptr& rhs){
        if(&rhs == this) return *this;
        if(0 == mpRefCnt_->subCnt()){
            delete  mptr_; // 差点漏了
            delete mpRefCnt_;
            mpRefCnt_ = nullptr;
        }
        mptr_ = rhs.mptr_;
        mpRefCnt_ = rhs.mpRefCnt_; 
        mpRefCnt_->addCnt();
        return *this; 
    }

    T& operator*(){
        return *mptr_;
    }
    T* operator->(){
        return mptr_;
    }

private:
    T* mptr_; 
    RefCnt<T> *mpRefCnt_; 
};


void test1(){
    int a = 10;
    Smart_ptr<int> my_p = &a;

    class Test{
    public:
        void show(){ std::cout<<"this is show function"<<std::endl;}
    };
    Test t1;
    Smart_ptr<Test> my_p2 = &t1; 
    my_p2->show(); // ptr_show() 我一开始的直觉 
}
void test2(){
    Smart_ptr<int> ptr1 = new int(0);
    Smart_ptr<int> ptr2 = ptr1;
    Smart_ptr<int> ptr3 ;
    ptr3 = ptr2; 
    *ptr3 = 20 ;
    std::cout<<*ptr1<<" "<<*ptr2<<" "<<*ptr3<<std::endl;


}

int main(){
    test2();
 
}