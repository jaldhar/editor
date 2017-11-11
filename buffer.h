// Buffer -- manages text in a text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <vector>

struct BufferInternals {
    size_t    _capacity;
    ptrdiff_t _point;
    size_t    _size;
    ptrdiff_t _gapStart;
    ptrdiff_t _gapEnd;
};

template<typename T, typename T_nonconst, typename elem_type = typename T::value_type>
class BufferIterator;

template<typename T,  std::size_t N, typename Container = std::vector<T>>
class Buffer {
public:
    using self_type = Buffer<T, N, Container>;
    using container_iterator = typename Container::iterator;
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = BufferIterator<self_type, self_type>;
    using const_iterator = BufferIterator<const self_type, self_type, const value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Buffer();
    Buffer(const self_type& that);
    Buffer(self_type&& that);

    Buffer<T, N, Container>& operator=(const self_type& that);
    Buffer<T, N, Container>& operator=(self_type&& that);
    bool       operator==(const self_type& that) const;
    bool       operator!=(const self_type& that) const;
    reference  operator[](size_type n);

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    size_type capacity() const;
    bool      empty() const;
    size_type max_size() const;
    size_type size() const;
    size_type front() const;
    size_type back() const;

    bool            deletePrevious();
    bool            deleteNext();
    bool            insert(value_type c);
    difference_type point() const;
    bool            pointSet(difference_type n);
    bool            pointMove(int count);


    BufferInternals internals();

private:
    friend iterator;
    friend const_iterator;

    Container                    _text;
    difference_type              _point;
    container_iterator           _gapStart;
    container_iterator           _gapEnd;

    void                         moveGap();
    container_iterator           userToGap(difference_type p);
    difference_type              gapToUser(container_iterator i);
};

template<typename T,  std::size_t N, typename Container = std::vector<T>>
void std::swap(Buffer<T, N, Container>& lhs, Buffer<T, N, Container>& rhs);

template<typename T, typename T_nonconst, typename elem_type>
class BufferIterator {
public:
    // Iterator traits, previously from std::iterator.
    using self_type = BufferIterator<T, T_nonconst, elem_type>;
    using buffer_type       = T;
    using value_type        = typename buffer_type::value_type;
    using size_type         = typename buffer_type::size_type;
    using difference_type   = typename buffer_type::difference_type;
    using pointer           = typename buffer_type::container_iterator;
    using const_pointer     = const pointer;
    using reference         = typename buffer_type::reference;
    using const_reference   = const reference;
    using iterator_category = std::random_access_iterator_tag;

    BufferIterator();
    BufferIterator(const T* buffer);
    BufferIterator(const self_type& that);

    self_type&          operator=(const self_type& that);
    bool                operator==(const self_type& that);
    bool                operator!=(const self_type& that);
    self_type&          operator+=(const difference_type& n);
    self_type           operator+(const difference_type& n);
    self_type&          operator++();
    self_type           operator++(int);
    self_type&          operator-=(const difference_type& n);
    self_type           operator-(const difference_type& n);
    self_type&          operator--();
    const self_type&    operator--(int);
    reference           operator*() const;
    pointer             operator->() const;
    reference           operator[](const size_type& n) const;

private:
    const buffer_type*  _buffer;
    pointer             _pos;

};

template<typename T, typename T_nonconst, typename elem_type = typename T::value_type>
bool std::operator<(const BufferIterator<T, T_nonconst, elem_type>& lhs,
const BufferIterator<T, T_nonconst, elem_type>& rhs);

template<typename T, typename T_nonconst, typename elem_type = typename T::value_type>
bool std::operator>(const BufferIterator<T, T_nonconst, elem_type>& lhs,
const BufferIterator<T, T_nonconst, elem_type>& rhs);

template<typename T, typename T_nonconst, typename elem_type = typename T::value_type>
bool std::operator<=(const BufferIterator<T, T_nonconst, elem_type>& lhs,
const BufferIterator<T, T_nonconst, elem_type>& rhs);

template<typename T, typename T_nonconst, typename elem_type = typename T::value_type>
bool std::operator>=(const BufferIterator<T, T_nonconst, elem_type>& lhs,
const BufferIterator<T, T_nonconst, elem_type>& rhs);

template<typename T, typename T_nonconst, typename elem_type = typename T::value_type>
typename BufferIterator<T, T_nonconst, elem_type>::difference_type
std::operator+(const BufferIterator<T, T_nonconst, elem_type>& lhs,
const BufferIterator<T, T_nonconst, elem_type>& rhs);

template<typename T, typename T_nonconst, typename elem_type = typename T::value_type>
typename BufferIterator<T, T_nonconst, elem_type>::difference_type
std::operator-(const BufferIterator<T, T_nonconst, elem_type>& lhs,
const BufferIterator<T, T_nonconst, elem_type>& rhs);

template<typename T, typename T_nonconst, typename elem_type = typename T::value_type>
void std::swap(BufferIterator<T, T_nonconst, elem_type>& lhs,
BufferIterator<T, T_nonconst, elem_type>& rhs);

#include "buffer.inl"

#endif
