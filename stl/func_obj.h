

template<typename T>
bool myGreater(T a,T b){
    return a > b; 
}

template<typename T>
bool myless(T a,T b){
    return a < b; 
}

template<typename T, typename Compare>
bool myCompare(T a,T b,Compare cmp){
    // 如果cmp 是函数指针 编译过程中无法得知具体的函数
    // 如果cmp 是函数对象 编译过程中清晰
    return cmp(a,b);
}