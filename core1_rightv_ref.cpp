#include<iostream>
#include <vector>
using namespace std; 

// 自定义类名大写
// 私有成员变量 _开头
// 私有成员变量接口 成员变量去除_开头
// 普通成员函数首字母大写 
class Data {
public:
    Data(){
        cout << "Data object created" << endl;
    }
    Data(const Data& other) {
        cout << "Data object copied" << endl;
    }
    ~Data() {
        cout << "Data object destroyed" << endl;
    }
};

// 全局自定义函数 首字母大写 
// 右值引用传递参数，函数的参数类型为 vector<Data> &&
// 右值引用可以避免不必要的拷贝，提高性能
// 右值引用的参数可以直接使用 std::move() 转换为右值
void TestData(vector<Data> d){

    cout<< "TestData function called"  << "Size of vector: " << d.size() << endl;
}
int main() {

    vector<Data> dataList(3);
    TestData(move(dataList)); 
    // 注意：在调用 TestData 后，dataList 的内容已经被移动，
    // dataList 现在是一个空的 vector，因为它的资源已经被转移
    cout << "Size of dataList after move: " << dataList.size() << endl;


    return 0;
}