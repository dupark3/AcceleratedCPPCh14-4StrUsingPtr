
#ifndef GUARD_Ptr_h
#define GUARD_Ptr_h

#include <stdexcept> // runtime_error

template <class T>
class Ptr{
public:
    Ptr() : p(0), refptr(new size_t(1)) { }
    Ptr(T* t) : p(t), refptr(new size_t(1)) { }
    Ptr(const Ptr& h) : p(h.p), refptr(h.refptr) { ++*refptr; }
    Ptr& operator=(const Ptr&);
    ~Ptr(){
        if(--*refptr == 0){
            delete p;
            delete refptr;
        }
    }

    T* operator+ (int i){
        return p + i;
    }
    const T* operator+ (int i) const{
        return p + i;
    }

    operator bool() const { return p; }
    T& operator*() const {
        if(p) return *p;
        throw std::runtime_error("uninitialized Ptr");
    }
    T* operator->() const {
        if(p) return p;
        throw std::runtime_error("uninitialized Ptr");
    }

    void make_unique(){
        if(*refptr != 1){
            --*refptr;
            refptr = new size_t(1);
            p = p? clone(p) : 0;
        }
    }
private:
    T* p;
    std::size_t* refptr;
};


    /*******************
    * MEMBER FUNCTIONS *
    ********************/
template <class T>
Ptr<T>& Ptr<T>::operator=(const Ptr<T>& rhs){
    ++*rhs.refptr;
    if(--*refptr == 0){
        delete p;
        delete refptr;
    }
    p = rhs.p;
    refptr = rhs.refptr;
    return *this;
}

template <class T>
T* clone(T* tp) {
    return tp->clone();
}

template <>
char* clone(char* cp) {
    return new char(*cp);
}

#endif // GUARD_Ptr_h

