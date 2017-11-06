// Buffer -- manages text in a text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _Container_H_
#define _Container_H_

#include <vector>

struct BufferInternals {
    size_t    _capacity;
    ptrdiff_t _point;
    size_t    _size;
    ptrdiff_t _gapStart;
    ptrdiff_t _gapEnd;
};

template<typename T, size_t N, typename Container> class BufferIterator;
class Redisplay;

template<typename T,  std::size_t N, typename Container = std::vector<T>>
class Buffer {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = BufferIterator<T, N, Container>;
    using const_iterator = BufferIterator<const T, N, Container>;
    using difference_type = ptrdiff_t;
    using size_type = size_t;

    Buffer();
    Buffer(const Buffer<T, N, Container>& that);

    Buffer<T, N, Container>& operator=(const Buffer<T, N, Container>& that);
    bool       operator==(const Buffer<T, N, Container>& that) const;
    bool       operator!=(const Buffer<T, N, Container>& that) const;
    reference  operator[](size_type n);

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    size_type capacity() const;
    bool      empty() const;
    size_type max_size() const;
    size_type size() const;

    bool            deletePrevious();
    bool            deleteNext();
    bool            insert(value_type c);
    difference_type point() const;
    bool            pointMove(int count);
    bool            pointSet(typename Container::iterator loc);

    BufferInternals internals();

private:
    friend iterator;
    friend Redisplay;

    Container                    _text;
    difference_type              _point;
    typename Container::iterator _gapStart;
    typename Container::iterator _gapEnd;

    void                         moveGap();
    typename Container::iterator userToGap(difference_type p);
    difference_type              gapToUser(typename Container::iterator i);
};

template<typename T, std::size_t N, typename Container>
void std::swap(Buffer<T, N, Container>& lhs, Buffer<T, N, Container>& rhs);

template<typename T, std::size_t N, typename Container>
class BufferIterator {
public:
    // Iterator traits, previously from std::iterator.
    using value_type = T;
    using difference_type = typename Container::difference_type;
    using pointer = typename Container::iterator;
    using reference = typename Container::reference;
    using iterator_category = std::random_access_iterator_tag;
    using size_type = typename Container::size_type;

    BufferIterator();
    BufferIterator(const Buffer<T, N, Container>& buffer);
    BufferIterator(const BufferIterator<T, N, Container>& that);

    BufferIterator<T, N, Container>&
    operator=(const BufferIterator<T, N, Container>& that);
    bool                operator==(const BufferIterator<T, N, Container>& that);
    bool                operator!=(const BufferIterator<T, N, Container>& that);
    BufferIterator<T, N, Container>&       operator+=(const difference_type& n);
    BufferIterator<T, N, Container>        operator+(const difference_type& n);
    BufferIterator<T, N, Container>&       operator++();
    BufferIterator<T, N, Container>        operator++(int);
    BufferIterator<T, N, Container>&       operator-=(const difference_type& n);
    BufferIterator<T, N, Container>        operator-(const difference_type& n);
    BufferIterator<T, N, Container>&       operator--();
    const BufferIterator<T, N, Container>& operator--(int);
    reference                              operator*() const;
    pointer                                operator->() const;
    reference                              operator[](const size_type& n) const;

private:
    const Buffer<T, N, Container>& _buffer;
    pointer                        _pos;

};

template<typename T, std::size_t N, typename Container>
bool std::operator<(const BufferIterator<T, N, Container>& lhs,
const BufferIterator<T, N, Container>& rhs);

template<typename T, std::size_t N, typename Container>
bool std::operator>(const BufferIterator<T, N, Container>& lhs,
const BufferIterator<T, N, Container>& rhs);

template<typename T, std::size_t N, typename Container>
bool std::operator<=(const BufferIterator<T, N, Container>& lhs,
const BufferIterator<T, N, Container>& rhs);

template<typename T, std::size_t N, typename Container>
bool std::operator>=(const BufferIterator<T, N, Container>& lhs,
const BufferIterator<T, N, Container>& rhs);

template<typename T, std::size_t N, typename Container>
typename BufferIterator<T, N, Container>::difference_type
std::operator+(const BufferIterator<T, N, Container>& lhs,
const BufferIterator<T, N, Container>& rhs);

template<typename T, std::size_t N, typename Container>
typename BufferIterator<T, N, Container>::difference_type
std::operator-(const BufferIterator<T, N, Container>& lhs,
const BufferIterator<T, N, Container>& rhs);

template<typename T, std::size_t N, typename Container>
void std::swap(BufferIterator<T, N, Container>& lhs,
BufferIterator<T, N, Container>& rhs);

#include "buffer.inl"

#endif
