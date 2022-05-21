#pragma once
#include "Simd/SimdLib.hpp"
#include "Simd/SimdFont.hpp"
#ifndef DRAWING_H
#define DRAWING_H

#include <memory>
#include <smmintrin.h>
#include <immintrin.h>

#define CHANNEL 4
#define FORMAT View::Bgra32
#define FORMATQT QImage::Format_ARGB32

typedef Simd::Point<ptrdiff_t> Point;
typedef Simd::View<Simd::Allocator> View;
typedef Simd::Pixel::Bgra32 Color;

namespace Simd {
namespace AVX2 {
    void Overlay(uint8_t * src, size_t stride, size_t src_width, size_t src_height, uint8_t * dest, size_t dest_width, size_t dest_height, size_t x, size_t y);
}

template<template<class> class A> SIMD_INLINE uint8_t* getPixel(View<A>& view,int x,int y) {
    return &view.data[(CHANNEL*(x))+(view.width*CHANNEL*(y))];
}


template<template<class> class A> SIMD_INLINE void Overlay(const View<A>& dest, size_t x, size_t y, View<A>& src) {
    AVX2::Overlay(src.data, CHANNEL, src.width,src.height,dest.data,dest.width,dest.height,x,y);
}

template<template<class> class A> SIMD_INLINE void FillValue(View<A>& dst, uint8_t value) {
    SimdFill(dst.data,dst.width*CHANNEL,dst.width,dst.height,CHANNEL,value);
}



}

namespace drw {

}

#endif // DRAWING_H
