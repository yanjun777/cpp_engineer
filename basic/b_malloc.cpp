#include <cstdlib>



int main(){

    int *p;
    p = (int*) malloc(sizeof(int)); 
    free(p); 
    return 0;
}