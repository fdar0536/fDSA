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

#include <new>

#include <cstdlib>

#include "ptrlinkedlist.hpp"

extern "C"
{

fdsa_exitstate fdsa_ptrLinkedList_init(fdsa_ptrLinkedList_api *ret)
{
    if (!ret) return fdsa_failed;

    ret->create = fdsa_ptrLinkedList_create;
    ret->destory = fdsa_ptrLinkedList_destory;
    ret->clear = fdsa_ptrLinkedList_clear;
    ret->pushFront = fdsa_ptrLinkedList_pushFront;
    ret->popFront = fdsa_ptrLinkedList_popFront;
    ret->pushBack = fdsa_ptrLinkedList_pushBack;
    ret->popBack = fdsa_ptrLinkedList_popBack;
    ret->insertAfter = fdsa_ptrLinkedList_insertAfter;
    ret->insertBefore = fdsa_ptrLinkedList_insertBefore;
    ret->remove = fdsa_ptrLinkedList_remove;
    ret->first = fdsa_ptrLinkedList_first;
    ret->last = fdsa_ptrLinkedList_last;
    ret->next = fdsa_ptrLinkedList_next;
    ret->priv = fdsa_ptrLinkedList_priv;

    return fdsa_success;
}

fdsa_ptrLinkedList *fdsa_ptrLinkedList_create(fdsa_freeFunc dataFreeFunc)
{
    fdsa_ptrLinkedList *ret = new (std::nothrow) fdsa_ptrLinkedList;
    if (!ret) return NULL;

    ret->root = createPtrLinkedListNode();
    if (!ret->root)
    {
        delete ret;
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
    fdsa_ptrLinkedList_clear(list);

    // clean up root and object.
    delete list->root;
    delete list;

    return fdsa_success;
}

void fdsa_ptrLinkedList_clear(fdsa_ptrLinkedList *list)
{
    if (!list) return;
    std::lock_guard<std::mutex> lock(list->mutex);

    ptrLinkedListNode *priv = list->root;
    ptrLinkedListNode *current = priv->next;

    while (current != list->root)
    {
        priv = current;
        current = current->next;

        if (list->dataFreeFunc) list->dataFreeFunc(priv->data);
        delete priv;
    }

    // current == list->root
    current->next = current;
    current->priv = current;
}

fdsa_exitstate fdsa_ptrLinkedList_pushFront(fdsa_ptrLinkedList *list, void *data)
{
    if (!list) return fdsa_failed;

    std::lock_guard<std::mutex> lock(list->mutex);
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

    std::lock_guard<std::mutex> lock(list->mutex);
    ptrLinkedListNode *root = list->root;
    if (root->next == root) /* list is empty */ return NULL;

    ptrLinkedListNode *head = root->next;
    ptrLinkedListNode *newHead = head->next;

    newHead->priv = root;
    root->next = newHead;

    uint8_t *ret = reinterpret_cast<uint8_t *>(head->data);
    delete head;

    return ret;
}

fdsa_exitstate fdsa_ptrLinkedList_pushBack(fdsa_ptrLinkedList *list, void *data)
{
    if (!list) return fdsa_failed;

    std::lock_guard<std::mutex> lock(list->mutex);
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

    std::lock_guard<std::mutex> lock(list->mutex);
    ptrLinkedListNode *root = list->root;
    if (root->next == root) /* list is empty */ return NULL;

    ptrLinkedListNode *tail = root->priv;
    ptrLinkedListNode *newTail = tail->priv;

    newTail->next = root;
    root->priv = newTail;

    uint8_t *ret= reinterpret_cast<uint8_t *>(tail->data);
    delete tail;

    return ret;
}

fdsa_exitstate fdsa_ptrLinkedList_insertAfter(fdsa_ptrLinkedList *list,
                                              fdsa_ptrLinkedListNode *ref,
                                              void *data)
{
    if (!list || !ref) return fdsa_failed;

    std::lock_guard<std::mutex> lock(list->mutex);
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
        ptrLinkedListNode *current = reinterpret_cast<ptrLinkedListNode *>(ref);
        ptrLinkedListNode *next = current->next;

        current->next = toBeInserted;

        toBeInserted->priv = current;
        toBeInserted->next = next;

        next->priv = toBeInserted;
    }

    return fdsa_success;
}

fdsa_exitstate fdsa_ptrLinkedList_insertBefore(fdsa_ptrLinkedList *list,
                                               fdsa_ptrLinkedListNode *ref,
                                               void *data)
{
    if (!list || !ref) return fdsa_failed;

    std::lock_guard<std::mutex> lock(list->mutex);
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
        ptrLinkedListNode *current = reinterpret_cast<ptrLinkedListNode *>(ref);
        ptrLinkedListNode *priv = current->priv;

        current->priv = toBeInserted;

        toBeInserted->priv = priv;
        toBeInserted->next = current;

        priv->next = toBeInserted;
    }

    return fdsa_success;
}

