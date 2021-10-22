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

fdsa_exitstate fdsa_ptrMap_init(fdsa_ptrMap_api *map)
{
    if (!map)
    {
        return fdsa_failed;
    }

    map->create = fdsa_ptrMap_create;
    map->destory = fdsa_ptrMap_destroy;
    map->at = fdsa_ptrMap_at;
    map->insertNode = fdsa_ptrMap_insertNode;
    map->deleteNode = fdsa_ptrMap_deleteNode;
    return fdsa_success;
}

fdsa_ptrMap *fdsa_ptrMap_create(fdsa_cmpFunc keyCmpFunc,
                                fdsa_freeFunc keyFreeFunc,
                                fdsa_freeFunc valueFreeFunc)
{
    if (!keyCmpFunc)
    {
        return NULL;
    }

    fdsa_ptrMap *ret = calloc(1, sizeof(fdsa_ptrMap));
    if (!ret)
    {
        return NULL;
    }

    // create nil
    ret->nil = calloc(1, sizeof(ptrRBTreeNode));
    if (!ret->nil)
    {
        free(ret);
        return NULL;
    }

    ret->nil->color = ptrRBTreeNodeColor_black; // nil must be black
    ret->nil->parent = ret->nil;
    ret->nil->left = ret->nil;
    ret->nil->right = ret->nil;

    ret->root = ret->nil;
    ret->keyCmpFunc = keyCmpFunc;
    ret->keyFreeFunc = keyFreeFunc;
    ret->valueFreeFunc = valueFreeFunc;

    return ret;
}

fdsa_exitstate fdsa_ptrMap_destroy(fdsa_ptrMap *tree)
{
    if (!tree)
    {
        return fdsa_failed;
    }

    fdsa_ptrMap_clear(tree, tree->root);

    // now root has been destroyed
    destroyPtrRBTreeNode(tree->nil, tree->keyFreeFunc, tree->valueFreeFunc);
    free(tree);
    return fdsa_success;
}

void *fdsa_ptrMap_at(fdsa_ptrMap *tree, void *key)
{
    if (!tree || !key)
    {
        return NULL;
    }

    ptrRBTreeNode *res = fdsa_ptrMap_searchNode(tree, key);
    if (res == tree->nil)
    {
        return NULL;
    }

    return res->value;
}

fdsa_exitstate fdsa_ptrMap_insertNode(fdsa_ptrMap *tree, void *key, void *value)
{
    if (!tree || !key)
    {
        return fdsa_failed;
    }

    ptrRBTreeNode *res = fdsa_ptrMap_searchNode(tree, key);
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
    ptrRBTreeNode *insert_node = createPtrRBTreeNode(tree->nil);
    if (!insert_node)
    {
        return fdsa_failed;
    }

    insert_node->key = key;
    insert_node->value = value;

    ptrRBTreeNode *y = tree->nil;
    ptrRBTreeNode *x = tree->root;

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

fdsa_exitstate fdsa_ptrMap_deleteNode(fdsa_ptrMap *tree, void *key)
{
    if (!tree || !key)
    {
        return fdsa_failed;
    }

    ptrRBTreeNode *delete_node = fdsa_ptrMap_searchNode(tree, key);
    if (delete_node == tree->nil)
    {
        return fdsa_failed;
    }

    ptrRBTreeNode *y = NULL; // to be deleted
    ptrRBTreeNode *x = NULL; // y's child

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
    ptrRBTreeNodeColor color = y->color;
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
        destroyPtrRBTreeNode(y, tree->keyFreeFunc, tree->valueFreeFunc);
    }

    if (color == ptrRBTreeNodeColor_black)
    {
        fdsa_ptrMap_deleteFixedUp(tree, x);
    }

    return fdsa_success;
} // end fdsa_ptrMap_deleteNode

