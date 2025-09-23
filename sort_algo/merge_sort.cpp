#include<iostream>
#include<vector>
using namespace std; 
// O(nlogn)
void merge(vector<int> &nums,vector<int> &temp, int left, int mid, int right){
    int i = left, j = mid+1, k= left; 
    while(i<=mid && j <=right){
        if(nums[i] > nums[j]){
            temp[k++] = nums[j++]; 
        }else{
            temp[k++] =nums[i++];
        }
    }
    while(i<=mid){
        temp[k++] = nums[i++];
    }
    while(j<=right){
        temp[k++] = nums[j++]; 
    }
    for(i = left; i <= right;++i){
        nums[i] = temp[i]; 
    }


}

void merge_sort(vector<int>&nums,vector<int> &temp,int left, int right){
    if(left<right){
        int mid = (left+right)/2; 
        merge_sort(nums,temp,left,mid);
        merge_sort(nums,temp,mid+1,right);
        merge(nums,temp,left,mid,right); 
    }
}


vector<int> sortArray(vector<int>& nums) {
    // bubble_sort(nums);
    // qsort_lomuto(nums,0,nums.size()-1);  
    // heap_sort(nums)
    // qsort(nums,0,nums.size()-1); 
    int n = nums.size();
    vector<int> temp(n,0);
    merge_sort(nums,temp,0,n-1); 
    return nums;
}


int main(){

    vector<int> nums = {12,3,3,4,2,44,6,54,34}; 
    int n = nums.size();
    vector<int> temp(n,0);
    merge_sort(nums,temp,0,n-1); 
    for(int i: nums){
        cout<<i<<" "; 
    }
    cout<<endl; 
}