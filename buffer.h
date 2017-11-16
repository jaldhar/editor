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
    std::size_t    _capacity;
    std::ptrdiff_t _point;
    std::size_t    _size;
    std::ptrdiff_t _gapStart;
    std::ptrdiff_t _gapEnd;
};

template<typename T, bool isConst> class BufferIterator;

template<typename T,  std::size_t N, typename Container = std::vector<T>>
class Buffer {
public:
    using self_type = Buffer<T, N, Container>;
    using container_iterator = typename Container::iterator;
    using container_const_iterator = typename Container::const_iterator;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = BufferIterator<self_type, false>;
    using const_iterator = BufferIterator<const self_type, true>;
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

    size_type searchBackward(value_type c, size_type pos) {
        auto i = rend();
        for(i -= (pos + 1); i < rend(); i += 1) {
            if (*i == c) {
                return i.base().pos();
            }
        }
        return npos;
    }

    size_type searchForward(value_type c, size_type pos) {
        auto i = begin();
        for(i += pos; i < end(); i += 1) {
            if (*i == c) {
                return i.pos();
            }
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

        container_const_iterator p = userToGap(_point);
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

    size_type gapToUser(container_const_iterator i) const {
        difference_type p = i - _text.begin();

        if (i >= _gapEnd) {
            p -= (_gapEnd - _gapStart);
        }

        if (p < 0) {
            p = 0;
        }

        return static_cast<size_type>(p);
    }
};


template<typename T, bool isConst>
class BufferIterator {
public:
    // Iterator traits, previously from std::iterator.
    using self_type         = BufferIterator<T, isConst>;
    using value_type        = typename T::value_type;
    using size_type         = typename T::size_type;
    using difference_type   = typename T::difference_type;
    using pointer           = typename T::pointer;
    using const_pointer     = const pointer;
    using reference         = typename T::reference;
    using const_reference   = const reference;
    using iterator_category = std::random_access_iterator_tag;
    using buffer_ptr_type   =
        typename std::conditional<isConst, const T*, T*>::type;
    using value_ref_type    =
        typename std::conditional<isConst,const value_type&, value_type&>::type;
    using container_type    =
        typename std::conditional<isConst,const value_type, value_type>::type;

    BufferIterator() : _buffer{nullptr}, _pos{nullptr} {
    }

    BufferIterator(buffer_ptr_type buffer) : _buffer{buffer} {
        if (_buffer->_gapStart == _buffer->_text.begin() && !_buffer->empty()) {
            _pos = _buffer->_gapEnd;
        } else {
            // we have to go through all these convulutions just to assign the
            // value of a const iterator to an iterator.
            std::advance(_pos,
                std::distance<typename T::container_const_iterator>(_pos,
                    _buffer->_text.begin()));
        }
    }

    BufferIterator(const BufferIterator<T, false>& that) :
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
        return BufferIterator<T, isConst>(*this) += n;
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
        return BufferIterator<T, isConst>(*this) -= n;
    }

    self_type& operator--() {
        this->operator-(1);
        return *this;
    }

    self_type operator--(int) {
        self_type tmp(*this);
        operator--();
        return tmp;
    }

    value_ref_type operator*() const {
        return *_pos;
    }

    pointer operator->() const {
        return _pos;
    }

    value_ref_type operator[](const size_type& n) const {
        return *(_pos + n);
    }

    size_type pos() const {
        return _buffer->gapToUser(_pos);
    }

    template<typename U, bool U_isConst>
    friend bool std::operator<(
    const BufferIterator<U, U_isConst>& lhs,
    const BufferIterator<U, U_isConst>& rhs);

    template<typename U, bool U_isConst>
    friend bool std::operator>(
    const BufferIterator<U, U_isConst>& lhs,
    const BufferIterator<U, U_isConst>& rhs);

    template<typename U, bool U_isConst>
    friend bool std::operator<=(
    const BufferIterator<U, U_isConst>& lhs,
    const BufferIterator<U, U_isConst>& rhs);

    template<typename U, bool U_isConst>
    friend bool std::operator>=(
    const BufferIterator<U, U_isConst>& lhs,
    const BufferIterator<U, U_isConst>& rhs);

    template<typename U, bool U_isConst>
    friend difference_type std::operator+(
    const BufferIterator<U, U_isConst>& lhs,
    const BufferIterator<U, U_isConst>& rhs);

    template<typename U, bool U_isConst>
    friend difference_type std::operator-(
    const BufferIterator<U, U_isConst>& lhs,
    const BufferIterator<U, U_isConst>& rhs);

    template<typename U, bool U_isConst>
    friend void std::swap(
    const BufferIterator<U, U_isConst>& lhs,
    const BufferIterator<U, U_isConst>& rhs);

    friend class BufferIterator<T, true>;

private:
    buffer_ptr_type                             _buffer;
    typename T::container_iterator              _pos;

};

namespace std {
    template<typename T, bool isConst>
    inline bool operator<(const BufferIterator<T, isConst>& lhs,
    const BufferIterator<T, isConst>& rhs) {
        return lhs._buffer == rhs._buffer && lhs._pos < rhs._pos;
    }

    template<typename T, bool isConst>
    inline bool operator>(const BufferIterator<T, isConst>& lhs,
    const BufferIterator<T, isConst>& rhs) {
        return lhs._buffer == rhs._buffer && lhs._pos > rhs._pos;
    }

    template<typename T, bool isConst>
    inline bool operator<=(const BufferIterator<T, isConst>& lhs,
    const BufferIterator<T, isConst>& rhs) {
        return !operator>(lhs, rhs);
    }

    template<typename T, bool isConst>
    inline bool operator>=(const BufferIterator<T, isConst>& lhs,
    const BufferIterator<T, isConst>& rhs) {
    return !operator<(lhs, rhs);
    }

    template<typename T, bool isConst>
    inline typename BufferIterator<T, isConst>::difference_type
    operator+(
    const BufferIterator<T, isConst>& lhs,
    const BufferIterator<T, isConst>& rhs) {
        return lhs._pos + rhs._pos;
    }

    template<typename T, bool isConst>
    inline typename BufferIterator<T, isConst>::difference_type
    operator-(
    const BufferIterator<T, isConst>& lhs,
    const BufferIterator<T, isConst>& rhs) {
    return lhs._pos - rhs._pos;
}

    template<typename T, bool isConst>
    inline void swap(BufferIterator<T, isConst>& lhs,
    BufferIterator<T, isConst>& rhs) {
        if (&lhs != &rhs) {
            lhs._buffer->swap(rhs._buffer);
            std::swap(lhs._pos, rhs._pos);
        }
    }
}

#endif
