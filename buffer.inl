// Buffer -- manages text in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <algorithm>
#include <cstdlib>
#include <iterator>

template<typename T>
Buffer<T>::Buffer() : _text(BUFFERSIZE, 0), _point{0}, _gapStart{_text.begin()},
_gapEnd{_text.end()} {
}

template<typename T>
Buffer<T>::Buffer(const Buffer<T>& that) : _text(that._text),
_point{that._point}, _gapStart{that._gapStart}, _gapEnd{that._gapEnd} {
}

template<typename T>
Buffer<T>& Buffer<T>::operator=(const Buffer<T>& that) {
    if (this != &that) {
        this->_text = that._text;
        this->_point = that._point;
        this->_gapStart = that._gapStart;
        this->_gapEnd = that._gapEnd;
    }
    return *this;
}

template<typename T>
bool Buffer<T>::operator==(const Buffer<T>& that) const {
    return this->_text == that._text &&
        this->_point == that._point &&
        this->_gapStart == that._gapStart &&
        this->_gapEnd == that._gapEnd;
}

template<typename T>
bool Buffer<T>::operator!=(const Buffer<T>& that) const {
    return !operator==(that);
}

template<typename T>
typename Buffer<T>::reference Buffer<T>::operator[](difference_type n) {
    return Buffer<T>::iterator(*this)[n];
}

template<typename T>
typename Buffer<T>::iterator Buffer<T>::begin() {
    return Buffer<T>::iterator(*this);
}

template<typename T>
typename Buffer<T>::iterator Buffer<T>::end() {
    return Buffer<T>::iterator(*this) +  size();
}

template<typename T>
typename Buffer<T>::const_iterator Buffer<T>::cbegin() {
    return const_cast<const Buffer<T>&>(*this).begin();
}

template<typename T>
typename Buffer<T>::const_iterator Buffer<T>::cend() {
    return const_cast<const Buffer<T>&>(*this).end();
}

template<typename T>
typename Buffer<T>::size_type Buffer<T>::capacity() {
    return _text.capacity();
}

template<typename T>
bool Buffer<T>::empty() {
    return size() ? false : true;
}

template<typename T>
typename Buffer<T>::size_type Buffer<T>::max_size() {
    return _text.max_size();
}

template<typename T>
typename Buffer<T>::size_type Buffer<T>::size() {
    return _text.capacity() - (_gapEnd - _gapStart);
}

template<typename T>
bool Buffer<T>::deletePrevious() {
    if (_point <= 0 || _point > static_cast<difference_type>(size())) {
        return false;
    }

    moveGap();
    _gapStart--;
    return pointMove(-1);
}

template<typename T>
bool Buffer<T>::deleteNext() {
    if (_point < 0 || _point >= static_cast<difference_type>(size())) {
        return false;
    }

    moveGap();
    _gapEnd++;
    return true;
}

template<typename T>
bool Buffer<T>::insert(value_type c) {
    if (_point < 0 || _point > static_cast<difference_type>(size())) {
        return false;
    }

    moveGap();
    *_gapStart = c;
    _gapStart++;
    return pointMove(1);
}

template<typename T>
typename Buffer<T>::difference_type Buffer<T>::point() const {
    return _point;
}

template<typename T>
bool Buffer<T>::pointMove(int count) {
    Buffer<T>::difference_type loc = _point + count;
    if (loc < 0 || loc > static_cast<Buffer<T>::difference_type>(size())) {
        return false;
    }

    return pointSet(userToGap(loc));
}

template<typename T>
bool Buffer<T>::pointSet(BUFFER::iterator loc) {
    if (loc < _text.begin() || loc > _text.end()) {
        return false;
    }

    _point = gapToUser(loc);

    return true;
}

template<typename T>
BufferInternals Buffer<T>::internals()  {
    return {
        capacity(),
        distance(_text.begin(), userToGap(_point)),
        size(),
        distance(_text.begin(), _gapStart),
        distance(_text.begin(), _gapEnd)
    };
}

