#include <algorithm> // copy
#include <cstring> // str_len, strcmp
#include <iterator> // ostream_iterator

#include "Str.h"

// Supports the default constructor
void Str::create(){
    data = last = limit = 0;
    arraySize = 0;
}

// Supports the constructor that takes a size and a char
// If no char provided, value initialized default argument for char
void Str::create(size_t n, const char& c){
    arraySize = n;
    data = alloc.allocate(arraySize);
    last = limit = data + arraySize;
    std::uninitialized_fill(data, last, c);
}

// Supports the constructor that takes a string literal
void Str::create(const char* cp){
    arraySize = std::strlen(cp);
    data = alloc.allocate(arraySize);
    last = limit = data + arraySize;
    std::uninitialized_copy(cp, cp + arraySize, data);
}

// Supports the destructor
void Str::uncreate(){
    if (data){
        clear();
        alloc.deallocate(data, limit - data);
    }
    data = last = limit = 0;
}

// Supports the uncreate() and operator>> function by deconstructing all existing elements
void Str::clear(){
    if (data){
        char* it = last;
        while (it != data)
            alloc.destroy(--it);
    }
    last = data;
}

// Overloaded append functions to take a character, string literal, or class object
void Str::append(const char& c){
    if (last == limit)
        grow();
    unchecked_append(c);
}

void Str::append(const char* c){
    for (size_t i = 0; i != sizeof c; ++i)
        append(*(c + i));
}

void Str::append(const Str& s){
    for (size_t i = 0; i != s.size(); ++i)
        append(*(s.data + i));
}

// Supports the append function
void Str::grow(){
    size_t new_size = std::max(2 * (limit - data), ptrdiff_t(1));
    char* new_data = alloc.allocate(new_size);
    char* new_last = std::uninitialized_copy(data, last, new_data);
    uncreate();
    data = new_data;
    last = new_last;
    limit = data + new_size;
}

// Supports the append function. Only called with enough memory already allocated
void Str::unchecked_append(const char& c){
    alloc.construct(last++, c);
    ++arraySize;
}

// Returns a pointer to a null-terminated array of chars
char* Str::c_str() const{
    // Allocate new memory for deep copy
    char* ret = new char[last - data];

    // Set one past the last element as NULL
    *last = '\0';

    // Copy into newly allocated char array, including the NULL element at "last"
    std::copy(data, last + 1, ret);

    return ret;
}

// Returns a pointer to a non-null terminated array of chars
char* Str::dataFunction() const{
    char* ret = new char[last - data];
    std::copy(data, last, ret);
    return ret;
}

size_t Str::copy(char* destination, size_t numberToCopy) const{
    size_t numberCopied;

    for (size_t i = 0; i != numberToCopy && i != arraySize; ++i){
        *destination++ = *(data + i);
        ++numberCopied;
    }

    return numberCopied;
}

std::istream& Str::getline(std::istream& inputStream){
    if(inputStream){
        clear();
        char c;
        while (inputStream.get(c) && !isNewLine(c))
            append(c);

    }

    return inputStream;
}

bool isNewLine(char c){
    return c == '\n';
}

Str::operator const void*() const{
    return arraySize == 0 ? 0 : this;
}



            /*********************
             NON MEMBER FUNCTIONS
             ********************/

// Output nonmember operator function using ostream_iterator
std::ostream& operator<<(std::ostream& os, const Str& s){
    std::copy(s.begin(), s.end(), std::ostream_iterator<char>(os, "\n"));
    return os;
}

// Input friend function (gains access to private member data for writing)
std::istream& operator>>(std::istream& is, Str& s){
    s.clear();

    // read and discard all leading whitespace
    char c;
    while (is.get(c) && isspace(c))
        ;

    // if something still to read, read and append each char until next whitespace character
    if (is){
        do s.append(c);
        while(is.get(c) && !isspace(c));

        // if we read whitspace, then put it back on the stream
        if (is)
            is.unget();
    }

    return is;
}

// Concatenate nonmember operator overload function
Str& operator+ (const Str& first, const Str& second){
    Str ret = first;
    ret += second;
    return ret;
}


// Relational and equality operators as friend nonmembers
// strcmp returns a negative if lhs < rhs, zero if lhs == rhs, and positive if lhs < rhs
bool operator< (const Str& lhs, const Str& rhs){
    return strcmp(lhs.c_str(), rhs.c_str()) < 0;
}

bool operator> (const Str& lhs, const Str& rhs){
    return strcmp(lhs.c_str(), rhs.c_str()) > 0;
}

bool operator== (const Str& lhs, const Str& rhs){
    return strcmp(lhs.c_str(), rhs.c_str()) == 0;
}

bool operator!= (const Str& lhs, const Str& rhs){
    return strcmp(lhs.c_str(), rhs.c_str()) != 0;
}
