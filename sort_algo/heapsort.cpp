#include<iostream>
#include<vector>
using namespace std; 

void heapify(vector<int> &nums, int n, int i){
    // 因为实际存储是用0 1 2 3 4 存储
    int largest = i;
    int lson = 2*i+1;
    int rson = 2*i+2;

    if(lson < n && nums[lson] > nums[largest]) largest = lson; 
    if(rson < n && nums[rson] > nums[largest]) largest = rson; 
    if(largest != i){
        swap(nums[largest],nums[i]);
        heapify(nums,n,largest); 
    }

}

void heap_sort(vector<int> &nums){
    int n = nums.size(); 
    //建堆
    for(int i = n/2-1;i>=0;--i){
        heapify(nums,n,i);
    }
    //排序
    for(int i = n-1; i>=0; --i){
        swap(nums[i],nums[0]);
        heapify(nums,i,0); 
    }

}

int main(){
    vector<int> nums= {1,2,43,4,1,41,24,12};
    heap_sort(nums);
    for(auto num:nums){
        cout<<num<<' '; 
    }
    cout<<endl;
    return 0; 

}