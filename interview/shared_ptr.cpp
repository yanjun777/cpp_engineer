//  quant 线程池没答好挂了，后面有认真看了! 
#include<iostream>

template <typename T>
class shared_ptr{
private:
    T* ptr;
    int* ref_count;

    void release(){
        if(ref_count){
            (*ref_count)--;
            if(*ref_count == 0){
                delete ptr;
                delete ref_count;
                ptr = nullptr;
                ref_count = nullptr; 
            }
        }
    }
public:
    explicit shared_ptr(T*p = nullptr): ptr(p),ref_count(new int(1)){}

    shared_ptr(const shared_ptr& other):ptr(other.ptr), ref_count(other.ref_count){
        if(ref_count){
            (*ref_count)++;
        }
    }

    shared_ptr& operator=(const shared_ptr& other){
        if(this != &other){
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            if(ref_count){
                (*ref_count)++; 
            }
        }
        return *this; 
    }

    ~shared_ptr(){
        release();
    }

    T& operator* () const{return *ptr;}
    T* operator->() const {return ptr;}

    int use_count() const {return ref_count? *ref_count :0;}
    T* get() const{return ptr;}

};
class MyClass{
public:
    MyClass(){std::cout<<"MyClass Construct\n";}
    ~MyClass(){std::cout<<"MyClass DeConstruct\n";}
    void print(){std::cout<<"Hello from MyClass\n"; }

};

int main(){
    shared_ptr<MyClass> ptr1(new MyClass());
    {
        shared_ptr<MyClass> ptr2 = ptr1; 
        ptr1->print();
        ptr2->print(); 
        std::cout<<"counter: "<<ptr1.use_count()<<std::endl;
    }
    std::cout<<"counter: "<<ptr1.use_count()<<std::endl;

    return 0;
}
