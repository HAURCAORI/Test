/*
* Simd Library (http://ermig1979.github.io/Simd).
*
* Copyright (c) 2011-2017 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef __SimdAvx512f_h__
#define __SimdAvx512f_h__

#include "Simd/SimdDefs.h"

namespace Simd
{
#ifdef SIMD_AVX512F_ENABLE    
    namespace Avx512f
    {
        void NeuralProductSum(const float * a, const float * b, size_t size, float * sum);

        void NeuralAddVectorMultipliedByValue(const float * src, size_t size, const float * value, float * dst);

        void NeuralAddVector(const float * src, size_t size, float * dst);

        void NeuralAddValue(const float * value, float * dst, size_t size);

        void NeuralSigmoid(const float * src, size_t size, const float * slope, float * dst);

        void NeuralRoughSigmoid(const float * src, size_t size, const float * slope, float * dst);

        void NeuralRoughSigmoid2(const float * src, size_t size, const float * slope, float * dst);

        void NeuralDerivativeSigmoid(const float * src, size_t size, const float * slope, float * dst);

        void NeuralTanh(const float * src, size_t size, const float * slope, float * dst);

        void NeuralRoughTanh(const float * src, size_t size, const float * slope, float * dst);

        void NeuralDerivativeTanh(const float * src, size_t size, const float * slope, float * dst);

        void NeuralRelu(const float * src, size_t size, const float * slope, float * dst);

        void NeuralDerivativeRelu(const float * src, size_t size, const float * slope, float * dst);

        void NeuralPow(const float * src, size_t size, const float * exponent, float * dst);

        void NeuralUpdateWeights(const float * x, size_t size, const float * a, const float * b, float * d, float * w);

        void NeuralAdaptiveGradientUpdate(const float * delta, size_t size, size_t batch, const float * alpha, const float * epsilon, float * gradient, float * weight);

        void NeuralAddConvolution2x2Forward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution3x3Forward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution4x4Forward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution5x5Forward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution2x2Backward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution3x3Backward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution4x4Backward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution5x5Backward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution2x2Sum(const float * src, size_t srcStride, const float * dst, size_t dstStride, size_t width, size_t height, float * sums);

        void NeuralAddConvolution3x3Sum(const float * src, size_t srcStride, const float * dst, size_t dstStride, size_t width, size_t height, float * sums);

        void NeuralAddConvolution4x4Sum(const float * src, size_t srcStride, const float * dst, size_t dstStride, size_t width, size_t height, float * sums);

        void NeuralAddConvolution5x5Sum(const float * src, size_t srcStride, const float * dst, size_t dstStride, size_t width, size_t height, float * sums);

        void NeuralPooling1x1Max3x3(const float * src, size_t srcStride, size_t width, size_t height, float * dst, size_t dstStride);

        void NeuralPooling2x2Max2x2(const float * src, size_t srcStride, size_t width, size_t height, float * dst, size_t dstStride);

        void NeuralPooling2x2Max3x3(const float * src, size_t srcStride, size_t width, size_t height, float * dst, size_t dstStride);

        void NeuralConvolutionForward(const float * src, size_t srcWidth, size_t srcHeight, size_t srcDepth, const float * weight,
            size_t kernelX, size_t kernelY, size_t padX, size_t padY, size_t strideX, size_t strideY, size_t dilationX, size_t dilationY,
            void * buffer, size_t * size, float * dst, size_t dstWidth, size_t dstHeight, size_t dstDepth, int add);

        void SquaredDifferenceSum32f(const float * a, const float * b, size_t size, float * sum);

        void SquaredDifferenceKahanSum32f(const float * a, const float * b, size_t size, float * sum);

        void SvmSumLinear(const float * x, const float * svs, const float * weights, size_t length, size_t count, float * sum);
    }
#endif// SIMD_AVX512F_ENABLE
}
#endif//__SimdAvx512f_h__