ptrRBTreeNode *createPtrRBTreeNode(ptrRBTreeNode *nil)
{
    if (!nil)
    {
        return NULL;
    }

    ptrRBTreeNode *ret = calloc(1, sizeof(ptrRBTreeNode));
    if (!ret)
    {
        return NULL;
    }

    ret->key = NULL;
    ret->value = NULL;
    ret->color = ptrRBTreeNodeColor_rad; // the default new node is rad
    ret->parent = nil;
    ret->left = nil;
    ret->right = nil;

    return ret;
}

void destroyPtrRBTreeNode(ptrRBTreeNode *node,
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

void fdsa_ptrMap_clear(fdsa_ptrMap *tree,
                       ptrRBTreeNode *in)
{
    // use LRV traversal to clean up
    if (in == tree->nil)
    {
        return;
    }

    fdsa_ptrMap_clear(tree, in->left);
    fdsa_ptrMap_clear(tree, in->right);
    destroyPtrRBTreeNode(in, tree->keyFreeFunc, tree->valueFreeFunc);
}

void fdsa_ptrMap_leftRotation(fdsa_ptrMap *tree, ptrRBTreeNode *x)
{
    ptrRBTreeNode *y = x->right;

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

void fdsa_ptrMap_rightRotation(fdsa_ptrMap *tree, ptrRBTreeNode *y)
{
    ptrRBTreeNode *x = y->left;

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

ptrRBTreeNode *fdsa_ptrMap_searchNode(fdsa_ptrMap *tree, void *key)
{
    ptrRBTreeNode *current = tree->root;
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

void fdsa_ptrMap_insertFixedUp(fdsa_ptrMap *tree, ptrRBTreeNode *current)
{
    // case0: the parent is black, so no need to enter the loop
    // in the other word
    // if the parent is rad, need to enter the loop
    while (current->parent->color == ptrRBTreeNodeColor_rad)
    {
        // the parent is the grandparent's left
        if (current->parent == current->parent->parent->left)
        {
            ptrRBTreeNode *uncle = current->parent->parent->right;
            // case1: uncle is rad
            if (uncle->color == ptrRBTreeNodeColor_rad)
            {
                current->parent->color = ptrRBTreeNodeColor_black;
                uncle->color = ptrRBTreeNodeColor_black;
                current->parent->parent->color = ptrRBTreeNodeColor_rad;
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
                current->parent->color = ptrRBTreeNodeColor_black;
                current->parent->parent->color = ptrRBTreeNodeColor_rad;
                fdsa_ptrMap_rightRotation(tree, current->parent->parent);
            }
        }
        // the parent is the grandparent's right
        else
        {
            ptrRBTreeNode *uncle = current->parent->parent->left;
            // case1: uncle is rad
            if (uncle->color == ptrRBTreeNodeColor_rad)
            {
                current->parent->color = ptrRBTreeNodeColor_black;
                uncle->color = ptrRBTreeNodeColor_black;
                current->parent->parent->color = ptrRBTreeNodeColor_rad;
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
                current->parent->color = ptrRBTreeNodeColor_black;
                current->parent->parent->color = ptrRBTreeNodeColor_rad;
                fdsa_ptrMap_leftRotation(tree, current->parent->parent);
            }
        }
    } // end while (current->parent->color == rad)

    tree->root->color = ptrRBTreeNodeColor_black; // insure root is black
} // end fdsa_ptrMap_insertFixedUp

ptrRBTreeNode *fdsa_ptrMap_nodeLeftmost(fdsa_ptrMap *tree, ptrRBTreeNode *current)
{
    while (current->left != tree->nil)
    {
        current = current->left;
    }

    return current;
}

ptrRBTreeNode *fdsa_ptrMap_nodeSuccessor(fdsa_ptrMap *tree, ptrRBTreeNode *current)
{
    if (current->right != tree->nil)
    {
        return fdsa_ptrMap_nodeLeftmost(tree, current->right);
    }

    ptrRBTreeNode *new_node = current->parent;

    while (new_node != tree->nil && current == new_node->right)
    {
        current = new_node;
        new_node = new_node->parent;
    }

    return new_node;
} // end fdsa_ptrMap_nodeSuccessor

void fdsa_ptrMap_deleteFixedUp(fdsa_ptrMap *tree, ptrRBTreeNode *current)
{
    // Case0:(i) if current is rad, let is black
    //       (ii) if current is root, let it black
    while (current != tree->root && current->color == ptrRBTreeNodeColor_black)
    {
        // current is left
        if (current == current->parent->left)
        {
            ptrRBTreeNode *sibling = current->parent->right;
            // Case1: sibling is rad
            if (sibling->color == ptrRBTreeNodeColor_rad)
            {
                sibling->color = ptrRBTreeNodeColor_black;
                current->parent->color = ptrRBTreeNodeColor_rad;
                fdsa_ptrMap_leftRotation(tree, current->parent);
                sibling = current->parent->right;
            }

            // Case2、3、4: sibling is black
            // Case2: both sibling's children are black
            if (sibling->left->color == ptrRBTreeNodeColor_black &&
                sibling->right->color == ptrRBTreeNodeColor_black)
            {
                sibling->color = ptrRBTreeNodeColor_rad;

                // if current becomes the root after the update,
                // then exit the loop.
                current = current->parent;
            }
            // Case3 & 4: one of the sibling's child is black
            else
            {
                // case3: the sibling's right child is black,
                //        the other one is rad.
                if (sibling->right->color == ptrRBTreeNodeColor_black)
                {
                    sibling->left->color = ptrRBTreeNodeColor_black;
                    sibling->color = ptrRBTreeNodeColor_rad;
                    fdsa_ptrMap_rightRotation(tree, sibling);
                    sibling = current->parent->right;
                }

                // After case3, it must enter case4.
                // case4: the sibling's right child is rad,
                //        the other one is black.
                sibling->color = current->parent->color;
                current->parent->color = ptrRBTreeNodeColor_black;
                sibling->right->color = ptrRBTreeNodeColor_black;
                fdsa_ptrMap_leftRotation(tree, current->parent);

                // if current becomes the root after the update,
                // then exit the loop.
                current = tree->root;
            }
        }
        // current is right
        else
        {
            ptrRBTreeNode *sibling = current->parent->left;
            // Case1: sibling is rad
            if (sibling->color == ptrRBTreeNodeColor_rad)
            {
                sibling->color = ptrRBTreeNodeColor_black;
                current->parent->color = ptrRBTreeNodeColor_rad;
                fdsa_ptrMap_rightRotation(tree, current->parent);
                sibling = current->parent->left;
            }

            // Case2 3 4: sibling is black
            // Case2: both sibling's children are black
            if (sibling->left->color == ptrRBTreeNodeColor_black &&
                sibling->right->color == ptrRBTreeNodeColor_black)
            {
                sibling->color = ptrRBTreeNodeColor_rad;

                // if current becomes the root after the update,
                // then exit the loop.
                current = current->parent;
            }
            // Case3 & 4: one of the sibling's child is black
            else
            {
                // case3: the sibling's left child is black,
                //        the other one is rad.
                if (sibling->left->color == ptrRBTreeNodeColor_black)
                {
                    sibling->right->color = ptrRBTreeNodeColor_black;
                    sibling->color = ptrRBTreeNodeColor_rad;
                    fdsa_ptrMap_leftRotation(tree, sibling);
                    sibling = current->parent->left;
                }

                // After case3, it must enter case4.
                // case4: the sibling's left child is rad,
                //        the other one is black.
                sibling->color = current->parent->color;
                current->parent->color = ptrRBTreeNodeColor_black;
                sibling->left->color = ptrRBTreeNodeColor_black;
                fdsa_ptrMap_rightRotation(tree, current->parent);

                // if current becomes the root after the update,
                // then exit the loop.
                current = tree->root;
            }
        } // end if (current == current->parent->left)
    } // end while (current != tree->root && current->color == black)

    current->color = ptrRBTreeNodeColor_black; // insure root is black
} // end fdsa_ptrMap_deleteFixedUp;
