// Buffer -- manages text in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <algorithm>
#include <cstdlib>
#include <iterator>

template<typename T, std::size_t N, typename Container>
Buffer<T, N, Container>::Buffer() : _text(N, 0), _point{0},
_gapStart{_text.begin()}, _gapEnd{_text.end()} {
}

template<typename T, std::size_t N, typename Container>
Buffer<T, N, Container>::Buffer(const Buffer<T, N, Container>& that) :
_text(that._text), _point{that._point}, _gapStart{that._gapStart},
_gapEnd{that._gapEnd} {
}

template<typename T, std::size_t N, typename Container>
Buffer<T, N, Container>&
Buffer<T, N, Container>::operator=(const Buffer<T, N, Container>& that) {
    if (this != &that) {
        this->_text = that._text;
        this->_point = that._point;
        this->_gapStart = that._gapStart;
        this->_gapEnd = that._gapEnd;
    }
    return *this;
}

template<typename T, std::size_t N, typename Container>
bool Buffer<T, N, Container>::operator==(
const Buffer<T, N, Container>& that) const {
    return this->_text == that._text &&
        this->_point == that._point &&
        this->_gapStart == that._gapStart &&
        this->_gapEnd == that._gapEnd;
}

template<typename T, std::size_t N, typename Container>
bool Buffer<T, N, Container>::operator!=(
const Buffer<T, N, Container>& that) const {
    return !operator==(that);
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::reference
Buffer<T, N, Container>::operator[](size_type n) {
    return Buffer<T, N, Container>::iterator(*this)[n];
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::iterator Buffer<T, N, Container>::begin() {
    return Buffer<T, N, Container>::iterator(*this);
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::iterator Buffer<T, N, Container>::end() {
    return Buffer<T, N, Container>::iterator(*this) +  size();
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::const_iterator
Buffer<T, N, Container>::cbegin() {
    return const_cast<const Buffer<T, N, Container>&>(*this).begin();
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::const_iterator
Buffer<T, N, Container>::cend() {
    return const_cast<const Buffer<T, N, Container>&>(*this).end();
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::size_type
Buffer<T, N, Container>::capacity() {
    return _text.capacity();
}

template<typename T, std::size_t N, typename Container>
bool Buffer<T, N, Container>::empty() {
    return !size();
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::size_type
Buffer<T, N, Container>::max_size() {
    return _text.max_size();
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::size_type Buffer<T, N, Container>::size() {
    return _text.capacity() - (_gapEnd - _gapStart);
}

template<typename T, std::size_t N, typename Container>
bool Buffer<T, N, Container>::deletePrevious() {
    if (_point <= 0 || _point > static_cast<difference_type>(size())) {
        return false;
    }

    moveGap();
    _gapStart--;
    return pointMove(-1);
}

template<typename T, std::size_t N, typename Container>
bool Buffer<T, N, Container>::deleteNext() {
    if (_point < 0 || _point >= static_cast<difference_type>(size())) {
        return false;
    }

    moveGap();
    _gapEnd++;
    return true;
}

template<typename T, std::size_t N, typename Container>
bool Buffer<T, N, Container>::insert(value_type c) {
    if (_point < 0 || _point > static_cast<difference_type>(size())) {
        return false;
    }

    moveGap();
    *_gapStart = c;
    _gapStart++;
    return pointMove(1);
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::difference_type
Buffer<T, N, Container>::point() const {
    return _point;
}

template<typename T, std::size_t N, typename Container>
bool Buffer<T, N, Container>::pointMove(int count) {
    Buffer<T, N, Container>::difference_type loc = _point + count;
    if (loc < 0 ||
    loc > static_cast<Buffer<T, N, Container>::difference_type>(size())) {
        return false;
    }

    return pointSet(userToGap(loc));
}

template<typename T, std::size_t N, typename Container>
bool Buffer<T, N, Container>::pointSet(typename Container::iterator loc) {
    if (loc < _text.begin() || loc > _text.end()) {
        return false;
    }

    _point = gapToUser(loc);

    return true;
}

template<typename T, std::size_t N, typename Container>
BufferInternals Buffer<T, N, Container>::internals() {
    return {
        capacity(),
        distance(_text.begin(), userToGap(_point)),
        size(),
        distance(_text.begin(), _gapStart),
        distance(_text.begin(), _gapEnd)
    };
}

template<typename T, std::size_t N, typename Container>
void Buffer<T, N, Container>::moveGap() {
    if (_gapStart == _gapEnd) {
        _text.resize(_text.capacity() + N, 0);
        _text.shrink_to_fit();
        _gapStart = _text.end() - N;
        _gapEnd = _text.end();
    }

    typename Container::iterator p = userToGap(_point);
    if (p == _gapStart) {
        return;
    }

    Buffer<T, N, Container>::difference_type n;
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

template<typename T, std::size_t N, typename Container>
typename Container::iterator
Buffer<T, N, Container>::userToGap(difference_type p) {
    typename Container::iterator i = _text.begin() + p;

    if (i > _gapStart) {
        i += (_gapEnd - _gapStart);
    }

    return i;
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::difference_type
Buffer<T, N, Container>::gapToUser(typename Container::iterator i) {
    Buffer<T, N, Container>::difference_type p = distance(_text.begin(), i);

    if (i > _gapEnd) {
        p -= (_gapEnd - _gapStart);
    }

    return p;
}

template<typename T, std::size_t N, typename Container>
void std::swap(Buffer<T, N, Container>& lhs, Buffer<T, N, Container>& rhs) {
    if (lhs != rhs) {
        lhs._text.swap(rhs._text);
        std::swap(lhs._point, rhs._point);
        std::swap(lhs._gapStart, rhs._gapStart);
        std::swap(lhs._gapEnd, rhs._gapEnd);
    }
}

template<typename T, std::size_t N, typename Container>
BufferIterator<T, N, Container>::BufferIterator() : _b{nullptr},
_i{nullptr} {
}

template<typename T, std::size_t N, typename Container>
BufferIterator<T, N, Container>::BufferIterator(
const Buffer<T, N, Container>& b) : _b{b} {
    if (_b._gapStart == _b._text.begin() &&
    !const_cast<Buffer<T, N, Container>&>(_b).empty()) {
        _i = const_cast<Buffer<T, N, Container>&>(_b)._gapEnd;
    } else {
        _i = const_cast<Buffer<T, N, Container>&>(_b)._text.begin();
    }
}

template<typename T, std::size_t N, typename Container>
BufferIterator<T, N, Container>::BufferIterator(
const BufferIterator<T, N, Container>& that) : _b(that._b),
_i{that._i} {
}

template<typename T, std::size_t N, typename Container>
BufferIterator<T, N, Container>&
BufferIterator<T, N, Container>::operator=(
const BufferIterator<T, N,Container> & that) {
    if (this != &that) {
        this->_b = that._b;
        this->_i = that._i;
    }
    return *this;
}

template<typename T, std::size_t N, typename Container>
bool BufferIterator<T, N, Container>::operator==(
const BufferIterator<T, N, Container>& that) {
    return _i == that._i;
}

template<typename T, std::size_t N, typename Container>
bool BufferIterator<T, N, Container>::operator!=(
const BufferIterator<T, N, Container>& that) {
    return !this->operator==(that);
}

template<typename T, std::size_t N, typename Container>
BufferIterator<T, N, Container>&
BufferIterator<T, N, Container>::operator+=(const difference_type& n) {
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

template<typename T, std::size_t N, typename Container>
BufferIterator<T, N, Container>
BufferIterator<T, N, Container>::operator+(const difference_type& n) {
    return BufferIterator<T, N, Container>(*this += n);
}

template<typename T, std::size_t N, typename Container>
BufferIterator<T, N, Container>&
BufferIterator<T, N, Container>::operator++() {
    this->operator+(1);
    return *this;
}

template<typename T, std::size_t N, typename Container>
BufferIterator<T, N, Container>
BufferIterator<T, N, Container>::operator++(int) {
    BufferIterator<T, N, Container> tmp(*this);
    operator++();
    return tmp;
}

template<typename T, std::size_t N, typename Container>
BufferIterator<T, N, Container>&
BufferIterator<T, N, Container>::operator-=(const difference_type& n) {
    return *this += -n;
}

template<typename T, std::size_t N, typename Container>
BufferIterator<T, N, Container>
BufferIterator<T, N, Container>::operator-(const difference_type& n) {
        return BufferIterator<T, N, Container>(*this - n);
    }

template<typename T, std::size_t N, typename Container>
BufferIterator<T, N, Container>& BufferIterator<T, N, Container>::operator--() {
    this->operator-(1);
    return *this;
}

template<typename T, std::size_t N, typename Container>
const BufferIterator<T, N, Container>&
BufferIterator<T, N, Container>::operator--(int) {
    BufferIterator<T, N, Container> tmp(*this);
    operator--();
    return tmp;
}

template<typename T, std::size_t N, typename Container>
typename BufferIterator<T, N, Container>::reference
BufferIterator<T, N, Container>::operator*() const {
    return *_i;
}

template<typename T, std::size_t N, typename Container>
typename BufferIterator<T, N, Container>::pointer
BufferIterator<T, N, Container>::operator->() const {
    return _i;
}

template<typename T, std::size_t N, typename Container>
typename BufferIterator<T, N, Container>::reference
BufferIterator<T, N, Container>::operator[](
const size_type& n) const {
    return *(_i + n);
}

template<typename T, std::size_t N, typename Container>
bool std::operator<(const BufferIterator<T, N, Container>& lhs,
const BufferIterator<T, N, Container>& rhs) {
    return lhs._i < rhs._i;
}

template<typename T, std::size_t N, typename Container>
bool std::operator>(const BufferIterator<T, N, Container>& lhs,
const BufferIterator<T, N, Container>& rhs){
    return lhs._i > rhs._i;
}

template<typename T, std::size_t N, typename Container>
bool std::operator<=(const BufferIterator<T, N, Container>& lhs,
const BufferIterator<T, N, Container>& rhs) {
    return !operator>(lhs, rhs);
}

template<typename T, std::size_t N, typename Container>
bool std::operator>=(const BufferIterator<T, N, Container>& lhs,
const BufferIterator<T, N, Container>& rhs) {
    return !operator<(lhs, rhs);
}

template<typename T, std::size_t N, typename Container>
typename BufferIterator<T, N, Container>::difference_type std::operator+(
const BufferIterator<T, N, Container>& lhs,
const BufferIterator<T, N, Container>& rhs) {
    return lhs._i + rhs._i;
}

template<typename T, std::size_t N, typename Container>
typename BufferIterator<T, N, Container>::difference_type
std::operator-(const BufferIterator<T, N, Container>& lhs,
const BufferIterator<T, N, Container>& rhs) {
    return lhs._i - rhs._i;
}

template<typename T, std::size_t N, typename Container>
void std::swap(BufferIterator<T, N, Container>& lhs,
BufferIterator<T, N, Container>& rhs) {
    if (lhs != rhs) {
        lhs._b.swap(rhs._b);
        std::swap(lhs._i, rhs._i);
    }
}
