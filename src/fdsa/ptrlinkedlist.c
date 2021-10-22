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

fdsa_ptrLinkedList *fdsa_ptrLinkedList_create(fdsa_freeFunc dataFreeFunc)
{
    if (!dataFreeFunc) return NULL;

    fdsa_ptrLinkedList *ret = calloc(1, sizeof(fdsa_ptrLinkedList));
    if (!ret) return NULL;

    ret->root = createPtrLinkedListNode();
    if (!ret->root)
    {
        free(ret);
        return NULL;
    }

    ret->dataFreeFunc = dataFreeFunc;
    ptrLinkedListNode *root = ret->root;
    root->priv = root;
    root->next = root;

    return ret;
}

fdsa_exitstate fdsa_ptrLinkedList_destory(fdsa_ptrLinkedList *list)
{
    if (!list) return fdsa_failed;

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

fdsa_exitstate fdsa_ptrLinkedList_pushFront(fdsa_ptrLinkedList *list, void *data)
{
    if (!list) return fdsa_failed;

    ptrLinkedListNode *toBeInserted = createPtrLinkedListNode();
    if (!toBeInserted) return fdsa_failed;
    toBeInserted->data = data;

    ptrLinkedListNode *root = list->root;
    if (root->next == root)
    {
        // list is empty
        root->next = toBeInserted;
        root->priv = toBeInserted;
        toBeInserted->next = root;
        toBeInserted->priv = root;
    }
    else
    {
        ptrLinkedListNode *head = root->next;
        root->next = toBeInserted;

        toBeInserted->priv = root;
        toBeInserted->next = head;

        head->priv = toBeInserted;
    }

    return fdsa_success;
}

void *fdsa_ptrLinkedList_popFront(fdsa_ptrLinkedList *list)
{
    if (!list) return NULL;

    ptrLinkedListNode *root = list->root;
    if (root->next == root) /* list is empty */ return NULL;

    ptrLinkedListNode *head = root->next;
    ptrLinkedListNode *newHead = head->next;

    newHead->priv = root;
    root->next = newHead;

    uint8_t *ret = head->data;
    free(head);

    return ret;
}

fdsa_exitstate fdsa_ptrLinkedList_pushBack(fdsa_ptrLinkedList *list, void *data)
{
    if (!list) return fdsa_failed;

    ptrLinkedListNode *toBeInserted = createPtrLinkedListNode();
    if (!toBeInserted) return fdsa_failed;
    toBeInserted->data = data;

    ptrLinkedListNode *root = list->root;
    if (root->next == root)
    {
        // list is empty
        root->next = toBeInserted;
        root->priv = toBeInserted;
        toBeInserted->next = root;
        toBeInserted->priv = root;
    }
    else
    {
        ptrLinkedListNode *tail = root->priv;

        root->priv = toBeInserted;

        toBeInserted->next = root;
        toBeInserted->priv = tail;

        tail->next = toBeInserted;
    }

    return fdsa_success;
}

void *fdsa_ptrLinkedList_popBack(fdsa_ptrLinkedList *list)
{
    if (!list) return NULL;

    ptrLinkedListNode *root = list->root;
    if (root->next == root) /* list is empty */ return NULL;

    ptrLinkedListNode *tail = root->priv;
    ptrLinkedListNode *newTail = tail->priv;

    newTail->next = root;
    root->priv = newTail;

    uint8_t *ret= tail->data;
    free(tail);

    return ret;
}

fdsa_exitstate fdsa_ptrLinkedList_insertAfter(fdsa_ptrLinkedList *list,
                                              fdsa_cmpFunc cmpFunc,
                                              void *data)
{
    if (!list || !cmpFunc) return fdsa_failed;

    ptrLinkedListNode *toBeInserted = createPtrLinkedListNode();
    if (!toBeInserted) return fdsa_failed;
    toBeInserted->data = data;

    ptrLinkedListNode *root = list->root;
    if (root->next == root)
    {
        // list is empty
        root->next = toBeInserted;
        root->priv = toBeInserted;
        toBeInserted->next = root;
        toBeInserted->priv = root;
    }
    else
    {
        ptrLinkedListNode *current = root->next;
        ptrLinkedListNode *next = current->next;
        uint8_t *currentData = NULL, *nextData = NULL;

        while (next != root)
        {
            currentData = current->data;
            nextData = next->data;
            if (!cmpFunc(currentData, nextData))
            {
                goto process;
            }

            current = next;
            next = next->next;
        }

process:
        current->next = toBeInserted;

        toBeInserted->priv = current;
        toBeInserted->next = next;

        next->priv = toBeInserted;

        if (next == root) return fdsa_successWithWarning;
    }

    return fdsa_success;
}

fdsa_exitstate fdsa_ptrLinkedList_removeAfter(fdsa_ptrLinkedList *list,
                                              fdsa_cmpFunc cmpFunc)
{
    if (!cmpFunc || !list) return fdsa_failed;

    ptrLinkedListNode *root = list->root;
    if (root->next == root)
    {
        // list is empty
        return fdsa_failed;
    }

    ptrLinkedListNode *current = root->next;
    ptrLinkedListNode *next = current->next;
    uint8_t *currentData = NULL, *nextData = NULL;
    while (next != root)
    {
        currentData = current->data;
        nextData = next->data;
        if (!cmpFunc(currentData, nextData))
        {
            goto process;
        }

        current = next;
        next = next->next;
    }

    // next == root
}

fdsa_exitstate fdsa_ptrLinkedList_insertBefore(fdsa_handle,
                                               fdsa_cmpFunc,
                                               void *);

fdsa_exitstate fdsa_ptrLinkedList_removeBefore(fdsa_handle,
                                               fdsa_cmpFunc);

ptrLinkedListNode *createPtrLinkedListNode()
{
    ptrLinkedListNode *ret = calloc(1, sizeof(ptrLinkedListNode));
    if (!ret) return NULL;

    ret->data = NULL;
    ret->next = NULL;
    ret->priv = NULL;

    return ret;
}
