

#include<iostream>
using namespace std; 




int main(){

    auto func1 = []{cout<<"test lambda!"<<endl;};
    func1(); 
    []{cout<<"test lambda directly!"<<endl;}();

    auto func2 = [](int x, int y)-> string{
        string res = "x multiply y equals to ";
        int z = x*y;
        res += to_string(z);
        return res; 
    };
    cout<<func2(10,20)<<endl;
    return 0 ; 
}