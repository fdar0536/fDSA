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
#include "types.h"

fdsa_handle fdsa_ptrLinkedList_create(fdsa_freeFunc dataFreeFunc)
{
    if (!dataFreeFunc) return NULL;

    ptrLinkedList *ret = calloc(1, sizeof(ptrLinkedList));
    if (!ret) return NULL;

    ret->root = createLinkedListNode();
    if (!ret->root)
    {
        free(ret);
        return NULL;
    }

    ret->id = fdsa_types_ptrLinkedList;
    ret->magic[0] = 0xf;
    ret->magic[1] = 0xd;
    ret->magic[2] = 's';
    ret->magic[3] = 0xa;

    ret->dataFreeFunc = dataFreeFunc;
    ptrLinkedListNode *root = ret->root;
    root->priv = root;
    root->next = root;

    return ret;
}

fdsa_exitstate fdsa_ptrLinkedList_destory(fdsa_handle in)
{
    if (fdsa_checkInput(in, fdsa_types_ptrLinkedList))
    {
        return fdsa_failed;
    }

    ptrLinkedList *list = (ptrLinkedList *)in;
    ptrLinkedListNode *priv = list->root;
    ptrLinkedListNode *current = priv->next;

    while (current != list->root)
    {
        priv = current;
        current = current->next;

        list->dataFreeFunc(priv->data);
        free(priv);
    }

    // current == list->root
    free(current);
    free(list);
    return fdsa_success;
}

ptrLinkedListNode *createLinkedListNode()
{
    ptrLinkedListNode *ret = calloc(1, sizeof(ptrLinkedListNode));
    if (!ret) return NULL;

    ret->data = NULL;
    ret->next = NULL;
    ret->priv = NULL;

    return ret;
}
