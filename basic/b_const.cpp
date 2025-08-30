
#include"stdio.h"

int main(){
    {
        //常变量 
        int b =10; 
        const int a = b; 

    }
    {
        const int a = 10 ; //立即数
        int *p = (int *)&a;
        *p = 30; 
        //  编译阶段使用常量替换了 
        printf("%d %d %d \n",a,*p,*(&a));
    }

    return 0 ;
}