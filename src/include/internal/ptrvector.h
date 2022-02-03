/*
 * This file is part of fDSA,
 * Copyright(C) 2019-2021 fdar0536.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <inttypes.h>
#include <stddef.h>

#include "defines.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct fdsa_ptrVector fdsa_ptrVector;

typedef struct fdsa_ptrVector_api
{
    fdsa_ptrVector *(*create)(fdsa_freeFunc freeFunc);

    fdsa_exitstate (*destory)(fdsa_ptrVector *ptrVector);

    void *(*at)(fdsa_ptrVector *ptrVector, size_t index);

    fdsa_exitstate (*setValue)(fdsa_ptrVector *ptrVector,
                               size_t index,
                               void *src);

    fdsa_exitstate (*clear)(fdsa_ptrVector *ptrVector);

    fdsa_exitstate (*size)(fdsa_ptrVector *ptrVector, size_t *dst);

    fdsa_exitstate (*capacity)(fdsa_ptrVector *ptrVector, size_t *dst);

    fdsa_exitstate (*reserve)(fdsa_ptrVector *ptrVector, size_t newSize);

    fdsa_exitstate (*pushBack)(fdsa_ptrVector *ptrVector, void *src);

    fdsa_exitstate (*resize)(fdsa_ptrVector *vector,
                             size_t newSize,
                             void *src,
                             void *(*deepCopyFunc)(void *));
} fdsa_ptrVector_api;

FDSA_API fdsa_ptrVector *fdsa_ptrVector_create(fdsa_freeFunc freeFunc);

FDSA_API fdsa_exitstate fdsa_ptrVector_destroy(fdsa_ptrVector *ptrVector);

FDSA_API void *fdsa_ptrVector_at(fdsa_ptrVector *ptrVector, size_t index);

FDSA_API fdsa_exitstate fdsa_ptrVector_setValue(fdsa_ptrVector *ptrVector,
                                                size_t index,
                                                void *src);

FDSA_API fdsa_exitstate fdsa_ptrVector_clear(fdsa_ptrVector *ptrVector);

FDSA_API fdsa_exitstate fdsa_ptrVector_size(fdsa_ptrVector *ptrVector,
                                            size_t *dst);

FDSA_API fdsa_exitstate fdsa_ptrVector_capacity(fdsa_ptrVector *ptrVector,
                                                size_t *dst);

FDSA_API fdsa_exitstate fdsa_ptrVector_reserve(fdsa_ptrVector *ptrVector,
                                               size_t newSize);

FDSA_API fdsa_exitstate fdsa_ptrVector_pushBack(fdsa_ptrVector *ptrVector,
                                                void *src);

FDSA_API fdsa_exitstate fdsa_ptrVector_resize(fdsa_ptrVector *vector,
                                              size_t newSize,
                                              void *src,
                                              void *(*deepCopyFunc)(void *));

#ifdef __cplusplus
}
#endif

