// Buffer -- manages text in a text editor
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <type_traits>
#include <vector>

struct BufferInternals {
    size_t    _capacity;
    ptrdiff_t _point;
    size_t    _size;
    ptrdiff_t _gapStart;
    ptrdiff_t _gapEnd;
};

template<typename T, typename T_nonconst = std::remove_cv_t<T>,
    typename elem_type = typename T::value_type> class BufferIterator;

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
    using iterator = BufferIterator<self_type>;
    using const_iterator =
        BufferIterator<const self_type, self_type, const value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static const size_type npos = -1;

    Buffer() : _text(N, 0), _point{0}, _gapStart{_text.begin()},
    _gapEnd{_text.end()} {
    }

    Buffer(const self_type& that) : _text(that._text),
    _point{that._point}, _gapStart{that._gapStart}, _gapEnd{that._gapEnd} {
    }

    Buffer(self_type&& that) : _text(std::move(that._text)),
    _point{that._point},_gapStart{std::move(that._gapStart)},
    _gapEnd{std::move(that._gapEnd)} {
    }

    self_type& operator=(const self_type& that) {
        if (this != &that) {
            this->_text = that._text;
            this->_point = that._point;
            this->_gapStart = that._gapStart;
            this->_gapEnd = that._gapEnd;
        }
        return *this;
    }

    self_type& operator=(self_type&& that) {
        if (this != &that) {
            this->_text = std::move(that._text);
            this->_point = that._point;
            this->_gapStart = std::move(that._gapStart);
            this->_gapEnd = std::move(that._gapEnd);
        }
        return *this;
    }

    bool operator==(const self_type& that) const {
        return this->_text == that._text &&
            this->_point == that._point &&
            this->_gapStart == that._gapStart &&
            this->_gapEnd == that._gapEnd;
    }

    bool operator!=(const self_type& that) const {
        return !operator==(that);
    }

    reference operator[](size_type n) {
        return iterator(this)[n];
    }

    iterator begin() {
        return iterator(this);
    }

    iterator end() {
        return iterator(this) +  size();
    }

    const_iterator begin() const {
        return const_iterator(this);
    }

    const_iterator end() const {
        return const_iterator(this) +  size();
    }

    const_iterator cbegin() const {
        return begin();
    }

    const_iterator cend() const {
        return end();
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const {
        return rbegin();
    }

    const_reverse_iterator crend() const {
        return rend();
    }

    size_type capacity() const {
        return _text.capacity();
    }

    bool empty() const {
        return !size();
    }

    size_type max_size() const {
        return _text.max_size();
    }

    size_type size() const {
        return _text.capacity() - (_gapEnd - _gapStart);
    }

    size_type front() const {
        return 0;
    }

    size_type back() const {
        return (size() == 0) ? 0 : size() - 1;
    }

    friend void std::swap(self_type& lhs, self_type& rhs) {
        if (lhs != rhs) {
            lhs._text.swap(rhs._text);
            std::swap(lhs._point, rhs._point);
            std::swap(lhs._gapStart, rhs._gapStart);
            std::swap(lhs._gapEnd, rhs._gapEnd);
        }
    }

    bool deletePrevious() {
        if (_point <= 0 || _point > size()) {
            return false;
        }

        moveGap();
        _gapStart--;
        return pointMove(-1);
    }

    bool deleteNext() {
        if (_point < 0 || _point >= size()) {
            return false;
        }

        moveGap();
        _gapEnd++;
        return true;
    }

    bool insert(value_type c) {
        if (_point < 0 || _point > size()) {
            return false;
        }

        moveGap();
        *_gapStart = c;
        _gapStart++;
        return pointMove(1);
    }

    difference_type point() const {
        return _point;
    }

    bool pointSet(size_type n) {
        _point = n;
        return true;
    }

    bool pointMove(int count) {
        size_type loc = _point + count;
        if (loc < 0 || loc > size()) {
            return false;
        }

        _point = loc;
        return true;
    }

    size_type searchBackward(value_type c, size_type pos) const {
        auto i = cbegin() + pos;

        while (i > cbegin()) {
            if (*i == c) {
                return i.pos();
            }
            --i;
        }

        return npos;
    }

    size_type searchForward(value_type c, size_type pos) const {
        auto i = cbegin() + pos;

        while (i <= cend()) {
            if (*i == c) {
                return i.pos();
            }
            ++i;
        }

        return npos;
    }

    BufferInternals internals() {
        return {
            capacity(),
            distance(_text.begin(), userToGap(_point)),
            size(),
            distance(_text.begin(), _gapStart),
            distance(_text.begin(), _gapEnd)
        };
    }

private:
    friend iterator;
    friend const_iterator;

    Container                    _text;
    size_type                    _point;
    container_iterator           _gapStart;
    container_iterator           _gapEnd;

    void moveGap() {
        if (_gapStart == _gapEnd) {
            _text.resize(_text.capacity() + N, 0);
            _text.shrink_to_fit();
            _gapStart = _text.end() - N;
            _gapEnd = _text.end();
        }

        container_iterator p = userToGap(_point);
        if (p == _gapStart) {
            return;
        }

        difference_type n;
        if (_gapStart < p) { // point is after gapStart
            n = p - _gapEnd;
            copy(p - n , p, _gapStart);
            _gapStart += n;
            _gapEnd += n;
            _point = gapToUser(_gapStart);
        } else { // point is before _gapStart
            n = _gapStart - p;
            _gapStart -= n;
            _gapEnd -= n;
            copy(p, p + n, _gapEnd);
        }
    }

    container_iterator userToGap(size_type p) {
        container_iterator i = _text.begin() + p;

        if (i >= _gapStart) {
            i += (_gapEnd - _gapStart);
        }

        return i;
    }

    size_type gapToUser(container_iterator i) {
        difference_type p = distance(_text.begin(), i);

        if (i >= _gapEnd) {
            p -= (_gapEnd - _gapStart);
        }

        if (p < 0) {
            p = 0;
        }

        return static_cast<size_type>(p);
    }
};


