
#include<iostream>
#include<vector>
using namespace std;

// O(n2)
void select_sort(vector<int>&nums){
    int n =nums.size();
    
    for(int i = 0; i < n-1; ++i){
        int min_idx = i;
        for(int j = i+1; j < n; ++j){
            if(nums[min_idx] > nums[j]){
                min_idx = j; 
            }
        }
        if(min_idx != i){
            swap(nums[min_idx],nums[i]); 
        }
    }


}

int main(){
    vector<int> nums = {123,43,432,123,334,234,23,3,3,34,2,52,5};
    select_sort(nums);
    for(int num: nums){
        cout<<num<<' ';
    }
    cout<<endl; 

}


//  插入排序  计数排序 
