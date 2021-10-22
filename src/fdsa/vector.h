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

#include "include/internal/vector.h"

#ifdef __cplusplus
extern "C"
{
#endif

fdsa_exitstate fdsa_vector_init(fdsa_vector_api *);

fdsa_vector *fdsa_vector_create(size_t sizeOfData);

fdsa_exitstate fdsa_vector_destroy(fdsa_vector *vector);

fdsa_exitstate fdsa_vector_at(fdsa_vector *vector, size_t index, void *dst);

fdsa_exitstate fdsa_vector_setValue(fdsa_vector *vector, size_t, const void *);

fdsa_exitstate fdsa_vector_clear(fdsa_vector *vector);

fdsa_exitstate fdsa_vector_size(fdsa_vector *vector, size_t *);

fdsa_exitstate fdsa_vector_capacity(fdsa_vector *vector, size_t *);

fdsa_exitstate fdsa_vector_reserve(fdsa_vector *vector, size_t);

fdsa_exitstate fdsa_vector_pushBack(fdsa_vector *vector, const void *);

fdsa_exitstate fdsa_vector_resize(fdsa_vector *vector, size_t, const void *);

#ifdef __cplusplus
}
#endif
