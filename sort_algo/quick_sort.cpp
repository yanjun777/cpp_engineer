#include<vector>
#include<iostream>
using namespace std; 

int partition_lomuto(vector<int> &nums,int left, int right){
    // 单一指责
    int pivot = nums[right];
    int i = left,j=left;
    for(;j<right;++j){
        if(nums[j] < pivot){
            swap(nums[j],nums[i]);
            i++;
        }
    }
    swap(nums[i],nums[right]);
    return i; 
}

void qsort_lomuto(vector<int> &nums,int start, int end){
    if(start >= end) return; 
    int pivot = partition_lomuto(nums,start,end);
    qsort_lomuto(nums,start,pivot-1);
    qsort_lomuto(nums,pivot+1,end); 
}

int partition(vector<int> &nums,int left, int right){
    int i = left -1, j= right+1; 
    int pivot = nums[left];
    while(i < j){
        do{i++;} while(nums[i] < pivot);
        do{j--;} while(nums[j] > pivot);
        if(i < j){
            swap(nums[i],nums[j]); 
        }
    }
    return j;  
}

void qsort(vector<int> &nums,int start, int end){
    if(start >= end) return; 
    int j = partition(nums,start,end);
    qsort(nums,start,j);
    qsort(nums,j+1,end); 
}

int main(){
    vector<int> nums = { 5,1,1,2,0,0};

    // bubble_sort(nums);
    // qsort_lomuto(nums,0,nums.size()-1);  
    // heap_sort(nums)
    qsort(nums,0,nums.size()-1); 

    for(int num:nums){
        cout<<num<<' ';
    }
    cout<<endl; 
    return 0 ; 

}