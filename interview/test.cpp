//经纬恒润
#include<iostream> 
#include<vector> 
#include<future> 
#include<queue> 
using namespace std; 

class Test{
public:
    Test(){
        std::cout<<"this is default construct"<<std::endl;
    }  
    // 输入单个参数的时候使用 explicit 
    // 承诺不抛出异常的时候使用 noexcept  
    Test(const Test & rhs){
        // copy elem
        cout<<"this is default copy constr"<<endl;
    } 
    // 这里还写错了！ 
    Test(Test&& rhs){ 
        cout<<"this is move constr"<<endl; 
    }
    ~Test(){
        cout<<"this is default deconstruct"<<endl;
    }
private: 


};

// std::queue<void()> tasks; 
std::queue<std::function<void()>> tasks; 
template<typename F, typename ...Arg>
auto add_task(F &&f, Arg &&... arg) -> std::future<decltype(f(arg...))>{
    using Return_type = decltype(f(arg...));

    auto task = packaged_task<decltype(f(arg...))()>(
        bind(std::forward<F>(f),std::forward<Arg>(arg)...)
    );  

    auto res = task.get_future(); 
}



int main(){
    //  底层扩容的时候具体是怎么扩容的！ 
    // 复制还是移动 ？
    vector<Test> myvec; 
    myvec.push_back(Test ());
    // Test mytest;  
    // 传递临时对象
    Test mytest(Test{});  
    myvec.push_back(mytest); 
    myvec.emplace_back(mytest); 
    myvec.push_back(std::move(mytest)); 
    myvec.emplace_back(); 
}