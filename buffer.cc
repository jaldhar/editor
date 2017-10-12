// Buffer -- manages text in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <algorithm>
#include <cstdlib>
#include <iterator>
using namespace std;

#include "buffer.h"

template<typename T>
Buffer<T>::Buffer() : _text{0}, _point{0}, _offset{_text.begin()},
_gapStart{_text.begin()}, _gapEnd{_text.end()} {
}

template<typename T>
bool Buffer<T>::operator==(const Buffer& that) const {
    return this->_text == that._text && this->_point == that._point  &&
        this->_offset == that._offset && this->_gapStart == that._gapStart &&
        this->_gapEnd == that._gapEnd;
}

template<typename T>
bool Buffer<T>::operator!=(const Buffer& that) const {
    return !operator==(that);
}

template<typename T>
int Buffer<T>::compareLocations(iterator loc1, iterator loc2) {
    if (loc1 > loc2) {
        return 1;
    } else if (loc1 < loc2) {
        return -1;
    }

    return 0;
}

template<typename T>
size_t Buffer<T>::locationToCount(iterator loc) {
    size_t count = loc - _text.begin();
    if (loc > _gapEnd) {
        count -= (_gapEnd - _gapStart);
    }

    return count;
}

template<typename T>
T* Buffer<T>::countToLocation(size_t count) {
    T* loc = _text.begin() + count;

    if (loc > _gapStart) {
        loc -= (_gapEnd - _gapStart);
    }

    return loc;
}

template<typename T>
void Buffer<T>::moveGap() {
    T* ploc = countToLocation(_point);
    int compare = compareLocations(ploc, _gapStart);
    
    if (compare == 0) { // point is already at gapStart
        return;
    }

    _gapStart = ploc;
    size_t n;

    if (compare == 1) { // point is after gapStart
        n = ploc - _gapEnd;
        _gapEnd += n;

    } else { // point is before _gapStart
        n = _gapStart - ploc;
        _gapEnd -= n;
    }

    copy(ploc, ploc + n, _gapEnd);
}

template<typename T>
BufferIterator<T>::BufferIterator(Buffer<T>& b, bool end) : _b{b}, _end{end},
_i{_b._text.begin()} {
}

template<typename T>
BufferIterator<T>::BufferIterator(const BufferIterator<T>& that) : _b(that._b),
_end{that._end}, _i{that._i} {
}

template<typename T>
BufferIterator<T>& BufferIterator<T>::operator=(const BufferIterator<T>& that) {
    if (this != &that) {
        this->_b = that._b;
        this->_end = that._end;
    }
    return *this;
}

template<typename T>
bool BufferIterator<T>::operator==(const BufferIterator<T>& that) const {
    return this->_b == that._b && this->_end == that._end  &&
        this->_i == that._i;
}

template<typename T>
bool BufferIterator<T>::operator!=(const BufferIterator<T>& that) const {
    return !operator==(that);
}

template<typename T>
BufferIterator<T> BufferIterator<T>::operator++() {
    if (_i >= _b._gapStart) {
        _i = _b._gapEnd;
    }
 
    _i++;
 
    return *this;
}

template<typename T>
BufferIterator<T> BufferIterator<T>::operator++(int) {
    BufferIterator<T> result(*this);
    operator++();
    return result;
}

template<typename T>
T BufferIterator<T>::operator*() const {
    return *_i;
}


template<typename T>
void BufferIterator<T>::swap(BufferIterator<T>& that) {
    if (this != &that) {
        this->_b.swap(that._b);
        std::swap(this->_end, that._end);
    }
}

template<typename T>
void swap(BufferIterator<T>& lhs, BufferIterator<T>& rhs) {
    lhs.swap(rhs);
}
