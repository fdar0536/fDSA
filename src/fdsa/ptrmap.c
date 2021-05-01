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

#include "ptrmap.h"
#include "fdsa/types.h"

fdsa_ptrMap *fdsa_ptrMap_init()
{
    fdsa_ptrMap *map = calloc(1, sizeof(fdsa_ptrMap));
    if (!map)
    {
        return NULL;
    }

    map->create = fdsa_ptrMap_create;
    map->at = fdsa_ptrMap_at;
    map->insertNode = fdsa_ptrMap_insertNode;
    map->deleteNode = fdsa_ptrMap_deleteNode;
    return map;
}

fdsa_handle fdsa_ptrMap_create(fdsa_cmpFunc keyCmpFunc,
                               fdsa_freeFunc keyFreeFunc,
                               fdsa_freeFunc valueFreeFunc)
{
    if (!keyCmpFunc)
    {
        return NULL;
    }

    RBTree *ret = calloc(1, sizeof(RBTree));
    if (!ret)
    {
        return NULL;
    }

    // create nil
    ret->nil = calloc(1, sizeof(RBTreeNode));
    if (!ret->nil)
    {
        free(ret);
        return NULL;
    }

    ret->nil->key = NULL;
    ret->nil->value = NULL;
    ret->nil->color = RBTreeNodeColor_black; // nil must be black
    ret->nil->parent = ret->nil;
    ret->nil->left = ret->nil;
    ret->nil->right = ret->nil;

    ret->id = fdsa_types_ptrMap;
    ret->root = ret->nil;
    ret->keyCmpFunc = keyCmpFunc;
    ret->keyFreeFunc = keyFreeFunc;
    ret->valueFreeFunc = valueFreeFunc;

    return ret;
}

fdsa_exitstate fdsa_ptrMap_destroy(fdsa_handle in)
{
    if (fdsa_checkInput(in, fdsa_types_ptrMap))
    {
        return fdsa_failed;
    }

    RBTree *tree = (RBTree *)in;
    fdsa_ptrMap_clear(tree, tree->root);

    // now root has been destroyed
    destroyNode(tree->nil, tree->keyFreeFunc, tree->valueFreeFunc);
    free(tree);
    return fdsa_success;
}

void *fdsa_ptrMap_at(fdsa_handle in, void *key)
{
    if (fdsa_checkInput(in, fdsa_types_ptrMap))
    {
        return NULL;
    }

    if (!key)
    {
        return NULL;
    }

    RBTree *tree = (RBTree *)in;
    RBTreeNode *res = fdsa_ptrMap_searchNode(tree, key);
    if (res == tree->nil)
    {
        return NULL;
    }

    return res->value;
}

