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

#include <stddef.h>

#include "defines.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct fdsa_vector fdsa_vector;

typedef struct fdsa_vector_api
{
    fdsa_vector *(*create)(size_t sizeOfData);

    fdsa_exitstate (*destory)(fdsa_vector *vector);

    fdsa_exitstate (*at)(fdsa_vector *vector, size_t index, void *dst);

    fdsa_exitstate (*setValue)(fdsa_vector *vector,
                               size_t index,
                               const void *src);

    fdsa_exitstate (*clear)(fdsa_vector *vector);

    fdsa_exitstate (*size)(fdsa_vector *vector, size_t *dst);

    fdsa_exitstate (*capacity)(fdsa_vector *vector, size_t *dst);

    fdsa_exitstate (*reserve)(fdsa_vector *vector, size_t newSize);

    fdsa_exitstate (*pushBack)(fdsa_vector *vector, const void *src);

    fdsa_exitstate (*resize)(fdsa_vector *vector,
                             size_t newSize,
                             const void *src);

    fdsa_exitstate (*append)(fdsa_vector *vector,
                             const void *dataArray,
                             size_t dataLen);

    const void *(*data)(fdsa_vector *vector);

    void *(*takeData)(fdsa_vector *vector);

} fdsa_vector_api;

FDSA_API fdsa_vector *fdsa_vector_create(size_t sizeOfData);

FDSA_API fdsa_exitstate fdsa_vector_destroy(fdsa_vector *vector);

FDSA_API fdsa_exitstate fdsa_vector_at(fdsa_vector *vector,
                                       size_t index,
                                       void *dst);

FDSA_API fdsa_exitstate fdsa_vector_setValue(fdsa_vector *vector,
                                             size_t index,
                                             const void *src);

FDSA_API fdsa_exitstate fdsa_vector_clear(fdsa_vector *vector);

FDSA_API fdsa_exitstate fdsa_vector_size(fdsa_vector *vector, size_t *dst);

FDSA_API fdsa_exitstate fdsa_vector_capacity(fdsa_vector *vector, size_t *dst);

FDSA_API fdsa_exitstate fdsa_vector_reserve(fdsa_vector *vector, size_t newSize);

FDSA_API fdsa_exitstate fdsa_vector_pushBack(fdsa_vector *vector,
                                             const void *src);

FDSA_API fdsa_exitstate fdsa_vector_resize(fdsa_vector *vector,
                                           size_t newSize,
                                           const void *src);

FDSA_API fdsa_exitstate fdsa_vector_append(fdsa_vector *vector,
                                           const void *dataArray,
                                           size_t dataLen);

FDSA_API const void *fdsa_vector_data(fdsa_vector *vector);

FDSA_API void *fdsa_vector_takeData(fdsa_vector *vector);

#ifdef __cplusplus
}
#endif
