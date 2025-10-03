#include<iostream>
#include<vector>
using namespace std; 


vector<int> merge(vector<int> a, vector<int> b){
    int m = a.size();
    int n = b.size();
    vector<int> res(m+n);
    int i = 0 , j =0, k =0;
    while(i < m && j < n){
        if(a[i] > b[j]){
            res[k++] = b[j++];
        }else{
            res[k++] = a[i++];
        }
    }
    while(i < m){
        res[k++] = a[i++];
    }
    while(j < n){
        res[k++] = a[j++];
    }
    return res; 
}

vector<int> mergevec(vector<vector<int>> &vecs,int left,int right){
    if(left>=right) return vecs[left]; 

    // vector<int> 
    int mid = (left+ right)/2;
    vector<int> lsub = mergevec(vecs,left,mid);
    vector<int> rsub = mergevec(vecs,mid+1,right); 
    return merge(lsub,rsub); 
}

int main(){
    vector<vector<int>> lists_vec = {{1,4,5},{1,3,4},{2,4}};
    vector<int> res = mergevec(lists_vec,0,lists_vec.size()-1);
    for(auto num:res){
        cout<<num<<' ';
    }
    cout<<endl; 
}