//
// Created by Ltpri on 5/8/2022.
//

#ifndef AIRLINEROUTER_STACK_H
#define AIRLINEROUTER_STACK_H

#include <vector>

template<typename T>
class Stack {
private:
    std::vector<T> vector;

public:
    void push(T element) {
        vector.push_back(element);
    }

    T pop() {
        auto top = vector[vector.size() - 1];
        vector.pop_back();
        return top;
    }

    bool empty() {
        return vector.empty();
    }
};


#endif //AIRLINEROUTER_STACK_H
