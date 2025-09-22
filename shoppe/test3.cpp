
#include<iostream>
#include<vector>
#include<sstream>
#include<cctype>
using namespace std;

void backtrack(const vector<int>&nums, int start, vector<int>& cur, vector<vector<int>> &res ){
    res.push_back(cur); 
    for(int i = start; i < nums.size(); i++){
        cur.push_back(nums[i]);
        backtrack(nums, i+1, cur, res); 
        cur.pop_back(); 
    }

}

vector<vector<int>> subsets(vector<int> & nums){
    vector<vector<int>> res;
    vector<int> cur; 
    sort(nums.begin(),nums.end());
    backtrack(nums,0,cur,res); 
    return res; 
}

int main(){
    vector<int> nums;
    string input;
    getline(cin,input);
    
    stringstream ss(input);
    char ch;
    int num;
    ss >> ch;
    while(ss >>ch){
        if(ch == ']') break; 
        if(isdigit(ch)){
            ss.putback(ch);
            ss>> num;
            nums.push_back(num);
        }
    }

    
    vector<vector<int>> res = subsets(nums);

    for(const auto& subset : res){
        for(int i = 0; i < subset.size(); ++i){
            cout<< subset[i];
            if(i < subset.size() -1) cout <<" ";
        }
        cout << endl; 
    }
    cout << endl; 

    return 0 ; 
}