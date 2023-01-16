#include "register.h"

template<typename T>
Register<T>::Register(const char *name) : name(name), data() {

}

template<typename T>
T Register<T>::operator*() const {
    return data;
}

template<typename T>
T & Register<T>::operator*() {
    return data;
}

template<typename T>
Register<T> & Register<T>::operator=(T value) {
    data = value;
    return *this;
}

template<typename T>
Register<T> & Register<T>::operator-=(T value) {
    data -= value;
    return *this;
}

template<typename T>
Register<T> & Register<T>::operator+=(T value) {
    data += value;
    return *this;
}

template<typename T>
T & Register<T>::operator++() {
    ++data;
    return data;
}

template<typename T>
T & Register<T>::operator--() {
    --data;
    return data;
}

template<typename T>
T Register<T>::operator++(int dummy) {
    T before = data;
    ++data;
    return before;
}

template<typename T>
T Register<T>::operator--(int dummy) {
    T before = data;
    --data;
    return before;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const Register<T> &reg) {
    os << "[" << reg.name << "]";
    return os;
}