template<typename T, typename T_nonconst, typename elem_type>
class BufferIterator {
public:
    // Iterator traits, previously from std::iterator.
    using self_type         = BufferIterator<T, T_nonconst, elem_type>;
    using buffer_type       = T;
    using value_type        = typename buffer_type::value_type;
    using size_type         = typename buffer_type::size_type;
    using difference_type   = typename buffer_type::difference_type;
    using pointer           = typename buffer_type::container_iterator;
    using const_pointer     = const pointer;
    using reference         = typename buffer_type::reference;
    using const_reference   = const reference;
    using iterator_category = std::random_access_iterator_tag;

    BufferIterator() : _buffer{nullptr}, _pos{nullptr} {
    }

    BufferIterator(const T* buffer) : _buffer{buffer} {
        auto b = const_cast<T_nonconst *>(_buffer);

        if (b->_gapStart == b->_text.begin() && !b->empty()) {
            _pos = b->_gapEnd;
        } else {
            _pos = b->_text.begin();
        }
    }

    BufferIterator(const BufferIterator<T, T_nonconst, elem_type>& that) :
    _buffer(that._buffer), _pos{that._pos} {
    }

    self_type& operator=(const self_type& that) {
        if (this != &that) {
            &this->_buffer = &that._buffer;
            this->_pos = that._pos;
        }
        return *this;
    }

    bool operator==(const self_type& that) {
        return _buffer == that._buffer && _pos == that._pos;
    }

    bool operator!=(const self_type& that) {
        return !this->operator==(that);
    }

    self_type& operator+=(const difference_type& n) {
        if (n >= 0) {
            if (_pos < _buffer->_gapStart && _buffer->_gapStart <= _pos + n){
                _pos += (_buffer->_gapEnd - _buffer->_gapStart);
            }
        } else {
            if (_pos + n <= _buffer->_gapEnd && _buffer->_gapEnd < _pos) {
                _pos -= (_buffer->_gapEnd - _buffer->_gapStart);
            }
        }
        _pos += n;

        return *this;
    }

    self_type operator+(const difference_type& n) {
        return BufferIterator<T, T_nonconst, elem_type>(*this += n);
    }

    self_type& operator++() {
        this->operator+(1);
        return *this;
    }

    self_type operator++(int) {
        self_type tmp(*this);
        operator++();
        return tmp;
    }

    self_type& operator-=(const difference_type& n) {
        return *this += -n;
    }

    self_type operator-(const difference_type& n) {
        return BufferIterator<T, T_nonconst, elem_type>(*this - n);
    }

    self_type& operator--() {
        this->operator-(1);
        return *this;
    }

