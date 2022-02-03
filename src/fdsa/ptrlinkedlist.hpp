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

#include <mutex>

#include <cinttypes>

#include "include/internal/defines.h"
#include "include/internal/ptrlinkedlist.h"

typedef struct ptrLinkedListNode
{
    void *data;

    struct ptrLinkedListNode *priv;

    struct ptrLinkedListNode *next;
} ptrLinkedListNode;

typedef struct fdsa_ptrLinkedList
{
    ptrLinkedListNode *root;

    fdsa_freeFunc dataFreeFunc;

    std::mutex mutex;
} fdsa_ptrLinkedList;

#ifdef __cplusplus
extern "C"
{
#endif

fdsa_exitstate fdsa_ptrLinkedList_init(fdsa_ptrLinkedList_api *);

fdsa_ptrLinkedList *fdsa_ptrLinkedList_create(fdsa_freeFunc);

fdsa_exitstate fdsa_ptrLinkedList_destory(fdsa_ptrLinkedList *);

void fdsa_ptrLinkedList_clear(fdsa_ptrLinkedList *);

fdsa_exitstate fdsa_ptrLinkedList_pushFront(fdsa_ptrLinkedList *, void *);

void *fdsa_ptrLinkedList_popFront(fdsa_ptrLinkedList *);

fdsa_exitstate fdsa_ptrLinkedList_pushBack(fdsa_ptrLinkedList *, void *);

void *fdsa_ptrLinkedList_popBack(fdsa_ptrLinkedList *);

fdsa_exitstate fdsa_ptrLinkedList_insertAfter(fdsa_ptrLinkedList *,
                                              fdsa_ptrLinkedListNode *,
                                              void *);

fdsa_exitstate fdsa_ptrLinkedList_insertBefore(fdsa_ptrLinkedList *,
                                               fdsa_ptrLinkedListNode *,
                                               void *);

fdsa_exitstate fdsa_ptrLinkedList_remove(fdsa_ptrLinkedList *,
                                         fdsa_ptrLinkedListNode *);


fdsa_ptrLinkedListNode *fdsa_ptrLinkedList_first(fdsa_ptrLinkedList *ptrLinkedList);

fdsa_ptrLinkedListNode *fdsa_ptrLinkedList_last(fdsa_ptrLinkedList *ptrLinkedList);

fdsa_ptrLinkedListNode *fdsa_ptrLinkedList_next(fdsa_ptrLinkedList *ptrLinkedList,
                                                fdsa_ptrLinkedListNode *node);

fdsa_ptrLinkedListNode *fdsa_ptrLinkedList_priv(fdsa_ptrLinkedList *ptrLinkedList,
                                                fdsa_ptrLinkedListNode *node);

ptrLinkedListNode *createPtrLinkedListNode();

#ifdef __cplusplus
}
#endif
