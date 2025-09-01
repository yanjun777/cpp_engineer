// 模板类不能分开声明 实例化的时候才生成代码
// 编译cpp 必须看到完整的定义！
#include <iostream>

// 模板名+类型 才是一个 类型
template <typename T>
struct Allocator
{

    T *allocate(size_t size)
    {
        return (T *)malloc(sizeof(T) * size);
    }
    void deallocate(T *p)
    {
        free(p);
        return;
    }

    /*
    void construct(T *p, const T &val)
    {
        new (p) T(val);
        return;
    }
    void construct(T *p, T&& val){
        new (p) T(std::move(val)); //无论左值还是右值都需要构造！
        return;
    }
    */
   template<typename Ty>
   void construct(T *p,Ty &&val){
        new (p) T(std::forward<Ty>(val));
        return ;
   }

    void destroy(T *p)
    {
        p->~T();
        return;
    }
};

template <typename T = int, typename Alloc = Allocator<T>>
class vector
{
public:
    // Alloc() 和 Allocator<T> 都行吧！
    vector(int size = 10, const Alloc &alloc = Alloc())
        : allocator_(alloc)
    {
        // first_ = new T[size];
        first_ = allocator_.allocate(size);
        last_ = first_;
        end_ = first_ + size;
    }
    ~vector()
    {
        // delete[] first_;
        for (T *p = first_; p != last_; p++)
        {
            allocator_.destroy(p);
        }
        allocator_.deallocate(first_);
        first_ = last_ = end_ = nullptr;
    }

    // 拷贝构造  移动构造
    vector(const vector<T> &rhs)
    {
        int size = rhs.end_ - rhs.first_;
        int len = rhs.last_ - rhs.first_;
        // first_ = new T[size];
        // 默认allocator_ ？
        first_ = allocator_.allocate(size);
        for (int i = 0; i < len; ++i)
        {
            // first_[i] = rhs.first_[i];
            allocator_.construct(first_ + i, rhs.first_[i]);
        }
        last_ = first_ + len;
        end_ = first_ + size;
    }

    // 等号操作符重载
    // 返回 *this
    vector<T> &operator=(const vector<T> &rhs)
    {
        // 在这里如果直接使用 rhs[] 重载[]会发生报错！
        // const 引用无法调用 非const 成员函数
        // 输入指针应该是判断地址是否一致  8.30
        // 输入两个值应该是判断内容是否想等
        if (&rhs == this)
            return *this;

        // delete []first_;
        for (T *p = first_; p != last_; p++)
        {
            allocator_.destroy(p);
        }
        allocator_.deallocate(first_);

        int size = rhs.end_ - rhs.first_;
        int len = rhs.last_ - rhs.first_;
        // first_ = new T[size];
        first_ = allocator_.allocate(size);
        for (int i = 0; i < len; ++i)
        {
            // first_[i] = rhs.first_[i];
            allocator_.construct(first_ + i, rhs.first_[i]);
        }
        last_ = first_ + len;
        end_ = first_ + size;
        return *this;
    }
    T &operator[](int i)
    {
        // 我们可以跑出异常 如果我们不跑出异常 1.可能编译不过 2.可能数组抛出 段错误
        // int capacity = end_ - first_;
        // 如果按照capacity 方式取构建 会导致last_维护失效
        if (i < 0 || i >= size())
        {
            throw std::out_of_range("index out of range");
        }
        return first_[i];
    }
    const T &operator[](int i) const
    {
        // int capacity = end_ - first_;
        if (i < 0 || i >= size())
        {
            throw std::out_of_range("index out of range");
        }
        return first_[i];
    }

    /*
    void push_back(const T &elem)
    {
        if (full())
        {
            expand();
        }
        // 向知道这里引用 和值传递的时候 数组会如何进行元素的构造
        // first_[last_++] = elem;  //错误的！ last 是一个指针！
        // *last_++ =elem ;
        allocator_.construct(last_, elem);
        last_++;
    }
    void push_back(T&& val){
        if(full()){
            expand();
        }
        allocator_.construct(last_,std::move(val));
        last_++; 
    }
    */
   template <typename Ty>
   void push_back(Ty &&val){
        if(full()){
            expand();
        }

        allocator_.construct(last_,std::forward<Ty>(val));
        last_++;
   }




