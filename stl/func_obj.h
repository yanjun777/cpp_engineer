

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


template <typename Iterator, typename Compare>
bool myFindIf(Iterator &it1, Iterator &it2, Compare cmp ){
    return false; 
}
template<typename>
class myFunction;   // 主模板声明

template<typename R, typename... A>
class myFunction<R(A...)>{
public:
    using PFUNC = R(*)(A...); 
    myFunction(PFUNC pfunc)
    : pfunc_(pfunc)
    {/*  */

    }
    R operator()(A... arg){
        return pfunc_(arg...); 
    }

private: 
    PFUNC pfunc_; 
};