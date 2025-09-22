

#include<iostream>


// 饿汉
// 一定是线程安全的！ .data --static  
class Singleton{

    Singleton * getInstance(){
        return &singleton_; 
    }
private:
    static Singleton singleton_; 
    Singleton(){} 
    Singleton(const Singleton &rhs) = delete; 
    Singleton& operator=(const Singleton &rhs) = delete; 
};

Singleton Singleton::singleton_;  


// 懒汉
class SingletonLazy{
public: 
    SingletonLazy* getInstance(){
        // 函数局部变量的初始化 汇编指令添加了线程互斥操作
        static SingletonLazy singletonLazy_;
        return &singletonLazy_; 
    }
private:
    SingletonLazy (){}
    SingletonLazy(const SingletonLazy &rhs) = delete; 
    SingletonLazy& operator=(const SingletonLazy &rhs) = delete; 

};