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

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "fdsa/types.h"
#include "vector.h"

typedef struct vector
{
    fdsa_types id;

    uint8_t magic[4];

    uint8_t *data;

    size_t sizeOfData;

    size_t size;

    size_t capacity;
} vector;

fdsa_vector *fdsa_vector_init()
{
    fdsa_vector *ret = calloc(1, sizeof(fdsa_vector));
    if (!ret)
    {
        return NULL;
    }

    ret->create = fdsa_vector_create;
    ret->at = fdsa_vector_at;
    ret->setValue = fdsa_vector_setValue;
    ret->clear = fdsa_vector_clear;
    ret->size = fdsa_vector_size;
    ret->capacity = fdsa_vector_capacity;
    ret->reserve = fdsa_vector_reserve;
    ret->pushback = fdsa_vector_pushback;
    ret->resize = fdsa_vector_resize;

    return ret;
}

fdsa_handle fdsa_vector_create(size_t sizeOfData)
{
    if (!sizeOfData)
    {
        return NULL;
    }

    vector *vec = calloc(1, sizeof (vector));
    if (!vec)
    {
        return NULL;
    }

    vec->id = fdsa_types_vector;
    vec->magic[0] = 0xf;
    vec->magic[1] = 0xd;
    vec->magic[2] = 's';
    vec->magic[3] = 0xa;

    vec->data = NULL;
    vec->sizeOfData = sizeOfData;
    vec->size = 0;
    vec->capacity = 0;

    return vec;
}

fdsa_exitstate fdsa_vector_destroy(fdsa_handle in)
{
    if (fdsa_checkInput(in, fdsa_types_vector))
    {
        return fdsa_failed;
    }

    vector *vec = (vector *)in;
    if (vec->data)
    {
        free(vec->data);
    }

    free(vec);
    return fdsa_success;
}

fdsa_exitstate fdsa_vector_at(fdsa_handle in, size_t index, void *dst)
{
    if (fdsa_checkInput(in, fdsa_types_vector))
    {
        return fdsa_failed;
    }

    if (!dst)
    {
        return fdsa_failed;
    }

    vector *vec = (vector *)in;
    if (index >= vec->size)
    {
        return fdsa_failed;
    }

    uint8_t *data = vec->data;
    data += (index * vec->sizeOfData);
    memcpy(dst, data, vec->sizeOfData);

    return fdsa_success;
}

fdsa_exitstate fdsa_vector_setValue(fdsa_handle in,
                                      size_t index,
                                      const void *src)
{
    if (fdsa_checkInput(in, fdsa_types_vector))
    {
        return fdsa_failed;
    }

    if (!src)
    {
        return fdsa_failed;
    }

    vector *vec = (vector *)in;
    if (index >= vec->size)
    {
        return fdsa_failed;
    }

    uint8_t *data = vec->data;
    data += (index * vec->sizeOfData);
    memcpy(data, src, vec->sizeOfData);

    return fdsa_success;
}

fdsa_exitstate fdsa_vector_clear(fdsa_handle in)
{
    if (fdsa_checkInput(in, fdsa_types_vector))
    {
        return fdsa_failed;
    }

    vector *vec = (vector *)in;
    vec->size = 0;
    return fdsa_success;
}

fdsa_exitstate fdsa_vector_size(fdsa_handle in, size_t *dst)
{
    if (fdsa_checkInput(in, fdsa_types_vector))
    {
        return fdsa_failed;
    }

    if (!dst)
    {
        return fdsa_failed;
    }

    vector *vec = (vector *)in;
    *dst = vec->size;

    return fdsa_success;
}

fdsa_exitstate fdsa_vector_capacity(fdsa_handle in, size_t *dst)
{
    if (fdsa_checkInput(in, fdsa_types_vector))
    {
        return fdsa_failed;
    }

    if (!dst)
    {
        return fdsa_failed;
    }

    vector *vec = (vector *)in;
    *dst = vec->capacity;

    return fdsa_success;
}

fdsa_exitstate fdsa_vector_reserve(fdsa_handle in, size_t newSize)
{
    if (fdsa_checkInput(in, fdsa_types_vector))
    {
        return fdsa_failed;
    }

    vector *vec = (vector *)in;
    if (newSize <= vec->capacity)
    {
        // do nothing
        return fdsa_success;
    }

    uint8_t *newData = calloc(newSize, vec->sizeOfData);
    if (!newData)
    {
        return fdsa_failed;
    }

    if (vec->data)
    {
        memcpy(newData, vec->data, vec->sizeOfData * vec->size);
        free(vec->data);
    }

    vec->data = newData;
    vec->capacity = newSize;

    return fdsa_success;
}

fdsa_exitstate fdsa_vector_pushback(fdsa_handle in, const void *src)
{
    if (fdsa_checkInput(in, fdsa_types_vector))
    {
        return fdsa_failed;
    }

    if (!src)
    {
        return fdsa_failed;
    }

    vector *vec = (vector *)in;
    if (vec->size == vec->capacity)
    {
        if (fdsa_vector_reserve(vec, vec->capacity + 1) == fdsa_failed)
        {
            return fdsa_failed;
        }
    }

    uint8_t *data = vec->data;
    data += (vec->size * vec->sizeOfData);
    memcpy(data, src, vec->sizeOfData);
    ++vec->size;

    return fdsa_success;
}

fdsa_exitstate fdsa_vector_resize(fdsa_handle in,
                                    size_t amount,
                                    const void *src)
{
    if (fdsa_checkInput(in, fdsa_types_vector))
    {
        return fdsa_failed;
    }

    if (!src)
    {
        return fdsa_failed;
    }

    vector *vec = (vector *)in;
    if (vec->capacity < amount)
    {
        if (fdsa_vector_reserve(in, amount) == fdsa_failed)
        {
            return fdsa_failed;
        }
    }

    if (vec->size > amount)
    {
        vec->size = amount;
    }
    else if (vec->size < amount)
    {
        // vec->capacity >= amount
        size_t i;
        for (i = vec->size; i < amount; ++i)
        {
            if (fdsa_vector_pushback(vec, src) == fdsa_failed)
            {
                fputs("fdsa_vector_resize: You should never see this line.",
                      stderr);
                return fdsa_failed;
            }
        }
    }

    return fdsa_success;
}
