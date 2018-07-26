#include <iostream>
using namespace std;

template<class T>
class Vector
{
public:
    class Iterator;
    class Constiterator;
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = Iterator;
    using const_iterator = Constiterator;
    
    
private:
    size_t max_sz;
    size_t sz;
    pointer values;
    
public:
    Vector(): max_sz{0}, sz{0}, values{nullptr} {}
    
    Vector(size_t max): max_sz{max}, sz{0}, values{new value_type[max_sz]} {}
    
    Vector(std::initializer_list<T> il){
        values = new value_type[max_sz];
        sz = 0;
        max_sz = il.size();
        for(auto& i: il)
            values[sz++] = i;
    }
    
    Vector& operator=(std::initializer_list<value_type> ilist){
        max_sz = ilist.size();
        sz = 0;
        values = new value_type[max_sz];
        for(auto& i: ilist)
            values[sz++] = i;
        return *this;
    }
    
    
    Vector(const Vector& obj): Vector(obj.max_sz){
        if(max_sz < obj.sz) max_sz = obj.sz;
        values = new value_type[max_sz];
        for(size_t i = 0; i < max_sz; i++)
            values[i] = obj.values[i];
        sz = max_sz;
    }
    
    ~Vector(){
        delete [] values;
    }
    
    Vector& operator=(const Vector& obj) {
        if (this == &obj) return *this;
        delete [] values;
        max_sz = obj.max_sz;
        sz = obj.sz;
        values = new T[obj.sz];
        for(size_t i {0}; i < sz; ++i)
            values[i] = obj.values[i];
        return *this;
    }
    
    bool empty(){
        return !sz;
    }
    
    void clear (){
        sz = 0;
    }
    
    void reserve(size_t new_sz){
        auto temp = new value_type[new_sz];
        for(size_t i=0; i < sz; ++i)
            temp[i] = values[i];
        delete [] values;
        values = temp;
        max_sz  = new_sz;
    }
    
    const size_t size() const {
        return sz;
    }
    
    const T& operator[] (const size_t index ) const {
        if(index >= sz) throw runtime_error("Out of bounds");
        return values[index];
    }
    
    T& operator[] (const size_t index )  {
        if(index >= sz) throw runtime_error("Out of bounds");
        return values[index];
    }
    
    
    void shrink_to_fit(){
        max_sz = sz;
    }
    
    void push_back(const_reference obj){
        if(sz == max_sz) reserve(max_sz*2 + 2);
        values[sz++] = obj;
    }
    
    void pop_back() {
        if(sz>0)
            --sz;
    }
    
    
    
    
    class Iterator : public std::iterator<std::forward_iterator_tag, T>
    {
        pointer ptr;
        Vector* v;
    public:
        
        Iterator(): ptr{nullptr} {}
        Iterator(pointer a, Vector* b): ptr{a}, v{b} {}
        
        
        operator Constiterator()const {
            return Constiterator(ptr, v);
        }
        
        Iterator operator++(int){if(ptr >= (v->end().ptr)) throw runtime_error("Cannot increment any further"); Iterator tmp(*this); operator++(); return tmp;}
        
        Iterator& operator++(){if(ptr >= v->end().ptr) throw runtime_error("Cannot increment any further");  ++ptr; return *this; }
        
        friend difference_type operator-(const Iterator& lop,
                                         const Iterator& rop) {
            return lop.ptr - rop.ptr;}
        
        const T* operator->() {return ptr; }
        
        bool operator==(const Iterator& lop){
            return ptr == lop.ptr;
        }
        bool operator!=(const Iterator& lop){return ptr != lop.ptr;}
        
        T& operator*() {
            if(ptr < v->begin().ptr || ptr >= v->end().ptr)
                throw runtime_error("Out of bounds");
            return *ptr;
        }
        
        
    };
    
    const_iterator begin() const{
        return const_iterator(values, this);
    }
    
    const_iterator end() const{
        return const_iterator(values + sz, this);
    }
    
    iterator begin() {
        return iterator(values, this);
    }
    
    iterator end() {
        return iterator(values + sz, this);
    }
    
    
    class Constiterator : public std::iterator<std::forward_iterator_tag, T>
    {
        pointer ptr;
        const Vector* v;
    public:
        Constiterator(pointer a,const Vector* v): ptr{a}, v{v} {}
        
        Constiterator operator++(int){if(ptr >= (v->end().ptr)) throw runtime_error("Cannot increment any further"); Constiterator tmp(*this); operator++(); return tmp;}
        
        Constiterator& operator++(){if(ptr >= v->end().ptr) throw runtime_error("Cannot increment any further");  ++ptr; return *this; }
        
        friend difference_type operator-(const Constiterator& lop,
                                         const Constiterator& rop) {
            return lop.ptr - rop.ptr;}
        
        const T* operator->() {return ptr; }

        bool operator==(const Constiterator& lop){
            return ptr == lop.ptr;
        }
        bool operator!=(const Constiterator& lop){return ptr != lop.ptr;}
        
        const_reference operator*() {
            if(ptr < v->begin().ptr || ptr >= v->end().ptr)
                throw runtime_error("Out of bounds");
            return *ptr;
        }
        
    };
    
    
    
    Iterator erase(const_iterator pos) {
        auto diff = pos - begin();
        if (diff<0 || static_cast<size_type>(diff)>=sz)
            throw runtime_error("Iterator out of bounds");
        size_type current{static_cast<size_type>(diff)};
        for (size_type i{current}; i<sz-1; ++i)
            values[i]=values[i+1]; --sz;
        return Iterator{values + current, this}; }
    
    
    
    Iterator insert(const_iterator pos, const_reference val) {
        auto diff = pos-begin();
        if (diff<0 || static_cast<size_type>(diff)>sz)
            throw runtime_error("Iterator out of bounds");
        size_type current{static_cast<size_type>(diff)};
        if (sz>=max_sz)
            reserve(max_sz*2+10);
        for (size_type i{sz}; i-->current; )
            values[i+1] = values[i];
        values[current]=val;
        ++sz;
        return Iterator{values+current, this};
    }
};





