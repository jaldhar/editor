// Buffer -- manages text in a text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <algorithm>
#include <array>
#include <cstdlib>
#include <iterator>

static constexpr size_t BUFFERSIZE = 80;
using BUFFER = std::array<char, BUFFERSIZE>;

struct BufferInternals {
    size_t    _size;
    ptrdiff_t  _point;
    size_t    _count;
    ptrdiff_t _gapStart;
    ptrdiff_t _gapEnd;
};

template<typename T> class BufferIterator;

template<typename T>
class Buffer {
public:
    using iterator = BufferIterator<T>;

    Buffer() : _text{0}, _point{*this, _text.begin()}, _count{0},
    _gapStart{*this, _text.begin()}, _gapEnd{*this, _text.end()} {
    };

    bool operator==(const Buffer& that) const {
        return this->_text == that._text &&
            this->_point == that._point &&
            this->_count == that._count &&
            this->_gapStart == that._gapStart &&
            this->_gapEnd == that._gapEnd;
    };

    bool operator!=(const Buffer& that) const {
        return !operator==(that);
    }

    T& operator[](ptrdiff_t loc) {
        return *iterator(*this, loc);
    }

    iterator begin() {
        return iterator(*this, (size_t)0);
    }

    iterator end() {
        return iterator(*this, _count);
    }

    bool insert(T c) {
        moveGap();
        *_gapStart = c;
        _gapStart++;
        _point++;
        _count++;

        return true;
    }

    iterator point() const {
        return _point;
    }

    bool pointMove(int count) {
        iterator loc(_point + count);

        return pointSet(loc);
    }

    bool pointSet(iterator loc) {
        if (loc < begin() || loc >= end()) {
            return false;
        }

        _point = loc;

        return true;
    }

    size_t size() {
        return _count;
    }

    void swap(Buffer<T>& that) {
        if (this != &that) {
            this->_text.swap(that._text);
            std::swap(this->_point, that._point);
            std::swap(this->_count, that._count);
            std::swap(this->_gapStart, that._gapStart);
            std::swap(this->_gapEnd, that._gapEnd);
        }
    }

    BufferInternals internals()  {
        return {
            size(),
            distance(begin(), _point),
            _count,
            distance(begin(), _gapStart),
            distance(begin(), _gapEnd)
        };
    }

private:
    friend iterator;

    BUFFER   _text;
    iterator _point;
    size_t   _count;
    iterator _gapStart;
    iterator _gapEnd;

    void moveGap() {
        if (_point == _gapStart) {
            return;
        }

        _gapStart = _point;
        size_t n;

        if (_gapStart < _point) { // point is after gapStart
            n = _point - _gapEnd;
            _gapEnd += n;

        } else { // point is before _gapStart
            n = _gapStart - _point;
            _gapEnd -= n;
        }

        copy(_point, _point + n, _gapEnd);
    }
};


template<typename T>
void swap(Buffer<T>& lhs, Buffer<T>& rhs) {
    lhs.swap(rhs);
}


template<typename T>
class BufferIterator {
public:
    // Iterator traits, previously from std::iterator.
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::random_access_iterator_tag;

    BufferIterator(Buffer<T>& b) : _b{b}, _i{_b._text.begin()} {
    }

    BufferIterator(Buffer<T>& b, pointer i) : _b{b}, _i{i} {
    }

    BufferIterator(Buffer<T>& b, size_t count) : _b{b},
    _i{_b._text.begin()} {
        _i += count;

        if (*this > _b._gapStart) {
            *this -= (_b._gapEnd - _b._gapStart);
        }
    }

    BufferIterator(const BufferIterator<T>& that) : _b(that._b),
    _i{that._i} {
    }

    BufferIterator<T>& operator=(const BufferIterator<T>& that) {
        if (this != &that) {
            this->_b = that._b;
            this->_i = that._i;
        }
        return *this;
    }

    friend bool operator==(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return /* lhs._b == rhs._b && */lhs._i == rhs._i;
    }

    friend bool operator!=(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return !lhs.operator==(rhs);
    }

    friend bool operator<(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return /* lhs._b == rhs._b && */ lhs._i < rhs._i;
    }

    friend bool operator>(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return /* lhs._b == rhs._b && */ lhs._i < rhs._i;
    }

    friend bool operator<=(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return operator>(lhs, rhs);
    }

    friend bool operator>=(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return operator<(lhs, rhs);
    }

    BufferIterator<T>& operator+=(const difference_type that) {
        this->_i += that;
        return *this;
    }

    BufferIterator<T> operator+(const difference_type that) {
        return BufferIterator<T>(this->_b, this->_i + that);
    }

    difference_type operator+(const BufferIterator<T>& that) const {
        return this->_i + that._i;
    }

    BufferIterator<T>& operator-=(const difference_type that) {
        this->_i -= that;
        return *this;
    }

    BufferIterator<T> operator-(const difference_type that) {
        return BufferIterator<T>(this->_b, this->_i - that);
    }

    difference_type operator-(const BufferIterator<T>& that) const {
        return this->_i - that._i;
    }

    BufferIterator<T>& operator++() {
        operator+(1);
        return *this;
    }

    BufferIterator<T> operator++(int) {
        BufferIterator<T> tmp(*this);
        operator++();
        return tmp;
    }

    BufferIterator<T>& operator--() {
        operator-(1);
        return *this;
    }

    BufferIterator<T> operator--(int) {
        BufferIterator<T> tmp(*this);
        operator--();
        return tmp;
    }

    reference operator*() {
        return *_i;
    }

    pointer operator->() {
        return _i;
    }

    difference_type pos() {
        difference_type count = _i - _b._text.begin();
        if (*this > _b._gapEnd) {
            count -= distance(_b._gapStart, _b._gapEnd);
        }

        return count;
    }

    void swap(BufferIterator<T>& that) {
        if (this != &that) {
            this->_b.swap(that._b);
            std::swap(this->_i, that._i);
        }
    }

private:
    Buffer<T>& _b;
    T*         _i;
};

template<typename T>
void swap(BufferIterator<T>& lhs, BufferIterator<T>& rhs) {
    lhs.swap(rhs);
}

#endif
