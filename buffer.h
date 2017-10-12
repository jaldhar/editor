// Buffer -- manages text in a text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <array>
#include <iterator>

static constexpr size_t BUFFERSIZE = 80;
using BUFFER = std::array<char, BUFFERSIZE>;

template<typename T> class Buffer;

template<typename T>
class BufferIterator : public
std::iterator<std::output_iterator_tag, T> {
public:
    BufferIterator(Buffer<T>& b, bool end = false);
    BufferIterator(const BufferIterator<T>& that);
    ~BufferIterator()=default;

    BufferIterator<T>&  operator=(const BufferIterator<T>& that);
    bool                operator==(const BufferIterator<T>& that) const;
    bool                operator!=(const BufferIterator<T>& that) const;
    BufferIterator<T>   operator++();
    BufferIterator<T>   operator++(int);
    T                   operator*() const;

    void                swap(BufferIterator<T>& that);

private:
    Buffer<T>& _b;
    bool       _end;
    T*         _i;
};

template<typename T>
class Buffer {
public:
    typedef BufferIterator<T> iterator;

    Buffer();
    bool     operator==(const Buffer<T>& that) const;
    bool     operator!=(const Buffer<T>& that) const;
    size_t   size();
    void     swap(Buffer& that);
private:
    friend iterator;

    BUFFER   _text;
    size_t   _point;
    T*       _offset;
    T*       _gapStart;
    T*       _gapEnd;

    int compareLocations(iterator loc1, iterator loc2);
    size_t locationToCount(iterator loc);
    T* countToLocation(size_t count);

    void moveGap();    
};

template<typename T>
void swap(Buffer<T>& lhs, Buffer<T>& rhs);


#endif