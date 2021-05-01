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

#include <stdlib.h>

#include "ptrlinkedlist.h"

fdsa_handle fdsa_ptrLinkedList_create(fdsa_freeFunc dataFreeFunc)
{
    if (!dataFreeFunc) return NULL;

    List *ret = calloc(1, sizeof(List));
    if (!ret) return NULL;

    ret->root = createListNode();
    if (!ret->root)
    {
        free(ret);
        return NULL;
    }

    ret->id = fdsa_types_ptrLinkedList;
    ret->dataFreeFunc = dataFreeFunc;
    ListNode *root = ret->root;
    root->priv = root;
    root->next = root;

    return ret;
}

ListNode *createListNode()
{
    ListNode *ret = calloc(1, sizeof(ListNode));
    if (!ret) return NULL;

    ret->data = NULL;
    ret->next = NULL;
    ret->priv = NULL;

    return ret;
}
