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
    size_t _size;
    size_t _point;
    size_t _count;
    size_t _gapStart;
    size_t _gapEnd;
};

template<typename T> class BufferIterator;

template<typename T>
class Buffer {
public:

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

    BufferIterator<T> operator[](size_t loc) {
        return BufferIterator<T>(*this, loc);
    }

    BufferIterator<T> begin() {
        return BufferIterator<T>(*this, _text.begin());
    }

    BufferIterator<T> end() {
        return BufferIterator<T>(*this, _count);
    }

    bool insert(T c) {
        moveGap();
        *_gapStart = c;
        _gapStart++;
        _point++;
        _count++;

        return true;
    }

    BufferIterator<T> point() const {
        return _point;
    }

    bool pointMove(int count) {
        BufferIterator<T> loc(_point);
        loc += count;

        if (loc < begin() || loc >= end()) {
            return false;
        }

        _point = loc;

        return true;
    }

    bool pointSet(BufferIterator<T> loc) {
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
            static_cast<size_t>(_point - begin()),
            _count,
            static_cast<size_t>(_gapStart - begin()),
            static_cast<size_t>(_gapEnd - begin())
        };
    }

private:
    friend BufferIterator<T>;

    BUFFER            _text;
    BufferIterator<T> _point;
    size_t            _count;
    BufferIterator<T> _gapStart;
    BufferIterator<T> _gapEnd;

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

    bool operator==(const BufferIterator<T>& that) const {
        return this->_b._text == that._b._text && this->_i == that._i;
    }

    bool operator!=(const BufferIterator<T>& that) const {
        return !operator==(that);
    }

    bool operator<(const BufferIterator<T>& that) const {
        return this->_b._text == that._b._text && this->_i < that._i;
    }

    bool operator>(const BufferIterator<T>& that) const {
        return this->_b._text == that._b._text && that._i < this->_i;
    }

    bool operator<=(const BufferIterator<T>& that) const {
        return !operator>(that);
    }

    bool operator>=(const BufferIterator<T>& that) const {
        return !operator<(that);
    }

    BufferIterator<T>& operator-=(size_t count) {
        auto& i = *this;
        while(count) {
            i--;
            if (i <= _b._gapEnd) {
                i = _b._gapStart;
            }
        }

        return i;
    }

    friend difference_type operator-=(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        if (lhs._b != rhs._b) {
            return 0;
        }
        return lhs._i - rhs._i;
    }

    BufferIterator<T>& operator-(size_t count) {
        return BufferIterator<T>(*this) -= count;
    }

    friend difference_type operator-(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return lhs -= rhs;
    }

    BufferIterator<T>& operator--() {
        return operator-(1);
    }

    BufferIterator<T> operator--(int) {
        BufferIterator<T> result(*this);
        operator--();
        return result;
    }

    BufferIterator<T>& operator+=(size_t count) {
        auto& i = *this;
        while(count) {
            i++;
            if (i >= _b._gapStart) {
                i = _b._gapEnd;
            }
        }

        return i;
    }

    friend difference_type operator+=(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        if (lhs._b != rhs._b) {
            return 0;
        }
        return lhs._i + rhs._i;
    }

    BufferIterator<T>& operator+(size_t count) {
        return BufferIterator<T>(*this) += count;
    }

    friend difference_type operator+(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return lhs += rhs;
    }

    BufferIterator<T>& operator++() {
        return operator+(1);
    }

    BufferIterator<T> operator++(int) {
        BufferIterator<T> result(*this);
        operator++();
        return result;
    }

    reference operator[](size_t i) {
        return _b[i];
    }

    const reference operator[](size_t i) const {
        return _b[i];
    }

    reference operator*() {
        return *_i;
    }

    size_t pos() {
        size_t count = _i - _b._text.begin();
        if (*this > _b._gapEnd) {
            count -= (_b._gapEnd - _b._gapStart);
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