    const self_type& operator--(int) {
        self_type tmp(*this);
        operator--();
        return tmp;
    }

    reference operator*() const {
        return *_pos;
    }

    pointer operator->() const {
        return _pos;
    }

    reference operator[](const size_type& n) const {
        return *(_pos + n);
    }

    size_type pos() const {
       return const_cast<T_nonconst *>(_buffer)->gapToUser(_pos);
    }

    template<typename U, typename U_nonconst, typename elem>
    friend bool std::operator<(
    const BufferIterator<U, U_nonconst, elem>& lhs,
    const BufferIterator<U, U_nonconst, elem>& rhs);

    template<typename U, typename U_nonconst, typename elem>
    friend bool std::operator>(
    const BufferIterator<U, U_nonconst, elem>& lhs,
    const BufferIterator<U, U_nonconst, elem>& rhs);

    template<typename U, typename U_nonconst, typename elem>
    friend bool std::operator<=(
    const BufferIterator<U, U_nonconst, elem>& lhs,
    const BufferIterator<U, U_nonconst, elem>& rhs);

    template<typename U, typename U_nonconst, typename elem>
    friend bool std::operator>=(
    const BufferIterator<U, U_nonconst, elem>& lhs,
    const BufferIterator<U, U_nonconst, elem>& rhs);

    template<typename U, typename U_nonconst, typename elem>
    friend difference_type std::operator+(
    const BufferIterator<U, U_nonconst, elem>& lhs,
    const BufferIterator<U, U_nonconst, elem>& rhs);

    template<typename U, typename U_nonconst, typename elem>
    friend difference_type std::operator-(
    const BufferIterator<U, U_nonconst, elem>& lhs,
    const BufferIterator<U, U_nonconst, elem>& rhs);

    template<typename U, typename U_nonconst, typename elem>
    friend void std::swap(
    const BufferIterator<U, U_nonconst, elem>& lhs,
    const BufferIterator<U, U_nonconst, elem>& rhs);

private:
    const buffer_type*  _buffer;
    pointer             _pos;

};

namespace std {
    template<typename T, typename T_nonconst, typename elem_type>
    inline bool operator<(const BufferIterator<T, T_nonconst, elem_type>& lhs,
    const BufferIterator<T, T_nonconst, elem_type>& rhs) {
        return lhs._buffer == rhs._buffer && lhs._pos < rhs._pos;
    }

    template<typename T, typename T_nonconst, typename elem_type>
    inline bool operator>(const BufferIterator<T, T_nonconst, elem_type>& lhs,
    const BufferIterator<T, T_nonconst, elem_type>& rhs) {
        return lhs._buffer == rhs._buffer && lhs._pos > rhs._pos;
    }

    template<typename T, typename T_nonconst, typename elem_type>
    inline bool operator<=(const BufferIterator<T, T_nonconst, elem_type>& lhs,
    const BufferIterator<T, T_nonconst, elem_type>& rhs) {
        return !operator>(lhs, rhs);
    }

    template<typename T, typename T_nonconst, typename elem_type>
    inline bool operator>=(const BufferIterator<T, T_nonconst, elem_type>& lhs,
    const BufferIterator<T, T_nonconst, elem_type>& rhs) {
    return !operator<(lhs, rhs);
    }

    template<typename T, typename T_nonconst, typename elem_type>
    inline typename BufferIterator<T, T_nonconst, elem_type>::difference_type
    operator+(
    const BufferIterator<T, T_nonconst, elem_type>& lhs,
    const BufferIterator<T, T_nonconst, elem_type>& rhs) {
        return lhs._pos + rhs._pos;
    }

    template<typename T, typename T_nonconst, typename elem_type>
    inline typename BufferIterator<T, T_nonconst, elem_type>::difference_type
    operator-(
    const BufferIterator<T, T_nonconst, elem_type>& lhs,
    const BufferIterator<T, T_nonconst, elem_type>& rhs) {
    return lhs._pos - rhs._pos;
}

    template<typename T, typename T_nonconst, typename elem_type>
    inline void swap(BufferIterator<T, T_nonconst, elem_type>& lhs,
    BufferIterator<T, T_nonconst, elem_type>& rhs) {
        if (&lhs != &rhs) {
            lhs._buffer->swap(rhs._buffer);
            std::swap(lhs._pos, rhs._pos);
        }
    }
}

#endif
