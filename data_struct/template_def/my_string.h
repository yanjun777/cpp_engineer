#include<cstring>
#include <ostream>
#include<iostream>

class String{
public:
// src rhs str vec queue 
    String(const char* src = nullptr){
        if(src != nullptr){
            // strlen 统计的是有效字符个数 不包括 \0
            cptr_ = new char[strlen(src)+1];
            strcpy(cptr_,src); 
        }else{
            cptr_ = new char[1];
            cptr_[0] = '\0';
        }
        std::cout<<"const char* src "<<std::endl;
    }
    ~String(){
        delete[] cptr_;
        cptr_ = nullptr; 
        std::cout<<" ~String "<<std::endl;
    }
    String(const String& rhs){
        cptr_ = new char[strlen(rhs.cptr_)+1];
        strcpy(cptr_,rhs.cptr_); 
        std::cout<<"const String& rhs "<<std::endl;
    }
    String(String&& rhs ){
        cptr_ = rhs.cptr_;
        rhs.cptr_ = nullptr; 
        std::cout<<"String&& rhs "<<std::endl;
    }
    

    String& operator=(const String& rhs){
        if(rhs == *this ) return *this; 
        // 先xigou 再拷贝
        delete[] cptr_;
        // 
        cptr_ = new char[strlen(rhs.cptr_)+1];
        strcpy(cptr_,rhs.cptr_); 
        std::cout<<"operator=(const String& rhs) "<<std::endl;
        
        return *this; 
    }

    String& operator=(String&& rhs){
        if(rhs == *this ) return *this; 
        // 先xigou 再拷贝
        delete[] cptr_;
        // 
        cptr_ = rhs.cptr_;
        rhs.cptr_ = nullptr;
        std::cout<<"operator=(String&& rhs) "<<std::endl;
        
        return *this; 
    }

    // const 对象只能调用const 函数
    // == > < != 
    bool operator==(const String& rhs)const {
        return strcmp(cptr_,rhs.cptr_) == 0;; 
    }
    bool operator>(const String& rhs)const{
        return strcmp(cptr_,rhs.cptr_) > 0;
    }
    bool operator<(const String& rhs)const{
        return strcmp(cptr_,rhs.cptr_) < 0;
    }
    char& operator[](int idx){
        return cptr_[idx]; 
    } 
    const char& operator[](int idx)const {
        return cptr_[idx]; 
    } 
    int length()const{
        return strlen(cptr_);
    }
    const char* c_str()const{
        return cptr_; 
    }
    // 迭代器
    class iterator{
    public:
        iterator(char* p = nullptr)
            :p_(p)
        {
            
        }
        bool operator==(const iterator& rhs){
            return rhs.p_ == this->p_; 
        }
        bool operator!=(const iterator& rhs){
            return rhs.p_ != this->p_;
        }
        iterator& operator++(){
            p_++;
            return *this; 
        }
        char operator*(){
            return *(this->p_); 
        }

    private:
        char* p_; 
    };

    iterator begin() { 
        return iterator(cptr_);
    } 
    iterator end() {
        return iterator(cptr_+length());
    }

private:
    char *cptr_;
    // + 操作符  全局 以及 成员
    // 目前遇到一个困惑 将一个引用传递给引用形参会发生什么？ 引用=别名=原来的值   符合左右值即可！ 
    // 都使用 const & 还能用“asd”构造
    friend String operator+(const String& lhs,const String& rhs ); 
    friend std::ostream& operator<<(std::ostream&out, String& src); 
    friend String GetString(String &str);
};

String operator+(const String& lhs, const String& rhs){
    // 字符串拼接   
    // 内存泄漏?? temp 析沟的时候会释放
    String temp; 
    temp.cptr_ = new char[strlen(lhs.cptr_)+strlen(rhs.cptr_)+1]; 
    strcpy(temp.cptr_,lhs.cptr_);
    strcat(temp.cptr_,rhs.cptr_); 
    return temp; 
}

std::ostream& operator<<(std::ostream&out, String& src){
    out<<src.cptr_;
    return out; 
}

String GetString(String &str){
    // String temp; 
    // temp.cptr_ = str.cptr_;
    const char* pstr = str.cptr_; 
    String temp(pstr);
    // 拷贝构造
    return temp;
}