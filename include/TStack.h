#pragma once
#include "Vector.h"

template <typename T>
class TStack
{
    Vector<T> mem;
public:
    TStack():mem(0){}
                    
    TStack(T val) {
        mem.push_back(val);
    }

    size_t stack_size() const noexcept {
        return mem.get_size(); 
    }

    bool empty() const noexcept {
        return mem.get_size() == 0;
    }

    void push(const T& x) {
        mem.push_back(x);
    }

    T top(){
        if (empty())
            throw std::out_of_range("Stack is empty\n");
        return mem[mem.get_size() - 1];
    }

    void pop() {
        if (empty())
            throw std::out_of_range("Stack is empty\n");
        mem.pop_back();
    }
};