template<typename T>
void Buffer<T>::moveGap() {
    if (_gapStart == _gapEnd) {
        _text.resize(_text.capacity() + BUFFERSIZE, 0);
        _text.shrink_to_fit();
        _gapStart = _text.end() - BUFFERSIZE;
        _gapEnd = _text.end();
    }

    BUFFER::iterator p = userToGap(_point);
    if (p == _gapStart) {
        return;
    }

    Buffer<T>::difference_type n;
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

template<typename T>
BUFFER::iterator Buffer<T>::userToGap(difference_type p) {
    BUFFER::iterator i = _text.begin() + p;

    if (i > _gapStart) {
        i += (_gapEnd - _gapStart);
    }

    return i;
}

template<typename T>
typename Buffer<T>::difference_type Buffer<T>::gapToUser(BUFFER::iterator i) {
    Buffer<T>::difference_type p = distance(_text.begin(), i);

    if (i > _gapEnd) {
        p -= (_gapEnd - _gapStart);
    }

    return p;
}

template<typename T>
void std::swap(Buffer<T>& lhs, Buffer<T>& rhs) {
    if (lhs != rhs) {
        lhs._text.swap(rhs._text);
        std::swap(lhs._point, rhs._point);
        std::swap(lhs._gapStart, rhs._gapStart);
        std::swap(lhs._gapEnd, rhs._gapEnd);
    }
}

template<typename T>
BufferIterator<T>::BufferIterator() : _b{nullptr}, _i{nullptr} {
}

template<typename T>
BufferIterator<T>::BufferIterator(Buffer<T>& b) : _b{b} {
    if (b._gapStart == b._text.begin() && !_b.empty()) {
        _i = b._gapEnd;
    } else {
        _i = b._text.begin();
    }
}

template<typename T>
BufferIterator<T>::BufferIterator(const BufferIterator<T>& that) : _b(that._b),
_i{that._i} {
}

template<typename T>
BufferIterator<T>& BufferIterator<T>::operator=(const BufferIterator<T>& that) {
    if (this != &that) {
        this->_b = that._b;
        this->_i = that._i;
    }
    return *this;
}

template<typename T>
bool BufferIterator<T>::operator==(const BufferIterator<T>& that) {
    return _i == that._i;
}

template<typename T>
bool BufferIterator<T>::operator!=(const BufferIterator<T>& that) {
    return !this->operator==(that);
}

template<typename T>
BufferIterator<T>& BufferIterator<T>::operator+=(const difference_type& n) {
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

template<typename T>
BufferIterator<T> BufferIterator<T>::operator+(const difference_type& n) {
    return BufferIterator<T>(*this += n);
}

template<typename T>
BufferIterator<T>& BufferIterator<T>::operator++() {
    this->operator+(1);
    return *this;
}

template<typename T>
BufferIterator<T> BufferIterator<T>::operator++(int) {
    BufferIterator<T> tmp(*this);
    operator++();
    return tmp;
}

template<typename T>
BufferIterator<T>& BufferIterator<T>::operator-=(const difference_type& n) {
    return *this += -n;
}

template<typename T>
    BufferIterator<T> BufferIterator<T>::operator-(const difference_type& n) {
        return BufferIterator<T>(*this - n);
    }

template<typename T>
BufferIterator<T>& BufferIterator<T>::operator--() {
    this->operator-(1);
    return *this;
}

template<typename T>
const BufferIterator<T>& BufferIterator<T>::operator--(int) {
    BufferIterator<T> tmp(*this);
    operator--();
    return tmp;
}

template<typename T>
typename BufferIterator<T>::reference BufferIterator<T>::operator*() const {
    return *_i;
}

template<typename T>
typename BufferIterator<T>::pointer BufferIterator<T>::operator->() const {
    return _i;
}

template<typename T>
typename BufferIterator<T>::reference BufferIterator<T>::operator[](
const difference_type& n) const {
    return *(_i + n);
}

template<typename T>
bool std::operator<(const BufferIterator<T>& lhs,
const BufferIterator<T>& rhs) {
    return lhs._i < rhs._i;
}

template<typename T>
bool std::operator>(const BufferIterator<T>& lhs,
const BufferIterator<T>& rhs){
    return lhs._i > rhs._i;
}

template<typename T>
bool std::operator<=(const BufferIterator<T>& lhs,
const BufferIterator<T>& rhs) {
    return !operator>(lhs, rhs);
}

template<typename T>
bool std::operator>=(const BufferIterator<T>& lhs,
const BufferIterator<T>& rhs) {
    return !operator<(lhs, rhs);
}

template<typename T>
typename BufferIterator<T>::difference_type std::operator+(
const BufferIterator<T>& lhs, const BufferIterator<T>& rhs) {
    return lhs._i + rhs._i;
}

template<typename T>
typename BufferIterator<T>::difference_type std::operator-(
const BufferIterator<T>& lhs, const BufferIterator<T>& rhs) {
    return lhs._i - rhs._i;
}

template<typename T>
void std::swap(BufferIterator<T>& lhs, BufferIterator<T>& rhs) {
    if (lhs != rhs) {
        lhs._b.swap(rhs._b);
        std::swap(lhs._i, rhs._i);
    }
}
