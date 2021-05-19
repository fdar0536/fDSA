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

#include "include/internal/ptrmap.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum ptrRBTreeNodeColor
{
    ptrRBTreeNodeColor_rad,
    ptrRBTreeNodeColor_black
} ptrRBTreeNodeColor;

typedef struct ptrRBTreeNode
{
    uint8_t *key;

    uint8_t *value;

    ptrRBTreeNodeColor color;

    struct ptrRBTreeNode *parent;

    struct ptrRBTreeNode *left;

    struct ptrRBTreeNode *right;
} ptrRBTreeNode;

typedef struct ptrRBTree
{
    fdsa_types id;

    uint8_t magic[4];

    ptrRBTreeNode *root;

    ptrRBTreeNode *nil;

    fdsa_cmpFunc keyCmpFunc;

    fdsa_freeFunc keyFreeFunc;

    fdsa_freeFunc valueFreeFunc;
} ptrRBTree;

fdsa_ptrMap *fdsa_ptrMap_init();

fdsa_handle fdsa_ptrMap_create(fdsa_cmpFunc,
                               fdsa_freeFunc,
                               fdsa_freeFunc);

fdsa_exitstate fdsa_ptrMap_destroy(fdsa_handle);

void *fdsa_ptrMap_at(fdsa_handle, void *);

fdsa_exitstate fdsa_ptrMap_insertNode(fdsa_handle, void *, void *);

fdsa_exitstate fdsa_ptrMap_deleteNode(fdsa_handle, void *);

ptrRBTreeNode *createNode(ptrRBTreeNode *nil);

void destroyNode(ptrRBTreeNode *, fdsa_freeFunc, fdsa_freeFunc);

void fdsa_ptrMap_clear(ptrRBTree *, ptrRBTreeNode *);

void fdsa_ptrMap_leftRotation(ptrRBTree *, ptrRBTreeNode *);

void fdsa_ptrMap_rightRotation(ptrRBTree *, ptrRBTreeNode *);

ptrRBTreeNode *fdsa_ptrMap_searchNode(ptrRBTree *, void *);

void fdsa_ptrMap_insertFixedUp(ptrRBTree *, ptrRBTreeNode *);

ptrRBTreeNode *fdsa_ptrMap_nodeLeftmost(ptrRBTree *, ptrRBTreeNode *);

ptrRBTreeNode *fdsa_ptrMap_nodeSuccessor(ptrRBTree *, ptrRBTreeNode *);

void fdsa_ptrMap_deleteFixedUp(ptrRBTree *, ptrRBTreeNode *);

#ifdef __cplusplus
}
#endif