    void pop_back()
    {
        if (empty())
            return;
        // last_--;
        verify(last_ - 1, last_);
        --last_;
        allocator_.destroy(last_);
    }
    T back() const
    {
        return *(last_ - 1);
    }

    bool empty() const
    {
        return first_ == last_;
    }
    bool full() const
    {
        return last_ == end_;
    }
    int size() const
    {
        return last_ - first_;
    }
    
    int capacity() const
    {
        return end_ - first_;
    }
    
    void reserve(int new_cap)
    {
        if (new_cap <= capacity())
            return;
            
        int len = size();
        T *old = first_;
        
        first_ = allocator_.allocate(new_cap);
        for (int i = 0; i < len; ++i)
        {
            allocator_.construct(first_ + i, old[i]);
        }
        for (T *p = old; p != last_; ++p)
            allocator_.destroy(p);
        allocator_.deallocate(old);
        
        last_ = first_ + len;
        end_ = first_ + new_cap;
    }
    
    void clear()
    {
        for (T *p = first_; p != last_; p++)
        {
            allocator_.destroy(p);
        }
        last_ = first_;
    }

    class iterator
    {
    public:
        // todo 需要在构造函数添加一个当前容器的地址 first_
        // 必须要传入容器对象！
        iterator(vector<T, Alloc> *pvec = nullptr, T *src = nullptr)
            : pvec_(pvec), ptr_(src)
        {
        }

        // 需要注意两点
        // 1. 迭代器是否有效
        // 2. 逻辑比较的时候 迭代器所指向的对象是否一致！（不仅需要类型，还需要同一个对象）
        bool operator==(const iterator &rhs) const
        {
            // 不同类型容器的迭代器不能相互比较 这里输入参数都不对 难道还能往下运行？

            if (pvec_ == nullptr)
            {
                throw std::runtime_error("iterator invalid! ");
            }
            if (this->pvec_ != rhs.pvec_)
            {
                throw std::logic_error("iterator incompatible");
            }
            return this->ptr_ == rhs.ptr_;
        }
        bool operator!=(const iterator &rhs) const
        {
            if (pvec_ == nullptr)
            {
                throw std::runtime_error("iterator invalid! ");
            }
            if (this->pvec_ != rhs.pvec_)
            {
                throw std::logic_error("iterator incompatible");
            }
            return this->ptr_ != rhs.ptr_;
        }
        T &operator*()
        {
            if (pvec_ == nullptr)
            {
                throw "invalid iterator";
            }
            return *ptr_;
        }
        const T &operator*() const
        {
            if (pvec_ == nullptr)
            {
                throw std::runtime_error("invalid iterator");
            }
            return *ptr_;
        }
        // 前置加加
        iterator &operator++()
        {
            if (pvec_ == nullptr)
            {
                throw std::runtime_error("invalid iterator");
            }
            ++ptr_;
            return *this;
        }
        iterator operator++(int)
        {
            if (pvec_ == nullptr)
            {
                throw std::runtime_error("invalid iterator");
            }
            return iterator(pvec_, ptr_++);
        }
        
        // 前置递减
        iterator &operator--()
        {
            if (pvec_ == nullptr)
            {
                throw std::runtime_error("invalid iterator");
            }
            --ptr_;
            return *this;
        }
        
        iterator operator--(int)
        {
            if (pvec_ == nullptr)
            {
                throw std::runtime_error("invalid iterator");
            }
            return iterator(pvec_, ptr_--);
        }
        
        // 比较操作符
        bool operator<(const iterator &rhs) const
        {
            if (pvec_ == nullptr || rhs.pvec_ == nullptr)
            {
                throw std::runtime_error("invalid iterator");
            }
            if (this->pvec_ != rhs.pvec_)
            {
                throw std::logic_error("iterator incompatible");
            }
            return this->ptr_ < rhs.ptr_;
        }
        
        bool operator<=(const iterator &rhs) const
        {
            return (*this < rhs) || (*this == rhs);
        }
        
        bool operator>(const iterator &rhs) const
        {
            return !(*this <= rhs);
        }
        
        bool operator>=(const iterator &rhs) const
        {
            return !(*this < rhs);
        }
        
