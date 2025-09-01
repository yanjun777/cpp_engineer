#include "queue_mem_pool.h"


int main(){

    Queue<int> my_que; 
    for(int i=0; i< 100000;i++){
        my_que.push(i);
        my_que.pop();
    }

    return 0;

}