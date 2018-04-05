#ifndef GUARD_Str_h
#define GUARD_Str_h

#include <algorithm> // copy
#include <cstddef> // size_t
#include <iostream> // istream, ostream
#include <memory> // allocator

#include "Ptr.h"

class Str{

public:
    typedef size_t size_type;

    // CONSTRUCTORS AND DESTRUCTOR
    Str() { create(); }
    explicit Str(size_t n, const char& c = char()) { create(n, c); }
    Str(const char* cp) { create(cp); }
    template <class In> Str(In b, In e) { create(b, e); }
    ~Str() { uncreate(); }

    // OPERATOR OVERLOADS
    char& operator[](size_t i) { return *(data + i); }
    const char& operator[](size_t i) const { return *(data + i); }
    friend std::istream& operator>>(std::istream&, Str&);
    Str& operator+=(const Str& rvalue) {
        append(rvalue);
        return *this;
    }

    // MEMBER FUNCTIONS
    size_t size() { return arraySize; }
    const size_t size() const { return arraySize; }

    char* begin() { return data + 0; }
    const char* begin() const { return data + 0; }
    char* end() { return last + 0; }
    const char* end() const { return last + 0; }

    void clear();

    void append(const char*);
    void append(const Str&);
    void append(const char&);

    char* c_str() const;
    char* dataFunction() const;
    size_t copy(char*, size_t) const;

    operator const void*() const;

    std::istream& getline(std::istream&);

private:
    Ptr<char> data;
    Ptr<char> last;
    Ptr<char> limit;
    size_t arraySize;
    std::allocator< Ptr<char> > alloc;

    void create();
    void create(size_t, const char&);
    void create(const char*);
    template <class In> void create(In, In);
    void uncreate();
    void grow();
    void unchecked_append(const char&);
};

// Supports the constructor that takes two iterators
// Defined in the header due to template linker error issues
template <class In>
void Str::create (In b, In e){
    arraySize = e - b;
    data = alloc.allocate(arraySize);
    last = limit = std::copy(b, e, data);
}
            /*********************
             NON MEMBER FUNCTIONS
             ********************/

// Output nonmember function declaration
std::ostream& operator<<(std::ostream&, const Str&);

// Concatenate nonmember function operator overload declaration
Str& operator+ (const Str&, const Str& );

// Relational, equality, inequality operators
bool operator< (const Str&, const Str&);
bool operator> (const Str&, const Str&);
bool operator== (const Str&, const Str&);
bool operator!= (const Str&, const Str&);

// Predicate to support getline member function
bool isNewLine(char c);


#endif // GUARD_Str_h
