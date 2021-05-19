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

#include "defines.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct fdsa_ptrLinkedList
{
    fdsa_handle (*create)(fdsa_freeFunc dataFreeFunc);

    void (*clear)(fdsa_handle ptrLinkedList);

    fdsa_exitstate (*pushFront)(fdsa_handle ptrLinkedList, void *data);

    void *(*popFront)(fdsa_handle ptrLinkedList);

    fdsa_exitstate (*pushBack)(fdsa_handle ptrLinkedList, void *data);

    void *(*popBack)(fdsa_handle ptrLinkedList);

    fdsa_exitstate (*insertAfter)(fdsa_handle ptrLinkedList,
                                  fdsa_cmpFunc cmpFunc,
                                  void *data);

    fdsa_exitstate (*removeAfter)(fdsa_handle ptrLinkedList,
                                  fdsa_cmpFunc cmpFunc);

    fdsa_exitstate (*insertBefore)(fdsa_handle ptrLinkedList,
                                   fdsa_cmpFunc cmpFunc,
                                   void *data);

    fdsa_exitstate (*removeBefore)(fdsa_handle ptrLinkedList,
                                   fdsa_cmpFunc cmpFunc);

} fdsa_ptrLinkedList;

#ifdef __cplusplus
}
#endif
