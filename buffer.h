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
    size_t    _capacity;
    ptrdiff_t _point;
    size_t    _size;
    ptrdiff_t _gapStart;
    ptrdiff_t _gapEnd;
};

template<typename T> class BufferIterator;

template<typename T>
class Buffer {
public:
    using iterator = BufferIterator<T>;

    Buffer() : _text{0}, _point{0}, _count{0},
    _gapStart{_text.begin()}, _gapEnd{_text.end()} {
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
        return *iterator(*this, userToGap(loc));
    }

    iterator begin() {
        return iterator(*this, (size_t)0);
    }

    size_t capacity() {
        return _text.size();
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

    T* point() const {
        return _point;
    }

    bool pointMove(int count) {
        T* loc = userToGap(_point + count);

        return pointSet(loc);
    }

    bool pointSet(T* loc) {
        if (loc < _text.begin() || loc >= _text.end()) {
            return false;
        }

        _point = gapToUser(loc);

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
            capacity(),
            distance(_text.begin(), userToGap(_point)),
            size(),
            distance(_text.begin(), _gapStart),
            distance(_text.begin(), _gapEnd)
        };
    }

private:
    friend iterator;

    BUFFER    _text;
    ptrdiff_t _point;
    size_t    _count;
    T*        _gapStart;
    T*        _gapEnd;

    void moveGap() {
        T* p = userToGap(_point);
        if (p == _gapStart) {
            return;
        }

        size_t n;
        if (_gapStart < p) { // point is after gapStart
            n = p - _gapEnd;
            _gapStart += n;
            _gapEnd += n;
            copy(p - n , p, _gapStart);
            _point = gapToUser(_gapStart);
        } else { // point is before _gapStart
            n = _gapStart - p;
            _gapStart -= n;
            _gapEnd -= n;
            copy(p, p + n, _gapEnd);
        }
    }

    T* userToGap(ptrdiff_t p) {
        T* t;

        t = _text.begin() + p;
        if (t > _gapStart) {
            t += distance(_gapStart, _gapEnd);
        }

        return t;
    }

    ptrdiff_t gapToUser(T* i) {
        ptrdiff_t count = i -_text.begin();
        if (i > _gapEnd) {
            count -= distance(_gapStart, _gapEnd);
        }

        return count;
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

    BufferIterator() : _b{nullptr}, _i{nullptr} {
    }

    BufferIterator(Buffer<T>& b) : _b{b}, _i{_b._text.begin()} {
    }

    BufferIterator(Buffer<T>& b, pointer i) : _b{b}, _i{i} {
    }

    BufferIterator(Buffer<T>& b, size_t count) : _b{b},
    _i{_b._text.begin()} {
        this->operator+=(count);
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

    bool operator==(const BufferIterator<T>& that) {
        return _i == that._i;
    }

    bool operator!=(const BufferIterator<T>& that) {
        return !this->operator==(that);
    }

    friend bool operator<(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return lhs._i < rhs._i;
    }

    friend bool operator>(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return lhs._i > rhs._i;
    }

    friend bool operator<=(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return !operator>(lhs, rhs);
    }

    friend bool operator>=(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return !operator<(lhs, rhs);
    }

    BufferIterator<T>& operator+=(const difference_type& that) {
        _i += that;
        return *this;
    }

    BufferIterator<T> operator+(const difference_type& that) {
        return BufferIterator<T>(this->_b, this->_i + that);
    }

    friend difference_type operator+(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return lhs._i + rhs._i;
    }

    BufferIterator<T>& operator-=(const difference_type& that) {
        _i -= that;
        return *this;
    }

    BufferIterator<T> operator-(const difference_type& that) {
        return BufferIterator<T>(this->_b, this->_i - that);
    }

    friend difference_type operator-(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
            return lhs._i - rhs._i;
    }

    BufferIterator<T>& operator++() {
        this->operator+(1);
        return *this;
    }

    BufferIterator<T> operator++(int) {
        BufferIterator<T> tmp(*this);
        operator++();
        return tmp;
    }

    BufferIterator<T>& operator--() {
        this->operator-(1);
        return *this;
    }

    BufferIterator<T> operator--(int) const {
        BufferIterator<T> tmp(*this);
        operator--();
        return tmp;
    }

    reference operator*() const {
        return *_i;
    }

    pointer operator->() const {
        return _i;
    }

    reference operator[](const difference_type& n) const {
        if (n < 0 || n >= this->_b._count) {
            return nullptr;
        }
        return *BufferIterator<T>(this->_b, this->_b.userToGap(n));
    }

    void swap(BufferIterator<T>& that) {
        if (this != &that) {
            this->_b.swap(that._b);
            std::swap(this->_i, that._i);
        }
    }

private:
    Buffer<T>& _b;
    pointer    _i;

};

template<typename T>
void swap(BufferIterator<T>& lhs, BufferIterator<T>& rhs) {
    lhs.swap(rhs);
}

#endif