fdsa_exitstate fdsa_ptrMap_insertNode(fdsa_handle in, void *key, void *value)
{
    if (fdsa_checkInput(in, fdsa_types_ptrMap))
    {
        return fdsa_failed;
    }

    if (!key || !value)
    {
        return fdsa_failed;
    }

    RBTree *tree = (RBTree *)in;
    RBTreeNode *res = fdsa_ptrMap_searchNode(tree, key);
    if (res != tree->nil)
    {
        // entry is already exist
        if (tree->valueFreeFunc)
        {
            tree->valueFreeFunc(res->value);
        }

        res->value = value;
        return fdsa_success;
    }

    // here is the just simply BST insert
    RBTreeNode *insert_node = createNode(tree->nil);
    if (!insert_node)
    {
        return fdsa_failed;
    }

    insert_node->key = key;
    insert_node->value = value;

    RBTreeNode *y = tree->nil;
    RBTreeNode *x = tree->root;

    while (x != tree->nil)
    {
        y = x;
        if (tree->keyCmpFunc(insert_node->key, x->key) < 0)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    insert_node->parent = y;

    if (y == tree->nil)
    {
        tree->root = insert_node;
    }
    else if (tree->keyCmpFunc(insert_node->key, x->key) < 0)
    {
        y->left = insert_node;
    }
    else
    {
        y->right = insert_node;
    }

    fdsa_ptrMap_insertFixedUp(tree, insert_node);
    return fdsa_success;
} // end fdsa_ptrMap_insertNode

fdsa_exitstate fdsa_ptrMap_deleteNode(fdsa_handle in, void *key)
{
    if (fdsa_checkInput(in, fdsa_types_ptrMap))
    {
        return fdsa_failed;
    }

    if (!key)
    {
        return fdsa_failed;
    }

    RBTree *tree = (RBTree *)in;

    RBTreeNode *delete_node = fdsa_ptrMap_searchNode(tree, key);
    if (delete_node == tree->nil)
    {
        return fdsa_failed;
    }

    RBTreeNode *y = NULL; // to be deleted
    RBTreeNode *x = NULL; // y's child

    if (delete_node->left == tree->nil || delete_node->right == tree->nil)
    {
        y = delete_node;
    }
    else
    {
        y = fdsa_ptrMap_nodeSuccessor(tree, delete_node);
    }

    if (y->left != tree->nil)
    {
        x = y->left;
    }
    else
    {
        x = y->right;
    }

    if (x != tree->nil)
    {
        x->parent = y->parent;
    }

    if (y->parent == tree->nil)
    {
        tree->root = x;
    }
    else if (y == y->parent->left)
    {
        y->parent->left = x;
    }
    else
    {
        y->parent->right = x;
    }

    // for case3
    RBTreeNodeColor color = y->color;
    if (y != delete_node)
    {
        // note that delete_node is not nil
        if (tree->keyFreeFunc) tree->keyFreeFunc(delete_node->key);
        delete_node->key = y->key;

        if (tree->valueFreeFunc) tree->valueFreeFunc(delete_node->value);
        delete_node->value = y->value;

        free(y);
    }
    else
    {
        // simply destroy y
        destroyNode(y, tree->keyFreeFunc, tree->valueFreeFunc);
    }

    if (color == RBTreeNodeColor_black)
    {
        fdsa_ptrMap_deleteFixedUp(tree, x);
    }

    return fdsa_success;
} // end fdsa_ptrMap_deleteNode

RBTreeNode *createRBTreeNode(RBTreeNode *nil)
{
    if (!nil)
    {
        return NULL;
    }

    RBTreeNode *ret = calloc(1, sizeof(RBTreeNode));
    if (!ret)
    {
        return NULL;
    }

    ret->key = NULL;
    ret->value = NULL;
    ret->color = RBTreeNodeColor_rad; // the default new node is rad
    ret->parent = nil;
    ret->left = nil;
    ret->right = nil;

    return ret;
}

void destroyNode(RBTreeNode *node,
                 fdsa_freeFunc keyFreeFunc,
                 fdsa_freeFunc valueFreeFunc)
{
    if (!node)
    {
        return;
    }

    if (node->key)
    {
        if (keyFreeFunc) keyFreeFunc(node->key);
    }

    if (node->value)
    {
        if (valueFreeFunc) valueFreeFunc(node->value);
    }

    free(node);
}

void fdsa_ptrMap_clear(RBTree *tree,
                       RBTreeNode *in)
{
    // use LRV traversal to clean up
    if (in == tree->nil)
    {
        return;
    }

    fdsa_ptrMap_clear(tree, in->left);
    fdsa_ptrMap_clear(tree, in->right);
    destroyNode(in, tree->keyFreeFunc, tree->valueFreeFunc);
}

void fdsa_ptrMap_leftRotation(RBTree *tree, RBTreeNode *x)
{
    RBTreeNode *y = x->right;

    x->right = y->left;

    if (y->left != tree->nil)
    {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == tree->nil)
    {
        tree->root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void fdsa_ptrMap_rightRotation(RBTree *tree, RBTreeNode *y)
{
    RBTreeNode *x = y->left;

    y->left = x->right;
    if (x->right != tree->nil)
    {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == tree->nil)
    {
        tree->root = x;
    }
    else if (y == y->parent->left)
    {
        y->parent->left = x;
    }
    else
    {
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}

RBTreeNode *fdsa_ptrMap_searchNode(RBTree *tree, void *key)
{
    RBTreeNode *current = tree->root;
    int res;
    while (current != tree->nil)
    {
        res = tree->keyCmpFunc(key, current->key);
        if (res == 0) // key == current->key
        {
            break;
        }
        else if (res < 0) // key < current->key
        {
            current = current->left;
        }
        else // key > current->key
        {
            current = current->right;
        }
    }

    return current;
} // end fdsa_ptrMap_searchNode

void fdsa_ptrMap_insertFixedUp(RBTree *tree, RBTreeNode *current)
{
    // case0: the parent is black, so no need to enter the loop
    // in the other word
    // if the parent is rad, need to enter the loop
    while (current->parent->color == RBTreeNodeColor_rad)
    {
        // the parent is the grandparent's left
        if (current->parent == current->parent->parent->left)
        {
            RBTreeNode *uncle = current->parent->parent->right;
            // case1: uncle is rad
            if (uncle->color == RBTreeNodeColor_rad)
            {
                current->parent->color = RBTreeNodeColor_black;
                uncle->color = RBTreeNodeColor_black;
                current->parent->parent->color = RBTreeNodeColor_rad;
                current = current->parent->parent;
            }
            // case2 & 3: uncle is black
            else
            {
                if (current == current->parent->right) // case2
                {
                    current = current->parent;
                    fdsa_ptrMap_leftRotation(tree, current);
                }

                // case3
                current->parent->color = RBTreeNodeColor_black;
                current->parent->parent->color = RBTreeNodeColor_rad;
                fdsa_ptrMap_rightRotation(tree, current->parent->parent);
            }
        }
        // the parent is the grandparent's right
        else
        {
            RBTreeNode *uncle = current->parent->parent->left;
            // case1: uncle is rad
            if (uncle->color == RBTreeNodeColor_rad)
            {
                current->parent->color = RBTreeNodeColor_black;
                uncle->color = RBTreeNodeColor_black;
                current->parent->parent->color = RBTreeNodeColor_rad;
                current = current->parent->parent;
            }
            // case2 & 3: uncle is black
            else
            {
                if (current == current->parent->left) // case2
                {
                    current = current->parent;
                    fdsa_ptrMap_rightRotation(tree, current);
                }

                // case3
                current->parent->color = RBTreeNodeColor_black;
                current->parent->parent->color = RBTreeNodeColor_rad;
                fdsa_ptrMap_leftRotation(tree, current->parent->parent);
            }
        }
    } // end while (current->parent->color == rad)

    tree->root->color = RBTreeNodeColor_black; // insure root is black
} // end fdsa_ptrMap_insertFixedUp

RBTreeNode *fdsa_ptrMap_nodeLeftmost(RBTree *tree, RBTreeNode *current)
{
    while (current->left != tree->nil)
    {
        current = current->left;
    }

    return current;
}

RBTreeNode *fdsa_ptrMap_nodeSuccessor(RBTree *tree, RBTreeNode *current)
{
    if (current->right != tree->nil)
    {
        return fdsa_ptrMap_nodeLeftmost(tree, current->right);
    }

    RBTreeNode *new_node = current->parent;

    while (new_node != tree->nil && current == new_node->right)
    {
        current = new_node;
        new_node = new_node->parent;
    }

    return new_node;
} // end fdsa_ptrMap_nodeSuccessor

void fdsa_ptrMap_deleteFixedUp(RBTree *tree, RBTreeNode *current)
{
    // Case0:(i) if current is rad, let is black
    //       (ii) if current is root, let it black
    while (current != tree->root && current->color == RBTreeNodeColor_black)
    {
        // current is left
        if (current == current->parent->left)
        {
            RBTreeNode *sibling = current->parent->right;
            // Case1: sibling is rad
            if (sibling->color == RBTreeNodeColor_rad)
            {
                sibling->color = RBTreeNodeColor_black;
                current->parent->color = RBTreeNodeColor_rad;
                fdsa_ptrMap_leftRotation(tree, current->parent);
                sibling = current->parent->right;
            }

            // Case2、3、4: sibling is black
            // Case2: both sibling's children are black
            if (sibling->left->color == RBTreeNodeColor_black &&
                sibling->right->color == RBTreeNodeColor_black)
            {
                sibling->color = RBTreeNodeColor_rad;

                // if current becomes the root after the update,
                // then exit the loop.
                current = current->parent;
            }
            // Case3 & 4: one of the sibling's child is black
            else
            {
                // case3: the sibling's right child is black,
                //        the other one is rad.
                if (sibling->right->color == RBTreeNodeColor_black)
                {
                    sibling->left->color = RBTreeNodeColor_black;
                    sibling->color = RBTreeNodeColor_rad;
                    fdsa_ptrMap_rightRotation(tree, sibling);
                    sibling = current->parent->right;
                }

                // After case3, it must enter case4.
                // case4: the sibling's right child is rad,
                //        the other one is black.
                sibling->color = current->parent->color;
                current->parent->color = RBTreeNodeColor_black;
                sibling->right->color = RBTreeNodeColor_black;
                fdsa_ptrMap_leftRotation(tree, current->parent);

                // if current becomes the root after the update,
                // then exit the loop.
                current = tree->root;
            }
        }
        // current is right
        else
        {
            RBTreeNode *sibling = current->parent->left;
            // Case1: sibling is rad
            if (sibling->color == RBTreeNodeColor_rad)
            {
                sibling->color = RBTreeNodeColor_black;
                current->parent->color = RBTreeNodeColor_rad;
                fdsa_ptrMap_rightRotation(tree, current->parent);
                sibling = current->parent->left;
            }

            // Case2 3 4: sibling is black
            // Case2: both sibling's children are black
            if (sibling->left->color == RBTreeNodeColor_black &&
                sibling->right->color == RBTreeNodeColor_black)
            {
                sibling->color = RBTreeNodeColor_rad;

                // if current becomes the root after the update,
                // then exit the loop.
                current = current->parent;
            }
            // Case3 & 4: one of the sibling's child is black
            else
            {
                // case3: the sibling's left child is black,
                //        the other one is rad.
                if (sibling->left->color == RBTreeNodeColor_black)
                {
                    sibling->right->color = RBTreeNodeColor_black;
                    sibling->color = RBTreeNodeColor_rad;
                    fdsa_ptrMap_leftRotation(tree, sibling);
                    sibling = current->parent->left;
                }

                // After case3, it must enter case4.
                // case4: the sibling's left child is rad,
                //        the other one is black.
                sibling->color = current->parent->color;
                current->parent->color = RBTreeNodeColor_black;
                sibling->left->color = RBTreeNodeColor_black;
                fdsa_ptrMap_rightRotation(tree, current->parent);

                // if current becomes the root after the update,
                // then exit the loop.
                current = tree->root;
            }
        } // end if (current == current->parent->left)
    } // end while (current != tree->root && current->color == black)

    current->color = RBTreeNodeColor_black; // insure root is black
} // end fdsa_ptrMap_deleteFixedUp;
