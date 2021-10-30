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

typedef struct fdsa_ptrMap
{
    ptrRBTreeNode *root;

    ptrRBTreeNode *nil;

    fdsa_cmpFunc keyCmpFunc;

    fdsa_freeFunc keyFreeFunc;

    fdsa_freeFunc valueFreeFunc;
} fdsa_ptrMap;

fdsa_exitstate fdsa_ptrMap_init(fdsa_ptrMap_api *);

fdsa_ptrMap *fdsa_ptrMap_create(fdsa_cmpFunc,
                                fdsa_freeFunc,
                                fdsa_freeFunc);

fdsa_exitstate fdsa_ptrMap_destroy(fdsa_ptrMap *);

fdsa_exitstate fdsa_ptrMap_isEmpty(fdsa_ptrMap *, uint8_t *);

void *fdsa_ptrMap_at(fdsa_ptrMap *, void *);

fdsa_exitstate fdsa_ptrMap_insertNode(fdsa_ptrMap *, void *, void *);

fdsa_exitstate fdsa_ptrMap_deleteNode(fdsa_ptrMap *, void *);

ptrRBTreeNode *createPtrRBTreeNode(ptrRBTreeNode *nil);

void destroyPtrRBTreeNode(ptrRBTreeNode *, fdsa_freeFunc, fdsa_freeFunc);

void fdsa_ptrMap_clear(fdsa_ptrMap *, ptrRBTreeNode *);

void fdsa_ptrMap_leftRotation(fdsa_ptrMap *, ptrRBTreeNode *);

void fdsa_ptrMap_rightRotation(fdsa_ptrMap *, ptrRBTreeNode *);

ptrRBTreeNode *fdsa_ptrMap_searchNode(fdsa_ptrMap *, void *);

void fdsa_ptrMap_insertFixedUp(fdsa_ptrMap *, ptrRBTreeNode *);

ptrRBTreeNode *fdsa_ptrMap_nodeLeftmost(fdsa_ptrMap *, ptrRBTreeNode *);

ptrRBTreeNode *fdsa_ptrMap_nodeSuccessor(fdsa_ptrMap *, ptrRBTreeNode *);

void fdsa_ptrMap_deleteFixedUp(fdsa_ptrMap *, ptrRBTreeNode *);

#ifdef __cplusplus
}
#endif
