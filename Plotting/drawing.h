#pragma once
#include "Simd/SimdLib.hpp"
#include "Simd/SimdFont.hpp"
#ifndef DRAWING_H
#define DRAWING_H

#include <memory>
#include <smmintrin.h>
#include <immintrin.h>
#include <iostream>

#define CHANNEL 4
#define FORMAT View::Bgra32
#define FORMATQT QImage::Format_ARGB32

typedef Simd::Point<ptrdiff_t> Point;
typedef Simd::View<Simd::Allocator> View;
typedef Simd::Pixel::Bgra32 Color;


struct Size {
    size_t width;
    size_t height;
    Size() : width(0), height(0) {}
    Size(size_t width, size_t height) : width(width), height(height) {}
    //Size(const Size& size) : width(size.width), height(size.height) {}
};

struct Location {
    size_t x;
    size_t y;
    Location() : x(0), y(0) {}
    Location(size_t x, size_t y) : x(x), y(y) {}
    operator Point() {
        return Point(x,y);
    }
};

template <class T>
inline void swapValue (T& d, T& s)
{
  T temp = d;
  d = s;
  s = temp;
}

namespace Simd {
namespace AVX2 {
    void Overlay(uint8_t * src, size_t stride, size_t src_width, size_t src_height, uint8_t * dest, size_t dest_width, size_t dest_height, size_t x, size_t y);
}


template<template<class> class A> SIMD_INLINE uint8_t* getPixel(View<A>& view,int x,int y) {
    if(view.width > x && view.height > y)
        return &view.data[(CHANNEL*(x))+(view.width*CHANNEL*(y))];
    return nullptr;
}

template<template<class> class A> void setPixel(View<A>& view, const Color color) {
    uint8_t* src = view.data;
    *(src++) = color.red;
    *(src++) = color.green;
    *(src++) = color.blue;
#if CHANNEL==4
    *(src++) = 255;
#endif
}

void setPixel(uint8_t*& src, const Color color);


template<template<class> class A> SIMD_INLINE void Overlay(const View<A>& dest, size_t x, size_t y, View<A>& src) {
    AVX2::Overlay(src.data, CHANNEL, src.width,src.height,dest.data,dest.width,dest.height,x,y);
}

template<template<class> class A> SIMD_INLINE void FillValue(View<A>& dst, uint8_t value) {
    SimdFill(dst.data,dst.width*CHANNEL,dst.width,dst.height,CHANNEL,value);
}
template<template<class> class A> SIMD_INLINE void FillValueBgra(View<A>& dst, uint8_t blue, uint8_t green, uint8_t red, uint8_t alpha) {
    SimdFillBgra(dst.data,dst.width*CHANNEL,dst.width,dst.height, blue, green, red, alpha);
}

template<template<class> class A> SIMD_INLINE void DrawingLine(View<A>& view, int x0, int y0, int x1, int y1, const Color color) {
    int dx, dy, sx,sy, error, e2;

    dx= abs(x1-x0);
    sx = x0 < x1 ? 1 : -1;
    dy= -abs(y1-y0);
    sy = y0 < y1 ? 1 : -1;
    error = dx + dy;
    uint8_t* src = getPixel(view,x0,y0);
    if(!src) { return; }

    size_t stride = view.width;
    while(true) {
        setPixel(src,color);
        if(x0== x1 && y0 == y1) break;
        e2 = 2 * error;
        if(e2 >= dy){
            if(x0 == x1 ) break;
            error = error + dy;
            x0 = x0 + sx;
            src+=CHANNEL*sx;
        }
        if(e2 <= dx) {
            if(y0 == y1) break;
            error = error + dx;
            y0 = y0 + sy;
            src+= stride * CHANNEL * sy;
        }
    }
}
template<template<class> class A> SIMD_INLINE void DrawingLine(View<A>& view, Location l1, Location l2, const Color color) {
    Simd::DrawingLine(view,l1.x,l1.y,l2.x,l2.y,color);
}
}


#endif // DRAWING_H
