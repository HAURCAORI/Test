#include "drawing.h"


namespace Simd {
/*
template<template<class> class A> SIMD_INLINE void Overlay(View<A>& dest, int x, int y, View<A>& src) {
    int h = src.height;
    int w = src.width;
    for (int sy = y, cy = 0; cy < h; sy++, cy++)
    {
        if((unsigned int) sy >= dest.height) { break; }
        int sx = x;
        int cx = 0;
        for (; cx + 8 <= w; sx+=8, cx+=8)
        {
            if( (unsigned int) sx + 8 >= dest.width) { break; }
            __m256i dm = _mm256_loadu_si256((__m256i_u*) getPixel(src,cx,cy));
            _mm256_storeu_si256((__m256i*) getPixel(dest,sx,sy),dm);
        }

        for(; cx < w; sx++, cx++) {
            if( (unsigned int) sx >= dest.width) { break; }
            uint8_t* dp = getPixel(dest,sx,sy);
            uint8_t* sp = getPixel(src,cx,cy);
            *(dp++) = *(sp++);
            *(dp++) = *(sp++);
            *(dp++) = *(sp++);
            *(dp++) = *(sp++);
        }
    }
}
*/
namespace AVX2 {
void Overlay(uint8_t * src, size_t stride, size_t src_width, size_t src_height, uint8_t * dest, size_t dest_width, size_t dest_height, size_t x, size_t y) {
    for (size_t sy = y, cy = 0; cy < src_height; sy++, cy++)
    {
        if(sy >= dest_height) { break; }
        size_t sx = x;
        size_t cx = 0;
        for (; cx + 8 <= src_width; sx+=8, cx+=8)
        {
            if(sx + 8 >= dest_width) { break; }
            __m256i dm = _mm256_loadu_si256((__m256i_u *) (src+(stride*cx+src_width*stride*cy)));//getPixel(src,cx,cy));
            _mm256_storeu_si256((__m256i*) (dest+(stride*sx+dest_width*stride*sy)), dm);//getPixel(dest,sx,sy),dm);
        }

        for(; cx < src_width; sx++, cx++) {
            if(sx >= dest_width) { break; }
            uint8_t* dp = (dest+(stride*sx+dest_width*stride*sy));
            const uint8_t* sp = (src+(stride*cx+src_width*stride*cy));
            *(dp++) = *(sp++);
            *(dp++) = *(sp++);
            *(dp++) = *(sp++);
            *(dp++) = *(sp++);
        }
    }
}

}

}
