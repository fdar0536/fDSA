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

#include "defines.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct fdsa_ptrMap fdsa_ptrMap;

typedef struct fdsa_ptrMap_api
{
    fdsa_ptrMap *(*create)(fdsa_cmpFunc keyCmpFunc,
                           fdsa_freeFunc keyFreeFunc,
                           fdsa_freeFunc valueFreeFunc);

    fdsa_exitstate (*destory)(fdsa_ptrMap *);

    fdsa_exitstate (*isEmpty)(fdsa_ptrMap *map, uint8_t *res);

    void *(*at)(fdsa_ptrMap *map, void *key);

    fdsa_exitstate (*insertNode)(fdsa_ptrMap *map, void *key, void *value);

    fdsa_exitstate (*deleteNode)(fdsa_ptrMap *map, void *key);
} fdsa_ptrMap_api;

#ifdef __cplusplus
}
#endif
