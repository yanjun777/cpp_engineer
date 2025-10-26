
// 多线程加


#include<iostream>
#include<thread>
#include<vector>
#include<mutex>
// 多线程jingtai
int total = 0; 
std::mutex mtx; 

int add(int start,int end){
    int ans = 0;
    for(int i = start;i<end;++i){
        ans +=i;
    }
    return ans; 
}

void worker(int start,int end){
    int t_sum = add(start,end); 
    // std::cout<<"worker: "<<std::this_thread::get_id()<<"res:"<<t_sum<<std::endl;
    std::unique_lock<std::mutex> lock(mtx);  
    total += t_sum; 
    // std::cout<<"worker: "<<std::this_thread::get_id()<<"curSum:"<<total<<std::endl;
}



int main(){
    // 边界问题
    int start = 100,end = 300;
    int numWork = 4; 
    int chunk = (end-start)/numWork; 
    // vector 管理线程生命周期
    std::vector<std::thread> ths; 
    for(int i = 0; i< numWork;++i){
        int t_start = start+ i*chunk;
        int t_end = (i==(numWork-1))?end:t_start+chunk;
        // 正确输入区间 
        ths.emplace_back(std::thread(worker,t_start,t_end)); 
    }

    for(auto &th :ths){
        if(th.joinable()){
            th.join(); 
        }
    }
    std::cout<<"res:"<<total<<std::endl;
    return 0 ;
}