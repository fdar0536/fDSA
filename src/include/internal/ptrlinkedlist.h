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

typedef struct fdsa_ptrLinkedListNode
{
    void *data;
} fdsa_ptrLinkedListNode;

typedef struct fdsa_ptrLinkedList fdsa_ptrLinkedList;

typedef struct fdsa_ptrLinkedList_api
{
    fdsa_ptrLinkedList *(*create)(fdsa_freeFunc dataFreeFunc);

    fdsa_exitstate (*destory)(fdsa_ptrLinkedList *ptrLinkedList);

    void (*clear)(fdsa_ptrLinkedList *ptrLinkedList);

    fdsa_exitstate (*pushFront)(fdsa_ptrLinkedList *ptrLinkedList, void *data);

    void *(*popFront)(fdsa_ptrLinkedList *ptrLinkedList);

    fdsa_exitstate (*pushBack)(fdsa_ptrLinkedList *ptrLinkedList, void *data);

    void *(*popBack)(fdsa_ptrLinkedList *ptrLinkedList);

    fdsa_exitstate (*insertAfter)(fdsa_ptrLinkedList *ptrLinkedList,
                                  fdsa_ptrLinkedListNode *reference,
                                  void *data);

    fdsa_exitstate (*insertBefore)(fdsa_ptrLinkedList *ptrLinkedList,
                                   fdsa_ptrLinkedListNode *reference,
                                   void *data);

    fdsa_exitstate (*remove)(fdsa_ptrLinkedList *ptrLinkedList,
                             fdsa_ptrLinkedListNode *target);

    fdsa_ptrLinkedListNode *(*first)(fdsa_ptrLinkedList *ptrLinkedList);

    fdsa_ptrLinkedListNode *(*last)(fdsa_ptrLinkedList *ptrLinkedList);

    fdsa_ptrLinkedListNode *(*next)(fdsa_ptrLinkedList *ptrLinkedList,
                                    fdsa_ptrLinkedListNode *node);

    fdsa_ptrLinkedListNode *(*priv)(fdsa_ptrLinkedList *ptrLinkedList,
                                    fdsa_ptrLinkedListNode *node);

} fdsa_ptrLinkedList_api;

FDSA_API fdsa_ptrLinkedList *fdsa_ptrLinkedList_create(
        fdsa_freeFunc dataFreeFunc);

FDSA_API fdsa_exitstate fdsa_ptrLinkedList_destory(
        fdsa_ptrLinkedList *ptrLinkedList);

FDSA_API void fdsa_ptrLinkedList_clear(fdsa_ptrLinkedList *ptrLinkedList);

FDSA_API fdsa_exitstate fdsa_ptrLinkedList_pushFront(
        fdsa_ptrLinkedList *ptrLinkedList,
        void *data);

FDSA_API void *fdsa_ptrLinkedList_popFront(fdsa_ptrLinkedList *ptrLinkedList);

FDSA_API fdsa_exitstate fdsa_ptrLinkedList_pushBack(
        fdsa_ptrLinkedList *ptrLinkedList,
        void *data);

FDSA_API void *fdsa_ptrLinkedList_popBack(fdsa_ptrLinkedList *ptrLinkedList);

FDSA_API fdsa_exitstate fdsa_ptrLinkedList_insertAfter(
        fdsa_ptrLinkedList *ptrLinkedList,
        fdsa_ptrLinkedListNode *reference,
        void *data);

FDSA_API fdsa_exitstate fdsa_ptrLinkedList_insertBefore(
        fdsa_ptrLinkedList *ptrLinkedList,
        fdsa_ptrLinkedListNode *reference,
        void *data);

FDSA_API fdsa_exitstate fdsa_ptrLinkedList_remove(
        fdsa_ptrLinkedList *ptrLinkedList,
        fdsa_ptrLinkedListNode *target);


FDSA_API fdsa_ptrLinkedListNode *fdsa_ptrLinkedList_first(
        fdsa_ptrLinkedList *ptrLinkedList);

FDSA_API fdsa_ptrLinkedListNode *fdsa_ptrLinkedList_last(
        fdsa_ptrLinkedList *ptrLinkedList);

FDSA_API fdsa_ptrLinkedListNode *fdsa_ptrLinkedList_next(
        fdsa_ptrLinkedList *ptrLinkedList,
        fdsa_ptrLinkedListNode *node);

FDSA_API fdsa_ptrLinkedListNode *fdsa_ptrLinkedList_priv(
        fdsa_ptrLinkedList *ptrLinkedList,
        fdsa_ptrLinkedListNode *node);

#ifdef __cplusplus
}
#endif
