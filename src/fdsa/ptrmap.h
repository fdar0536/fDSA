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

typedef struct ptrRBTreeNode ptrRBTreeNode;

typedef struct fdsa_ptrMap fdsa_ptrMap;

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum ptrRBTreeNodeColor
{
    ptrRBTreeNodeColor_rad,
    ptrRBTreeNodeColor_black
} ptrRBTreeNodeColor;

fdsa_exitstate fdsa_ptrMap_init(fdsa_ptrMap_api *);

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
