#include<iostream>
#include<vector>
using namespace std; 

// 这个算法还是很多坑的！ 

// 将一个十进制数转换为二进制数
// 注意这里是小于n的质数数量 
int bruteforce(vector<bool>& nums) {
    if (nums.size() < 2)
        return 0;
    nums[0] = false;
    nums[1] = false;
    int cnt = 0;
    int n = nums.size() - 1;
    for (int i = 2; i < n; ++i) {
        for (int j = 2; j * j <= i; ++j) {
            if (i % j == 0) {
                nums[i] = false;
                break;
            }
        }
        if (nums[i]) cnt++;
    }
    return cnt;
}

int era_seive(vector<bool> &nums){
    nums[0] = false;
    nums[1] = false; 
    int n = nums.size()-1; 
    int cnt = 0; 
    for(int i = 2; i <=n; ++i){
        if(nums[i]){
            // i*i 开始！ 
            for(long long j = (long long)i*i; j <=n; j+=i ){
                nums[j] = false; 
            }
            cnt++; 
        }
        
    }
    return cnt; 
}

int main(){

    return 0; 
}