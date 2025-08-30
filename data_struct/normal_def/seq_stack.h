class SeqStack{
public:
    SeqStack();
    ~SeqStack();
    void reserve(int size);
    void push(int x); 
    void pop();
    int top(); 
    bool empty();
    bool full();     

private:
    int top_;
    int size_; 
    int *stk_ptr_;
};
