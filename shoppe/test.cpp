
#include<iostream>
using namespace std;

int main(){

    string input;
    cin>> input;
    size_t split_pos = input.find(';');
    string bin1 = input.substr(0,split_pos);
    string bin2 = input.substr(split_pos+1); 

    int num1 = stoi(bin1,nullptr,2);
    int num2 = stoi(bin2,nullptr,2); 
    long long product = num1 * num2; 

    if(product == 0){
        cout<<"0"<<endl;
        return 0; 
    }

    string res;
    while(product > 0){
        res = to_string(product % 2) + res; 
        product /= 2;
    }

    cout<<res<<endl; 
    return 0 ; 
}