        // 算术运算操作符
        iterator operator+(int n) const
        {
            if (pvec_ == nullptr)
            {
                throw std::runtime_error("invalid iterator");
            }
            return iterator(pvec_, ptr_ + n);
        }
        
        iterator operator-(int n) const
        {
            if (pvec_ == nullptr)
            {
                throw std::runtime_error("invalid iterator");
            }
            return iterator(pvec_, ptr_ - n);
        }
        
        int operator-(const iterator &rhs) const
        {
            if (pvec_ == nullptr || rhs.pvec_ == nullptr)
            {
                throw std::runtime_error("invalid iterator");
            }
            if (this->pvec_ != rhs.pvec_)
            {
                throw std::logic_error("iterator incompatible");
            }
            return this->ptr_ - rhs.ptr_;
        }
        
        // 复合赋值操作符
        iterator &operator+=(int n)
        {
            if (pvec_ == nullptr)
            {
                throw std::runtime_error("invalid iterator");
            }
            ptr_ += n;
            return *this;
        }
        
        iterator &operator-=(int n)
        {
            if (pvec_ == nullptr)
            {
                throw std::runtime_error("invalid iterator");
            }
            ptr_ -= n;
            return *this;
        }

    private:
        T *ptr_;
        // todo 需要添加一个
        vector<T, Alloc> *pvec_;
        friend class vector<T, Alloc>;
    };

    iterator begin()
    {
        return iterator(this, first_);
    }
    iterator end()
    {
        return iterator(this, end_);
    }
    
    // const版本
    const iterator begin() const
    {
        return iterator(const_cast<vector<T, Alloc>*>(this), first_);
    }
    const iterator end() const
    {
        return iterator(const_cast<vector<T, Alloc>*>(this), end_);
    }
    void check_iter(const iterator &it) const
    {
        if (it.pvec_ != this || it.ptr_ < first_ || it.ptr_ > last_)
        {
            throw std::logic_error("iterator incompatible or out of range");
        }
    }

    // insert 还需要判断是否扩容
    iterator insert(iterator it, const T &val)
    {
        check_iter(it); // 只校验传入 it
        
        T *pos = it.ptr_;
        int offset = pos - first_; // 保存相对位置
        
        if (full())
            expand(); // 需要时扩容
        
        // 扩容后需要重新计算pos，因为first_可能已经改变
        pos = first_ + offset;
        
        if (pos == last_)
        {
            allocator_.construct(last_, val);
            ++last_;
            return iterator(this, pos);
        }

        // 1) 在未构造区（last_）placement new 一份尾元素
        allocator_.construct(last_, *(last_ - 1));

        // 2) 已构造区用赋值右移一格
        for (T *p = last_ - 1; p != pos; --p)
        {
            *p = *(p - 1);
        }

        // 3) 覆写插入点
        *pos = val;
        ++last_;
        return iterator(this, pos);
    }

    // erase
    iterator erase(iterator it)
    {
        check_iter(it);
        if (it.ptr_ >= last_)
            throw std::logic_error("erase at end()");

        T *pos = it.ptr_;
        for (T *p = pos; p + 1 != last_; ++p)
        {
            *p = *(p + 1);
        }
        --last_;
        allocator_.destroy(last_);
        return iterator(this, pos);
    }

private:
    T *first_;
    T *last_;
    T *end_;
    Alloc allocator_;
    void expand()
    {
        int cap = end_ - first_;
        int len = last_ - first_;
        T *old = first_;

        int new_cap = cap ? cap * 2 : 1;
        first_ = allocator_.allocate(new_cap);
        for (int i = 0; i < len; ++i)
        {
            allocator_.construct(first_ + i, old[i]); // 只拷贝 len 个
        }
        for (T *p = old; p != last_; ++p)
            allocator_.destroy(p);
        allocator_.deallocate(old);

        end_ = first_ + new_cap;
        last_ = first_ + len;
        
        // 注意：扩容后，所有指向旧内存的迭代器都会失效
        // 这就是为什么在insert中需要保存相对位置的原因
    }
};

class Test
{
public:
    Test()
    {
        std::cout << "Test()" << std::endl;
    }
    Test(const Test &rhs)
    {
        std::cout << "Copy Test()" << std::endl;
    }
    ~Test()
    {
        std::cout << "~Test()" << std::endl;
    }

private:
};
