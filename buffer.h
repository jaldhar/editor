// Buffer -- manages text in a text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <vector>

using BUFFER = std::vector<char>;

struct BufferInternals {
    size_t    _capacity;
    ptrdiff_t _point;
    size_t    _size;
    ptrdiff_t _gapStart;
    ptrdiff_t _gapEnd;
};

template<typename T> class BufferIterator;
class Redisplay;

template<typename T>
class Buffer {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = BufferIterator<T>;
    using const_iterator = BufferIterator<const T>;
    using difference_type = ptrdiff_t;
    using size_type = size_t;

    Buffer();
    Buffer(const Buffer<T>& that);

    Buffer<T>& operator=(const Buffer<T>& that);
    bool       operator==(const Buffer<T>& that) const;
    bool       operator!=(const Buffer<T>& that) const;
    reference  operator[](difference_type n);

    iterator begin();
    iterator end();

    const_iterator cbegin();
    const_iterator cend();

    size_type capacity();
    bool      empty();
    size_type max_size();
    size_type size();

    bool            deletePrevious();
    bool            deleteNext();
    bool            insert(value_type c);
    difference_type point() const;
    bool            pointMove(int count);
    bool            pointSet(BUFFER::iterator loc);

    BufferInternals internals();

private:
    friend iterator;
    friend Redisplay;

    static constexpr size_t BUFFERSIZE = 80;

    BUFFER           _text;
    difference_type  _point;
    BUFFER::iterator _gapStart;
    BUFFER::iterator _gapEnd;

    void             moveGap();
    BUFFER::iterator userToGap(difference_type p);
    difference_type  gapToUser(BUFFER::iterator i);
};

template<typename T>
void std::swap(Buffer<T>& lhs, Buffer<T>& rhs);

template<typename T>
class BufferIterator {
public:
    // Iterator traits, previously from std::iterator.
    using value_type = T;
    using difference_type = typename BUFFER::difference_type;
    using pointer = typename BUFFER::iterator;
    using reference = typename Buffer<T>::reference;
    using iterator_category = std::random_access_iterator_tag;

    BufferIterator();
    BufferIterator(Buffer<T>& b);
    BufferIterator(const BufferIterator<T>& that);

    BufferIterator<T>&       operator=(const BufferIterator<T>& that);
    bool                     operator==(const BufferIterator<T>& that);
    bool                     operator!=(const BufferIterator<T>& that);
    BufferIterator<T>&       operator+=(const difference_type& n);
    BufferIterator<T>        operator+(const difference_type& n);
    BufferIterator<T>&       operator++();
    BufferIterator<T>        operator++(int);
    BufferIterator<T>&       operator-=(const difference_type& n);
    BufferIterator<T>        operator-(const difference_type& n);
    BufferIterator<T>&       operator--();
    const BufferIterator<T>& operator--(int);
    reference                operator*() const;
    pointer                  operator->() const;
    reference                operator[](const difference_type& n) const;

private:
    Buffer<T>& _b;
    pointer    _i;

};

template<typename T>
bool std::operator<(const BufferIterator<T>& lhs, const BufferIterator<T>& rhs);
template<typename T>
bool std::operator>(const BufferIterator<T>& lhs, const BufferIterator<T>& rhs);
template<typename T>
bool std::operator<=(const BufferIterator<T>& lhs,
const BufferIterator<T>& rhs);
template<typename T>
bool std::operator>=(const BufferIterator<T>& lhs,
const BufferIterator<T>& rhs);
template<typename T>
typename BufferIterator<T>::difference_type std::operator+(
const BufferIterator<T>& lhs, const BufferIterator<T>& rhs);
template<typename T>
typename BufferIterator<T>::difference_type std::operator-(
const BufferIterator<T>& lhs, const BufferIterator<T>& rhs);

template<typename T>
void std::swap(BufferIterator<T>& lhs, BufferIterator<T>& rhs);

#include "buffer.inl"

#endif
