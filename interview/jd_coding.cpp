#include<iostream>
#include<vector>
using namespace std;



int sum = 0;
int cnt = 0;
int minCnt = INT_MAX; 
// int ans = -1; 

void recurse(vector<int> &vec, int idx){
    if(sum >= target){
        if(sum == target && minCnt > cnt){
            minCnt = cnt; 
        }
    }

    for(int i = idx; i < n;++i){
        sum += vec[i];
        cnt++;
        recurse(vec,idx+1);
        cnt--;
        sum -= vec[i];
    }
}

int main(){
    
    // recurse(arr,0);
    // cout<<minCnt; 
    // if(minCnt == INT_MAX){
    //     cout<<-1<<endl; 
    // }else{
    //     cout<<minCnt<<endl; 
    // }
    
    int n = 4, target = 57;
    vector<int> vec = {1,2,5,10};

    vector<int> dp(target+1,INT_MAX);
    dp[0] = 0;
    for(int i = 0; i <n;++i){
        for(int j = 0; j <= target;++j){
            // 这里逻辑有问题 ！ 
            if(vec[i] >= j){
                 dp[j] = min(dp[vec[i]-j]+1,dp[j]);
            }
        }
    }

    // for(int i = 1;i <= target;i++){
    //     for(int j = 0; j < n;j++){
    //         if(vec[j] >= i){
    //             dp[i] = min(dp[vec[j] - i]+1,dp[i]); 
    //         }
    //     }
    // }

    if(dp[target] != INT_MAX){
        cout<<dp[target]<<endl; 
    }else{
        cout<<-1<<endl; 
    }
    // cout<<dp[target]<<endl; 
    // cin>>n;
}