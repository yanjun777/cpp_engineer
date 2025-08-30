
#include<iostream>

using namespace std; 

int main(){

    // malloc 使用方法 前后注意！ 
    int *ptr = (int*) malloc(sizeof(int)*10);
    if(ptr == nullptr) return 0; 

    for(int i=0;i<10;i++){
        ptr[i] =i*20; 
    }

    for(int i=0;i<10;i++){
        cout<<"ptr["<<i<<"] = "<<ptr[i]<<endl; 
    }
    // 每20次 切换一下
    // static int cnt=0;  cnt++;   (cnt/20)%3 

    // new delete 
    

}