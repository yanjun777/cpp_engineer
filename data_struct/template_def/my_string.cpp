
#include<iostream>
#include<string>
#include "my_string.h"
using namespace std;

void test1(){
    // ascii 的0 是什么... 
    string str6 = "aaasssddd"; 
    char buf[1024] = { 0 };
    // char buf[1024] = {"1"};// 只初始化 第一个元素

    strcpy(buf,str6.c_str()); 
    for(int i=0;i<10;i++){
        cout<<buf[i]<<" ";
    }
    cout<<endl;


}
void test2()
{

    String s1;
    String s2 = "aaa";
    String s3 = "bbb";
    String s4 = s2 + s3;
    String s5 = s2 + "ccc";
    String s6 = "ddd" + s2;
    cout<< "s6: "<<s6<<endl; 
    if(s5>s6){
        cout<< s5<<" > "<<s6<<endl;
    }else{
        cout<< s5<<" < "<<s6<<endl;
    }
    int len = s5.length(); 
    for(int i=0;i<len;i++){
        cout<<s5[i]<<" ";
    }
    cout<<endl; 

    char buf[1024] = { 0 };
    strcpy(buf,s6.c_str()); 
    for(int i=0;i<10;i++){
        cout<<buf[i]<<" ";
    } 
    cout<<endl;


}
void test3(){

    String str1 = "hello_world";
    // 容器如果是嵌套类型会比较长  可以使用 auto  ！
    String::iterator it = str1.begin(); 
    for(it; it != str1.end(); ++it){
        cout<<*it<<" ";
    }
    cout<<endl; 
    //for each 底层还是通过 迭代器遍历 
    for(char ch : str1){
        cout<<ch<<" ";
    }
    cout<<endl; 
}

int main(){

    // test2();
    test3(); 
}