fdsa_exitstate fdsa_ptrLinkedList_remove(fdsa_ptrLinkedList *list,
                                         fdsa_ptrLinkedListNode *ref)
{
    if (!list || !ref) return fdsa_failed;

    std::lock_guard<std::mutex> lock(list->mutex);
    ptrLinkedListNode *root = list->root;
    if (root->next == root)
    {
        // list is empty
        return fdsa_failed;
    }

    ptrLinkedListNode *toBeRemoved = reinterpret_cast<ptrLinkedListNode *>(ref);
    if (toBeRemoved == root) return fdsa_failed;

    ptrLinkedListNode *priv = toBeRemoved->priv;
    ptrLinkedListNode *next = toBeRemoved->next;

    priv->next = next;
    next->priv = priv;

    // clean up
    if (list->dataFreeFunc) list->dataFreeFunc(toBeRemoved->data);
    delete toBeRemoved;

    return fdsa_success;
}

fdsa_ptrLinkedListNode *fdsa_ptrLinkedList_first(fdsa_ptrLinkedList *list)
{
    if (!list) return NULL;

    std::lock_guard<std::mutex> lock(list->mutex);
    if (list->root->next == list->root)
    {
        // list is empty
        return NULL;
    }

    return reinterpret_cast<fdsa_ptrLinkedListNode *>(list->root->next);
}

fdsa_ptrLinkedListNode *fdsa_ptrLinkedList_last(fdsa_ptrLinkedList *list)
{
    if (!list) return NULL;

    std::lock_guard<std::mutex> lock(list->mutex);
    if (list->root->next == list->root)
    {
        // list is empty
        return NULL;
    }

    return reinterpret_cast<fdsa_ptrLinkedListNode *>(list->root->priv);
}

fdsa_ptrLinkedListNode *fdsa_ptrLinkedList_next(fdsa_ptrLinkedList *list,
                                                fdsa_ptrLinkedListNode *ref)
{
    if (!list || !ref) return NULL;

    std::lock_guard<std::mutex> lock(list->mutex);
    if (list->root->next == list->root)
    {
        // list is empty
        return NULL;
    }

    ptrLinkedListNode *node = reinterpret_cast<ptrLinkedListNode *>(ref);
    if (node->next == list->root) return NULL;
    return reinterpret_cast<fdsa_ptrLinkedListNode *>(node->next);
}

fdsa_ptrLinkedListNode *fdsa_ptrLinkedList_priv(fdsa_ptrLinkedList *list,
                                                fdsa_ptrLinkedListNode *ref)
{
    if (!list || !ref) return NULL;

    std::lock_guard<std::mutex> lock(list->mutex);
    if (list->root->next == list->root)
    {
        // list is empty
        return NULL;
    }

    ptrLinkedListNode *node = reinterpret_cast<ptrLinkedListNode *>(ref);
    if (node->priv == list->root) return NULL;
    return reinterpret_cast<fdsa_ptrLinkedListNode *>(node->priv);
}

ptrLinkedListNode *createPtrLinkedListNode()
{
    ptrLinkedListNode *ret = new (std::nothrow) ptrLinkedListNode;
    if (!ret) return NULL;

    ret->data = NULL;
    ret->next = NULL;
    ret->priv = NULL;

    return ret;
}

} // end extern "C"
