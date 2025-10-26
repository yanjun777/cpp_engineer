

// thread 创建线程的方式


// join detach 
// 线程对象的生命周期 
// 线程对象的生命周期如果先于线程函数结束
// 线程对象发出terminate 终止线程执行
// 如果使用了detach 无所谓：线程与线程对象分离。线程后台独立运行，对象销毁后不再能管理它
// ----原理解析---- 
// 线程对象是C++ C++ 程序里的一个控制句柄，用来管理线程

// 普通的main函数 创建线程对象


// 在某个类中启动子线程
// 启动多线程的时候，thread(lambda)
//  public_th_ = thread([this](){
//          执行逻辑
//      })； 
// thread(直接给成员函数)  
//  public_th_ = thread(&MyClass::myMemberfunc，this, myInt_)；
// 确保this 生命周期需要长于线程，这个一般是ok的。  
// thread(bind 绑定) 
//  public_th_ = thread(bind(&MyClass::myMemberfunc,this,myInt))； 
// bind 默认按值传递 std::ref()

// 最原始的方式： 有点忘记了


#include<iostream>
#include<vector>
#include<functional>
#include<thread> 


class Thread{
public:
    Thread(std::function<void()> func):
        th_(func)
    {

    }
    ~Thread(){
        std::cout<<"Thread deconstr"<<std::endl;
    }
    void join(){
        if(th_.joinable()){
            th_.join(); 
        }
    }
private: 

    std::thread th_; 
};


class ThreadPool{
public:
    ThreadPool():
        poolSize_(4),stop_(false)
    {}
    ThreadPool(size_t poolsize):
        poolSize_(poolsize),stop_(false)
    {}
    ~ThreadPool(){
        stop_= true;
        for(int i =0;i<poolSize_;++i){
            pool_[i]->join(); 
            delete pool_[i]; 
        }
        // 对象生命周期结束 vector 析构
        // vector析构时 pool_会释放指针指向内存吗？ 
        // 只会调用指针的析构函数，但是不会释放堆内存需要手动释放！ 
        // 改进使用unique_ptr ! 
    }
    void start(){
        for(int i =0;i<poolSize_;++i){
            pool_.push_back(new Thread(
                std::bind(&ThreadPool::workingFunc,this)
            ));
            // pool_.push_back(new Thread(
            //     [this](){
            //         workingFunc();     
            //     }
            // ));
        }
    }
    void stop(){
        stop_ = true; 
    }
private:
    std::vector<Thread*> pool_;
    size_t poolSize_; 
    void workingFunc(){
        while(!stop_){
            std::cout<<"working thread"<<std::this_thread::get_id()<<std::endl; 
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
        }
    }
    bool stop_; 
};

int main(){

    ThreadPool tp(5);
    tp.start();
    std::this_thread::sleep_for(std::chrono::seconds(4));
    tp.stop();  

}