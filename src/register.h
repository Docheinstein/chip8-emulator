#ifndef REGISTER_H
#define REGISTER_H

#include <iostream>

template<typename T>
class Register {
public:
    explicit Register(const char *name = nullptr);
    T &operator*();
    T operator*() const;
    Register &operator=(T value);
    Register &operator-=(T value);
    Register &operator+=(T value);
    T &operator++();
    T &operator--();
    T operator++(int dummy);
    T operator--(int dummy);

    template<typename TT>
    friend std::ostream & operator<<(std::ostream &os, const Register<TT> &reg);
private:
    const char *name;
    T data;
};

#include "register.tpp"

#endif // REGISTER_H