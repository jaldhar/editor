// Buffer -- manages text in a text editor (Interface and Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <vector>

static constexpr size_t BUFFERSIZE = 80;
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

    Buffer() : _text(BUFFERSIZE, 0), _point{0}, _gapStart{_text.begin()},
    _gapEnd{_text.end()} {
    };

    Buffer(const Buffer<T>& that) : _text(that._text), _point{that._point},
    _gapStart{that._gapStart}, _gapEnd{that._gapEnd} {
    }

    Buffer<T>& operator=(const Buffer<T>& that) {
        if (this != &that) {
            this->_text = that._text;
            this->_point = that._point;
            this->_gapStart = that._gapStart;
            this->_gapEnd = that._gapEnd;
        }
        return *this;
    }

    bool operator==(const Buffer<T>& that) const {
        return this->_text == that._text &&
            this->_point == that._point &&
            this->_gapStart == that._gapStart &&
            this->_gapEnd == that._gapEnd;
    };

    bool operator!=(const Buffer<T>& that) const {
        return !operator==(that);
    }

    reference operator[](difference_type n) {
        return iterator(*this)[n];
    }

    iterator begin() {
        return iterator(*this);
    }

    size_type capacity() {
        return _text.size();
    }

    const_iterator cbegin() {
        return const_cast<const Buffer<T>&>(*this).begin();
    }

    const_iterator cend() {
        return const_cast<const Buffer<T>&>(*this).end();
    }

    iterator end() {
        return iterator(*this) +  size();
    }

    bool deletePrevious() {
        if (_point <= 0 || _point > static_cast<difference_type>(size())) {
            return false;
        }

        moveGap();
        _gapStart--;
        return pointMove(-1);
    }

    bool deleteNext() {
        if (_point < 0 || _point >= static_cast<difference_type>(size())) {
            return false;
        }

        moveGap();
        _gapEnd++;
        return true;
    }

    bool empty() {
        return size() ? false : true;
    }

    bool insert(value_type c) {
        if (_point < 0 || _point > static_cast<difference_type>(size())) {
            return false;
        }

        moveGap();
        *_gapStart = c;
        _gapStart++;
        return pointMove(1);
    }

    size_type max_size() {
        return _text.max_size();
    }

    difference_type point() const {
        return _point;
    }

    bool pointMove(int count) {
        difference_type loc = _point + count;
        if (loc < 0 || loc > static_cast<difference_type>(size())) {
            return false;
        }

        return pointSet(userToGap(loc));
    }

    bool pointSet(BUFFER::iterator loc) {
        if (loc < _text.begin() || loc > _text.end()) {
            return false;
        }

        _point = gapToUser(loc);

        return true;
    }

    size_type size() {
        return _text.size() - (_gapEnd - _gapStart);
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

    friend void swap(Buffer<T>& lhs, Buffer<T>& rhs) {
        if (lhs != rhs) {
            lhs._text.swap(rhs._text);
            std::swap(lhs._point, rhs._point);
            std::swap(lhs._gapStart, rhs._gapStart);
            std::swap(lhs._gapEnd, rhs._gapEnd);
        }
    }

private:
    friend iterator;
    friend Redisplay;

    BUFFER           _text;
    difference_type  _point;
    BUFFER::iterator _gapStart;
    BUFFER::iterator _gapEnd;

    void moveGap() {
        BUFFER::iterator p = userToGap(_point);
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

    BUFFER::iterator userToGap(difference_type p) {
        BUFFER::iterator i = _text.begin() + p;

        if (i > _gapStart) {
            i += (_gapEnd - _gapStart);
        }

        return i;
    }

    difference_type gapToUser(BUFFER::iterator i) {
        difference_type p = distance(_text.begin(), i);

        if (i > _gapEnd) {
            p -= (_gapEnd - _gapStart);
        }

        return p;
    }
};

template<typename T>
class BufferIterator {
public:
    // Iterator traits, previously from std::iterator.
    using value_type = T;
    using difference_type = typename BUFFER::difference_type;
    using pointer = typename BUFFER::iterator;
    using reference = typename Buffer<T>::reference;
    using iterator_category = std::random_access_iterator_tag;

    BufferIterator() : _b{nullptr}, _i{nullptr} {
    }

    BufferIterator(Buffer<T>& b) : _b{b} {
        if (b._gapStart == b._text.begin() && _b.size()) {
            _i = b._gapEnd;
        } else {
            _i = b._text.begin();
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

    BufferIterator<T>& operator+=(const difference_type& n) {
        auto count = n;

        if (count >= 0) {
            while (count--) {
                ++_i;
                if (_i == _b._gapStart) {
                    _i += (_b._gapEnd - _b._gapStart);
                }
            }
        } else {
            while (count++) {
                --_i;
                if (_i == _b._gapEnd) {
                    _i -= (_b._gapEnd - _b._gapStart);
                }
            }
        }

        return *this;
    }

    BufferIterator<T> operator+(const difference_type& n) {
        return BufferIterator<T>(*this += n);
    }

    friend difference_type operator+(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return lhs._i + rhs._i;
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

    BufferIterator<T>& operator-=(const difference_type& n) {
        return *this += -n;
    }

    BufferIterator<T> operator-(const difference_type& n) {
        return BufferIterator<T>(*this - n);
    }

    friend difference_type operator-(const BufferIterator<T>& lhs,
    const BufferIterator<T>& rhs) {
        return lhs._i - rhs._i;
    }

    BufferIterator<T>& operator--() {
        this->operator-(1);
        return *this;
    }

    const BufferIterator<T>& operator--(int) {
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
        return *(_i + n);
    }

    friend void swap(BufferIterator<T>& lhs, BufferIterator<T>& rhs) {
        if (lhs != rhs) {
            lhs._b.swap(rhs._b);
            std::swap(lhs._i, rhs._i);
        }
    }

private:
    Buffer<T>& _b;
    pointer    _i;

};

#endif
