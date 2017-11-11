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
Buffer<T, N, Container>::Buffer(Buffer<T, N, Container>&& that) :
_text(std::move(that._text)), _point{that._point},
_gapStart{std::move(that._gapStart)}, _gapEnd{std::move(that._gapEnd)} {
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
Buffer<T, N, Container>&
Buffer<T, N, Container>::operator=(Buffer<T, N, Container>&& that) {
    if (this != &that) {
        this->_text = std::move(that._text);
        this->_point = that._point;
        this->_gapStart = std::move(that._gapStart);
        this->_gapEnd = std::move(that._gapEnd);
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
    return Buffer<T, N, Container>::iterator(this)[n];
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::iterator Buffer<T, N, Container>::begin() {
    return Buffer<T, N, Container>::iterator(this);
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::iterator Buffer<T, N, Container>::end() {
    return Buffer<T, N, Container>::iterator(this) +  size();
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::const_iterator Buffer<T, N, Container>::begin() const {
    return const_iterator(
        *(const_cast<Buffer<T, N, Container> *>(this)));
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::const_iterator Buffer<T, N, Container>::end() const {
    return Buffer<T, N, Container>::const_iterator(this) +  size();
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::const_iterator
Buffer<T, N, Container>::cbegin() const {
    return begin();
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::const_iterator
Buffer<T, N, Container>::cend() const {
    return end();
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::size_type
Buffer<T, N, Container>::capacity() const {
    return _text.capacity();
}

template<typename T, std::size_t N, typename Container>
bool Buffer<T, N, Container>::empty() const {
    return !size();
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::size_type
Buffer<T, N, Container>::max_size() const {
    return _text.max_size();
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::size_type Buffer<T, N, Container>::size() const {
    return _text.capacity() - (_gapEnd - _gapStart);
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::size_type
Buffer<T,N,Container>::front() const {
    return 0;
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::size_type
Buffer<T,N,Container>::back() const {
    return (size() == 0) ? 0 : size() - 1;
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
bool Buffer<T, N, Container>::pointSet(difference_type n) {
    _point = n;
    return true;
}

template<typename T, std::size_t N, typename Container>
bool Buffer<T, N, Container>::pointMove(int count) {
    Buffer<T, N, Container>::difference_type loc = _point + count;
    if (loc < 0 ||
    loc > static_cast<Buffer<T, N, Container>::difference_type>(size())) {
        return false;
    }

    _point = loc;
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

    if (i >= _gapStart) {
        i += (_gapEnd - _gapStart);
    }

    return i;
}

template<typename T, std::size_t N, typename Container>
typename Buffer<T, N, Container>::difference_type
Buffer<T, N, Container>::gapToUser(typename Container::iterator i) {
    Buffer<T, N, Container>::difference_type p = distance(_text.begin(), i);

    if (i >= _gapEnd) {
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

template<typename T, typename T_nonconst, typename elem_type>
BufferIterator<T, T_nonconst, elem_type>::BufferIterator() : _buffer{nullptr},
_pos{nullptr} {
}

template<typename T, typename T_nonconst, typename elem_type>
BufferIterator<T, T_nonconst, elem_type>::BufferIterator(const T* buffer) :
_buffer{buffer} {
    auto b = const_cast<T*>(_buffer);

    if (b->_gapStart == b->_text.begin() && !b->empty()) {
        _pos = b->_gapEnd;
    } else {
        _pos = b->_text.begin();
    }
}

template<typename T, typename T_nonconst, typename elem_type>
BufferIterator<T, T_nonconst, elem_type>::BufferIterator(
const BufferIterator<T, T_nonconst, elem_type>& that) : _buffer(that._buffer),
_pos{that._pos} {
}

template<typename T, typename T_nonconst, typename elem_type>
BufferIterator<T, T_nonconst, elem_type>&
BufferIterator<T, T_nonconst, elem_type>::operator=(
const BufferIterator<T, T_nonconst, elem_type>& that) {
    if (this != &that) {
        &this->_buffer = &that._buffer;
        this->_pos = that._pos;
    }
    return *this;
}

template<typename T, typename T_nonconst, typename elem_type>
bool BufferIterator<T, T_nonconst, elem_type>::operator==(
const BufferIterator<T, T_nonconst, elem_type>& that) {
    return _buffer == that._buffer && _pos == that._pos;
}

template<typename T, typename T_nonconst, typename elem_type>
bool BufferIterator<T, T_nonconst, elem_type>::operator!=(
const BufferIterator<T, T_nonconst, elem_type>& that) {
    return !this->operator==(that);
}

template<typename T, typename T_nonconst, typename elem_type>
BufferIterator<T, T_nonconst, elem_type>&
BufferIterator<T, T_nonconst, elem_type>::operator+=(const difference_type& n) {
    if (n >= 0) {
        if (_pos < _buffer->_gapStart && _buffer->_gapStart <= _pos + n) {
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

template<typename T, typename T_nonconst, typename elem_type>
BufferIterator<T, T_nonconst, elem_type>
BufferIterator<T, T_nonconst, elem_type>::operator+(const difference_type& n) {
    return BufferIterator<T, T_nonconst, elem_type>(*this += n);
}

template<typename T, typename T_nonconst, typename elem_type>
BufferIterator<T, T_nonconst, elem_type>&
BufferIterator<T, T_nonconst, elem_type>::operator++() {
    this->operator+(1);
    return *this;
}

template<typename T, typename T_nonconst, typename elem_type>
BufferIterator<T, T_nonconst, elem_type>
BufferIterator<T, T_nonconst, elem_type>::operator++(int) {
    BufferIterator<T, T_nonconst, elem_type> tmp(*this);
    operator++();
    return tmp;
}

template<typename T, typename T_nonconst, typename elem_type>
BufferIterator<T, T_nonconst, elem_type>&
BufferIterator<T, T_nonconst, elem_type>::operator-=(const difference_type& n) {
    return *this += -n;
}

template<typename T, typename T_nonconst, typename elem_type>
BufferIterator<T, T_nonconst, elem_type>
BufferIterator<T, T_nonconst, elem_type>::operator-(const difference_type& n) {
        return BufferIterator<T, T_nonconst, elem_type>(*this - n);
    }

template<typename T, typename T_nonconst, typename elem_type>
BufferIterator<T, T_nonconst, elem_type>& BufferIterator<T, T_nonconst, elem_type>::operator--() {
    this->operator-(1);
    return *this;
}

template<typename T, typename T_nonconst, typename elem_type>
const BufferIterator<T, T_nonconst, elem_type>&
BufferIterator<T, T_nonconst, elem_type>::operator--(int) {
    BufferIterator<T, T_nonconst, elem_type> tmp(*this);
    operator--();
    return tmp;
}

template<typename T, typename T_nonconst, typename elem_type>
typename BufferIterator<T, T_nonconst, elem_type>::reference
BufferIterator<T, T_nonconst, elem_type>::operator*() const {
    return *_pos;
}

template<typename T, typename T_nonconst, typename elem_type>
typename BufferIterator<T, T_nonconst, elem_type>::pointer
BufferIterator<T, T_nonconst, elem_type>::operator->() const {
    return _pos;
}

template<typename T, typename T_nonconst, typename elem_type>
typename BufferIterator<T, T_nonconst, elem_type>::reference
BufferIterator<T, T_nonconst, elem_type>::operator[](
const size_type& n) const {
    return *(_pos + n);
}

template<typename T, typename T_nonconst, typename elem_type>
bool std::operator<(const BufferIterator<T, T_nonconst, elem_type>& lhs,
const BufferIterator<T, T_nonconst, elem_type>& rhs) {
    return lhs._buffer == rhs._buffer && lhs._pos < rhs._pos;
}

template<typename T, typename T_nonconst, typename elem_type>
bool std::operator>(const BufferIterator<T, T_nonconst, elem_type>& lhs,
const BufferIterator<T, T_nonconst, elem_type>& rhs){
    return lhs._buffer == rhs._buffer && lhs._pos > rhs._pos;
}

template<typename T, typename T_nonconst, typename elem_type>
bool std::operator<=(const BufferIterator<T, T_nonconst, elem_type>& lhs,
const BufferIterator<T, T_nonconst, elem_type>& rhs) {
    return !operator>(lhs, rhs);
}

template<typename T, typename T_nonconst, typename elem_type>
bool std::operator>=(const BufferIterator<T, T_nonconst, elem_type>& lhs,
const BufferIterator<T, T_nonconst, elem_type>& rhs) {
    return !operator<(lhs, rhs);
}

template<typename T, typename T_nonconst, typename elem_type>
typename BufferIterator<T, T_nonconst, elem_type>::difference_type std::operator+(
const BufferIterator<T, T_nonconst, elem_type>& lhs,
const BufferIterator<T, T_nonconst, elem_type>& rhs) {
    return lhs._pos + rhs._pos;
}

template<typename T, typename T_nonconst, typename elem_type>
typename BufferIterator<T, T_nonconst, elem_type>::difference_type
std::operator-(const BufferIterator<T, T_nonconst, elem_type>& lhs,
const BufferIterator<T, T_nonconst, elem_type>& rhs) {
    return lhs._pos - rhs._pos;
}

template<typename T, typename T_nonconst, typename elem_type>
void std::swap(BufferIterator<T, T_nonconst, elem_type>& lhs,
BufferIterator<T, T_nonconst, elem_type>& rhs) {
    if (&lhs != &rhs) {
        lhs._buffer->swap(rhs._buffer);
        std::swap(lhs._pos, rhs._pos);
    }
}
