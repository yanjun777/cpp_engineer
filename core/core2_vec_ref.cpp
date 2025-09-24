#include<iostream>
#include <vector>
using namespace std; 


// 外部传入左值则进行拷贝构造形参v
// 外部传入右值则进行移动构造形参v (vector保存的内容不需要重新构造)
vector<int> TestVector(vector<int> v) {
    cout << "TestVector function called" << endl;
    cout<<"TestVector function data:"<<v.data() << endl;
    cout<<"TestVector function end"  << endl; 
    // 可以返回栈区变量 不会拷贝 进行了优化
    return v;
}

//C++ 无法区分“左值引用”和拷贝 必须要重命名一个函数 
// 只接受左值引用
vector<int> TestVectorRef(vector<int> &v) {
    cout << "TestVectorRef function called" << endl;
    cout<<"TestVectorRef function data"<<v.data()<<endl; 
    cout<<"TestVectorRef function end"  << endl;
    // 可以返回栈区变量 不会拷贝 进行了优化
    return v;
}

int main() {

    {
        // --------------测试2：测试引用传参--------------- 
        vector<int> vec1 = {1, 2, 3, 4, 5};
        cout<<"main :"<<vec1.data() << endl;
        // 传入左值 
        auto rdatas = TestVectorRef(vec1);
        cout<<"main :"<<rdatas.data() << endl; 

        // 只能传入常量或者左值  move会报错
        // auto mdatas =TestVectorRef(move(vec1));
        // auto mdatas =TestVectorRef(const_cast<vector<int>&>(move(vec1)));
        // cout<<"main :"<<mdatas.data() << endl;
    }

    cout<<"----------Test1----------" << endl; 

    {
        // --------------测试1：测试左值和右值传参--------------- 
        vector<int> vec1 = {1, 2, 3, 4, 5};
        cout<<"main :"<<vec1.data() << endl;
        // 传入左值vec1，进行拷贝构造 
        auto rdatas = TestVector(vec1);
        cout<<"main :"<<rdatas.data() << endl; 

        // 传入右值，进行移动构造
        // vector.data() 返回的是一个指向该 vector 内部元素首地址的 原始指针（裸指针）
        auto mdatas =TestVector(move(vec1));
        cout<<"main :"<<mdatas.data() << endl;
    }

    return 0;
}