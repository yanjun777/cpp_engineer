

#include<iostream>


template<typename T>
class Queue{
public:
    Queue(){
        front_ = rear_ = new QueueItem(); 
    }
    ~Queue(){
        // 析沟队列 
        QueueItem *cur = front_;
        while(cur){
            front_ = cur->next_; 
            delete cur; 
            cur = front_; 
        }
        // 内存池？析沟 

    }
    void push(const T& val){
        // 尾部插入 更新尾指针
        QueueItem *temp = new QueueItem(val);
        rear_->next_ = temp;
        rear_ = temp;
    }
    void pop(){
        // 头部pop 判空 更新尾指针有必要的化！ 
        if(empty()) return;
        QueueItem * temp = front_->next_; 
        front_->next_ = temp->next_; 
        if(front_->next_ ==nullptr) rear_ = front_; 
        delete temp; 

    }
    T front() const{
        // 判断空？ 没必要了
        return front_->next_->val; 
    } 
    T rear() const{
        // 判断空？ 没必要了
        return rear_->val;
    }
    bool empty() const{
        return front_ == rear_ ; 
    }
    
    
private:
struct QueueItem
    {
        QueueItem(T data = T(),QueueItem *next = nullptr): data_(data),next_(next){} 
        T data_;
        QueueItem* next_; 
        // 内存池？
        // 创建时机： 第一次获取QueueItem
        // 析沟时机： 析沟Queue 之前! 进阶方法使用智能指针 自动进行析沟！
        // 1.new  delete 对象没有创建前使用 static 
        static void* operator new(size_t size){
            if(item_pool_ == nullptr){
                item_pool_ = (QueueItem*)new char[POOLSIZE*sizeof(QueueItem)];
                QueueItem *temp =item_pool_; 
                for(; temp < item_pool_+POOLSIZE-1;temp++){
                    temp->next_ = (temp+1);
                }
                // 
                temp->next_ = nullptr; 

            }
            QueueItem *temp = item_pool_;
            item_pool_ = item_pool_->next_;
            return temp ; 

        } 
        static void operator delete(void *ptr){
            QueueItem *temp = (QueueItem*)ptr;
            // 巧妙！
            temp->next_ = item_pool_;
            item_pool_ = temp ; 

        }

        // 2.item_pool_ SIZE 所有节点公用 static 
        static QueueItem* item_pool_; // 内存池 
        static const int POOLSIZE=100000; // 常量可以类内！
    };
    QueueItem *front_;
    QueueItem *rear_;

};

template<typename T>
typename Queue<T>::QueueItem::QueueItem *Queue<T>::QueueItem::item_pool_= nullptr; 

