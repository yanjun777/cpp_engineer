

#include<iostream>
#include<vector>
#include<map>
using namespace std ;

// push/insert  emplace_back
class Test
{
public:
    Test(int a ){
        cout<<"Test(int)"<<endl;
    }
    Test(int a,int b ){
        cout<<"Test(int,int)"<<endl;
    }
    Test(const Test& ){
        cout<<"Test(const Test&)"<<endl;
    }
    // 右值拷贝构造 形参定义 
    Test(Test&&){
        cout<<"Test(Test&&)"<<endl;
    }
    ~Test(){
        cout<<"~Test"<<endl; 
    }
};

template<typename T>
struct MyAllocator{
    //allocate deallocate

    // construct destory 

}; 

// template<t>

int main(){
    Test t1(10);
    vector<Test> v;
    v.reserve(100); 
    cout<< "---------------"<<endl;
    // 
    v.push_back(t1); 
    v.emplace_back(t1);
    cout<< "---------------"<<endl;
    // 传递临时对象结果都是一样的 有移动用移动  没移动用拷贝 (不自定义有默认的，直接复制) 
    // 直接插入对象没有区别
    v.push_back(Test(20));
    v.emplace_back(Test(20));
    cout<< "---------------"<<endl;
    // empack_back 传入的参数 与容器存储对象构造函数新参一致 
    v.emplace_back(20);
    v.emplace_back(30,30);
    cout<< "---------------"<<endl; 
    
    map<int,string> m;
    m.insert(make_pair(10,"zhaang san")); // 先构造 再资源转移 
    m.emplace(10,"zhang san"); // 直接map 底层生成  

    return 0; 
}