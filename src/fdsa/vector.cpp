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

#include <mutex>
#include <new>

#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "vector.h"

typedef struct fdsa_vector
{
    uint8_t *data = NULL;

    size_t sizeOfData = 0;

    size_t size = 0;

    size_t capacity = 0;

    std::mutex mutex;
} fdsa_vector;

extern "C"
{

fdsa_exitstate fdsa_vector_init(fdsa_vector_api *ret)
{
    if (!ret) return fdsa_failed;

    ret->create = fdsa_vector_create;
    ret->destory = fdsa_vector_destroy;
    ret->at = fdsa_vector_at;
    ret->setValue = fdsa_vector_setValue;
    ret->clear = fdsa_vector_clear;
    ret->size = fdsa_vector_size;
    ret->capacity = fdsa_vector_capacity;
    ret->reserve = fdsa_vector_reserve;
    ret->pushBack = fdsa_vector_pushBack;
    ret->resize = fdsa_vector_resize;
    ret->append = fdsa_vector_append;
    ret->data = fdsa_vector_data;
    ret->takeData = fdsa_vector_takeData;

    return fdsa_success;
}

fdsa_vector *fdsa_vector_create(size_t sizeOfData)
{
    if (!sizeOfData)
    {
        return NULL;
    }

    fdsa_vector *vec = new (std::nothrow) fdsa_vector;
    if (!vec)
    {
        return NULL;
    }

    vec->sizeOfData = sizeOfData;

    return vec;
}

fdsa_exitstate fdsa_vector_destroy(fdsa_vector *vec)
{
    if (!vec) return fdsa_failed;

    vec->mutex.lock();
    if (vec->data)
    {
        delete[] vec->data;
        vec->data = NULL;
    }

    vec->mutex.unlock();
    delete vec;
    return fdsa_success;
}

fdsa_exitstate fdsa_vector_at(fdsa_vector *vec, size_t index, void *dst)
{

    if (!vec || !dst)
    {
        return fdsa_failed;
    }

    std::lock_guard<std::mutex> lock(vec->mutex);
    if (index >= vec->size)
    {
        return fdsa_failed;
    }

    uint8_t *data = vec->data;
    data += (index * vec->sizeOfData);
    memcpy(dst, data, vec->sizeOfData);

    return fdsa_success;
}

fdsa_exitstate fdsa_vector_setValue(fdsa_vector *vec,
                                    size_t index,
                                    const void *src)
{
    if (!vec || !src)
    {
        return fdsa_failed;
    }

    std::lock_guard<std::mutex> lock(vec->mutex);
    if (index >= vec->size)
    {
        return fdsa_failed;
    }

    uint8_t *data = vec->data;
    data += (index * vec->sizeOfData);
    memcpy(data, src, vec->sizeOfData);

    return fdsa_success;
}

fdsa_exitstate fdsa_vector_clear(fdsa_vector *vec)
{
    if (!vec) return fdsa_failed;

    std::lock_guard<std::mutex> lock(vec->mutex);
    vec->size = 0;
    return fdsa_success;
}

fdsa_exitstate fdsa_vector_size(fdsa_vector *vec, size_t *dst)
{
    if (!vec || !dst)
    {
        return fdsa_failed;
    }

    std::lock_guard<std::mutex> lock(vec->mutex);
    *dst = vec->size;

    return fdsa_success;
}

fdsa_exitstate fdsa_vector_capacity(fdsa_vector *vec, size_t *dst)
{
    if (!vec || !dst)
    {
        return fdsa_failed;
    }

    std::lock_guard<std::mutex> lock(vec->mutex);
    *dst = vec->capacity;

    return fdsa_success;
}

fdsa_exitstate fdsa_vector_reserve(fdsa_vector *vec, size_t newSize)
{
    if (!vec) return fdsa_failed;

    std::lock_guard<std::mutex> lock(vec->mutex);
    if (newSize <= vec->capacity)
    {
        // do nothing
        return fdsa_success;
    }

    uint8_t *newData = new (std::nothrow) uint8_t[newSize * vec->sizeOfData]();
    if (!newData)
    {
        return fdsa_failed;
    }

    if (vec->data)
    {
        memcpy(newData, vec->data, vec->sizeOfData * vec->size);
        delete[] vec->data;
    }

    vec->data = newData;
    vec->capacity = newSize;

    return fdsa_success;
}

fdsa_exitstate fdsa_vector_pushBack(fdsa_vector *vec, const void *src)
{
    if (!vec || !src)
    {
        return fdsa_failed;
    }

    std::lock_guard<std::mutex> lock(vec->mutex);
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

fdsa_exitstate fdsa_vector_resize(fdsa_vector *vec,
                                  size_t amount,
                                  const void *src)
{
    if (!vec || !src)
    {
        return fdsa_failed;
    }

    if (vec->capacity < amount)
    {
        if (fdsa_vector_reserve(vec, amount) == fdsa_failed)
        {
            return fdsa_failed;
        }
    }

    if (vec->size > amount)
    {
        std::lock_guard<std::mutex> lock(vec->mutex);
        vec->size = amount;
    }
    else if (vec->size < amount)
    {
        // vec->capacity >= amount
        size_t i;
        for (i = vec->size; i < amount; ++i)
        {
            if (fdsa_vector_pushBack(vec, src) == fdsa_failed)
            {
                fputs("fdsa_vector_resize: You should never see this line.",
                      stderr);
                return fdsa_failed;
            }
        }
    }

    return fdsa_success;
}

fdsa_exitstate fdsa_vector_append(fdsa_vector *vec,
                                  const void *in,
                                  size_t inLen)
{
    if (!vec || !in || !inLen) return fdsa_failed;

    std::lock_guard<std::mutex> lock(vec->mutex);
    if (fdsa_vector_reserve(vec, vec->capacity + inLen) == fdsa_failed)
    {
        return fdsa_failed;
    }

    uint8_t *data = vec->data;
    data += (vec->size * vec->sizeOfData);

    memcpy(data, in, vec->sizeOfData * inLen);
    vec->size += inLen;
    return fdsa_success;
}

const void *fdsa_vector_data(fdsa_vector *vec)
{
    if (!vec) return NULL;

    std::lock_guard<std::mutex> lock(vec->mutex);
    return vec->data;
}

void *fdsa_vector_takeData(fdsa_vector *vec)
{
    if (!vec) return NULL;

    std::lock_guard<std::mutex> lock(vec->mutex);
    uint8_t *ret = vec->data;

    vec->capacity = 0;
    vec->size = 0;
    vec->data = NULL;
    return ret;
}

} // end extern "C"
