#include<iostream>
#include<vector>
using namespace std;

// O(n2)
void bubble_sort(vector<int>&nums){
    int n =nums.size();

    for(int i = 0; i < n-1; ++i){
        for(int j = 0; j < n-1-i;++j){
            if(nums[j] > nums[j+1]){
                swap(nums[j],nums[j+1]); 
            }
        }
    }

}

int main(){
    vector<int> nums = {123,43,432,123,334,234,23,3,3,34,2,52,5};
    bubble_sort(nums);
    for(int num: nums){
        cout<<num<<' ';
    }
    cout<<endl; 

}