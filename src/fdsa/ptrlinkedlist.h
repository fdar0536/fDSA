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

#include "include/internal/defines.h"
#include "include/internal/ptrlinkedlist.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct ptrLinkedListNode
{
    uint8_t *data;

    struct ptrLinkedListNode *priv;

    struct ptrLinkedListNode *next;
} ptrLinkedListNode;

typedef struct ptrLinkedList
{
    fdsa_types id;

    uint8_t magic[4];

    ptrLinkedListNode *root;

    fdsa_freeFunc dataFreeFunc;
} ptrLinkedList;

fdsa_handle fdsa_ptrLinkedList_create(fdsa_freeFunc);

fdsa_exitstate fdsa_ptrLinkedList_destory(fdsa_handle);

void fdsa_ptrLinkedList_clear(fdsa_handle);

fdsa_exitstate fdsa_ptrLinkedList_pushFront(fdsa_handle, void *);

void *fdsa_ptrLinkedList_popFront(fdsa_handle);

fdsa_exitstate fdsa_ptrLinkedList_pushBack(fdsa_handle, void *);

void *fdsa_ptrLinkedList_popBack(fdsa_handle);

fdsa_exitstate fdsa_ptrLinkedList_insertAfter(fdsa_handle,
                                              fdsa_cmpFunc,
                                              void *);

fdsa_exitstate fdsa_ptrLinkedList_removeAfter(fdsa_handle,
                                              fdsa_cmpFunc);

fdsa_exitstate fdsa_ptrLinkedList_insertBefore(fdsa_handle,
                                               fdsa_cmpFunc,
                                               void *);

fdsa_exitstate fdsa_ptrLinkedList_removeBefore(fdsa_handle,
                                               fdsa_cmpFunc);

ptrLinkedListNode *createLinkedListNode();

#ifdef __cplusplus
}
#endif
