#include<iostream>
#include<vector>
using namespace std; 

struct ListNode{
    int val;
    ListNode *next;
    ListNode():val(0),next(nullptr){}
    ListNode(int x):val(x),next(nullptr){}
    ListNode(int x, ListNode *y):val(x),next(y){} 
};

ListNode *merge(ListNode *lsub,ListNode *rsub){
        ListNode *dummy = new ListNode(0); 
        ListNode *cur  = dummy;
        while(lsub && rsub){
            if(lsub->val > rsub->val){
                cur->next = rsub;
                rsub = rsub->next;
            }else{
                cur->next = lsub;
                lsub = lsub->next; 
            }
            cur = cur->next; 
        }
        if(lsub){
            cur->next = lsub;
        }
        if(rsub){
            cur->next = rsub ; 
        }
        return dummy->next; 
}

ListNode* recurse(vector<ListNode*>&lists,int left,int right ){
    if(left >= right) return lists[left];

    int mid = (left+right)/2;
    ListNode *lsub =recurse(lists,left,mid);
    ListNode *rsub =recurse(lists,mid+1,right);
    return merge(lsub,rsub); 
} 
ListNode* mergeKLists(vector<ListNode*>& lists) {
    if(lists.size() == 0) return nullptr; 
    return recurse(lists,0,lists.size()-1); 
}
int main(){

    vector<vector<int>> lists_vec = {{1,4,5},{1,3,4},{2,4}};
    int n = lists_vec.size(); 
    vector<ListNode*> lists(n);  
    int idx = 0;
    for(const auto & list : lists_vec){
        ListNode *dummy = new ListNode(0);
        ListNode *temp = nullptr,*cur = dummy; 
        for(auto num:list){
            temp = new ListNode(num);
            cur->next = temp; 
            cur = cur->next; //非常关键！ 
        }
        lists[idx++] = dummy->next; 
    }
    cout<<"my input"<<endl; 
    for(auto list : lists){
        while(list){
            cout<<list->val<<' ';
            list = list->next; 
        }
        cout<<endl; 
    }
    
    cout<<"my result"<<endl; 
    ListNode*merged_list =  mergeKLists(lists);
    while(merged_list){
        cout<<merged_list->val<<' ';
        merged_list = merged_list->next; 
    }
    cout<<endl; 
}