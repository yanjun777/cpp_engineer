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

    void construct(T *p, const T &val)
    {
        new (p) T(val);
        return;
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
        if (i < 0 || i >= size()) {
            throw std::out_of_range("index out of range");
        }
        return first_[i];
    }
    const T &operator[](int i) const
    {
        // int capacity = end_ - first_; 
        if (i < 0 || i >= size()) {
            throw std::out_of_range("index out of range");
        }
        return first_[i]; 
    }

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
    void pop_back()
    {
        if (empty())
            return;
        // last_--;
        verify(last_-1,last_ );
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
    class iterator
    {
    public:
        // todo 需要在构造函数添加一个当前容器的地址 first_
        // 必须要传入容器对象！
        iterator(vector<T, Alloc> *pvec = nullptr, T *src = nullptr)
            : pvec_(pvec), ptr_(src)
        {
            iterator_base *itb = new iterator_base(this, pvec_->head_.next_);
            pvec_->head_.next_ = itb;
        }
        // 需要注意两点
        // 1. 迭代器是否有效
        // 2. 逻辑比较的时候 迭代器所指向的对象是否一致！（不仅需要类型，还需要同一个对象）
        bool operator==(const iterator &rhs) const
        {
            // 不同类型容器的迭代器不能相互比较 这里输入参数都不对 难道还能往下运行？
            
            if ( pvec_ == nullptr)
            {
                throw std::runtime_error("iterator invalid! ");
            }
            if ( this->pvec_ != rhs.pvec_)
            {
                throw std::logic_error("iterator incompatible");
            }
            return this->ptr_ == rhs.ptr_;
        }
        bool operator!=(const iterator &rhs) const
        {
            if ( pvec_ == nullptr)
            {
                throw std::runtime_error("iterator invalid! ");
            }
            if ( this->pvec_ != rhs.pvec_)
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
            return iterator(pvec_,ptr_++);
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
    // todo 需要添加一个 verify private函数：用于检查迭代器失效问题
    void verify(T *first, T *last)
    {
        iterator_base *pre = &(this->head_); 
        iterator_base *it = head_.next_; 
        while(it != nullptr){
            // (] //边界问题 
            // ? poanle 包含边界的！ 
            if(it->cur_->ptr_ >= first && it->cur_->ptr_ <= last ){
                // 迭代器失效
                it->cur_->pvec_ = nullptr; 
                // 
                it = it->next_; 
                delete pre->next_; 
                pre->next_ = it; 

            }else{
                pre = it;
                it = it->next_; 
            }
        }
    }
    // insert 还需要判断是否扩容
    iterator insert(iterator it,const T& val)
    {
        // 先判断是否够容量 todo 
        // 判断it.ptr_ 合法性 todo  （实际写代码需要考虑的问题）
        // 检查迭代器 
        verify(it.ptr_,last_); 
        // 将元素后移动 双指针 (尽量原地移动)
            // 我觉得这里可以使用memcpy 或者是移动构造 ， 因为指向的资源是相同的！ 
            // 拷贝构造那里不能直接memcpy 因为对象可能占有资源 
        T* p = last_; 
        while(p != it.ptr_){
            allocator_.construct(p,*(p-1));
            allocator_.destroy(p-1);
            p--; 
        }
        allocator_.construct(p,val);
        //维护last_
        last_++;
        // 返回
        return iterator(this,p); 
    }

    // erase
    iterator erase(iterator it)
    {
        T* p = it.ptr_;
        // 检查迭代器 
        verify(it.ptr_,last_); 
        // 将元素后移动 双指针 (尽量原地移动)
        // 我觉得这里可以使用memcpy 或者是移动构造 ， 因为指向的资源是相同的！ 
        // 拷贝构造那里不能直接memcpy 因为对象可能占有资源 
         
        while(p+1 != this->last_){
            allocator_.destroy(p);
            allocator_.construct(p,*(p+1));
            p++; 
        }
        // 最后也有说法的！注意要删除的元素是谁？ 以及维护last_
        allocator_.destory(p);
        last_--;
        // 返回
        return iterator(this,it.ptr_); 
    }


private:
    T *first_;
    T *last_;
    T *end_;
    Alloc allocator_;
    void expand()
    {
        int size = end_ - first_;
        // 思路优化 应该给 temptr创新空间然后构造
        T *temptr = first_;
        // first_ = new T[size * 2];
        first_ = allocator_.allocate(size * 2);
        for (int i = 0; i < size; i++)
        {
            // first_[i] = temptr[i];
            allocator_.construct(first_ + i, temptr[i]);
        }
        // delete[] temptr;
        // temptr = nullptr;
        for (T *p = temptr; p != last_; p++)
        {
            allocator_.destroy(p);
        }
        allocator_.deallocate(temptr);
        temptr = nullptr;

        end_ = first_ + 2 * size;
        last_ = first_ + size;
    }
    // todo 需要再添加一个迭代器链表结构
    struct iterator_base
    {
        iterator_base(iterator *cur = nullptr, iterator_base *next = nullptr)
            : cur_(cur), next_(next)
        {
        }
        iterator *cur_;
        iterator_base *next_;
    };
    // todo 需要添加一个私有成员  迭代器链表的头节点（变量）
    iterator_base head_; // 构造的时候就传入一个nullptr 不传也可以...

    // pop_back insert erase 需要verify
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
