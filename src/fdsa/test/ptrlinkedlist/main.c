/*
 * This file is part of fDSA,
 * Copyright(C) 2020-2021 fdar0536.
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
#include <stdio.h>

#include "fdsa.h"

typedef struct Testing
{
    int a;
    int b;
} Testing;

Testing *createTesting()
{
    Testing *ret = calloc(1, sizeof(Testing));
    if (!ret)
    {
        return NULL;
    }

    return ret;
}

void freeTesting(void *in)
{
    if (!in)
    {
        return;
    }

    Testing *testing = (Testing *)in;
    free(testing);
}

void printTesting(Testing *data)
{
    printf("data->a = %d, data->b = %d\n", data->a, data->b);
}

fdsa_exitstate dumpData(fdsa_ptrLinkedList_api *listApi,
                        fdsa_ptrLinkedList *list)
{
    fdsa_ptrLinkedListNode *node = listApi->first(list);
    if (!node)
    {
        puts("List is empty!");
        return fdsa_failed;
    }

    Testing *data = NULL;

    while(1)
    {
        data = node->data;
        if (!data)
        {
            puts("Fail to dump data.");
            return fdsa_failed;
        }

        printTesting(data);

        node = listApi->next(list, node);
        if (!node) break;
    }

    printf("\n");

    node = listApi->last(list);
    while(1)
    {
        data = node->data;
        if (!data)
        {
            puts("Fail to dump data.");
            return fdsa_failed;
        }

        printf("data->a = %d, data->b = %d\n", data->a, data->b);

        node = listApi->priv(list, node);
        if (!node) break;
    }

    printf("\n");
    return fdsa_success;
}

int main()
{
    fDSA api;
    if (fdsa_init(&api) == fdsa_failed)
    {
        fputs("Fail to create api entry.\n", stderr);
        return 1;
    }

    fdsa_ptrLinkedList_api *listApi = &api.ptrLinkedList;

    fdsa_ptrLinkedList *list = listApi->create(freeTesting);
    if (!list)
    {
        fputs("Fail to create list.\n", stderr);
        return 1;
    }

    Testing *data = createTesting();
    if (!data)
    {
        fputs("Fail to allocate memory.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    if (listApi->pushBack(list, data) == fdsa_failed)
    {
        fputs("Fail to pushBack.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    data = createTesting();
    if (!data)
    {
        fputs("Fail to allocate memory.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    data->a = 1;
    data->b = 2;
    if (listApi->pushBack(list, data) == fdsa_failed)
    {
        fputs("Fail to pushBack.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    data = createTesting();
    if (!data)
    {
        fputs("Fail to allocate memory.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    data->a = 2;
    data->b = 3;
    if (listApi->pushFront(list, data) == fdsa_failed)
    {
        fputs("Fail to pushFront.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    if (dumpData(listApi, list) == fdsa_failed)
    {
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    data = createTesting();
    if (!data)
    {
        fputs("Fail to allocate memory.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    data->a = 5;
    data->b = 5;
    if (listApi->pushBack(list, data) == fdsa_failed)
    {
        fputs("Fail to pushBack.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    if (dumpData(listApi, list) == fdsa_failed)
    {
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory vector.\n", stderr);
        }

        return 1;
    }

    data = listApi->popBack(list);
    if (!data)
    {
        fputs("Fail to popBack.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    printTesting(data);
    free(data);
    printf("\n");
    if (dumpData(listApi, list) == fdsa_failed)
    {
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    data = listApi->popFront(list);
    if (!data)
    {
        fputs("Fail to popBack.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    printTesting(data);
    free(data);
    printf("\n");
    if (dumpData(listApi, list) == fdsa_failed)
    {
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    fdsa_ptrLinkedListNode *node = listApi->first(list);
    if (!node)
    {
        fputs("Fail to get first node.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    data = createTesting();
    if (!data)
    {
        fputs("Fail to allocate memory.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    data->a = 7;
    data->b = 8;
    if (listApi->insertAfter(list, node, data) == fdsa_failed)
    {
        fputs("Fail to insert after node", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        freeTesting(data);
        return 1;
    }

    data = createTesting();
    if (!data)
    {
        fputs("Fail to allocate memory.", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    data->a = 9;
    data->b = 6;
    if (listApi->insertBefore(list, node, data) == fdsa_failed)
    {
        fputs("Fail to insert before node", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        free(data);
        return 1;
    }

    if (dumpData(listApi, list) == fdsa_failed)
    {
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    if (listApi->remove(list, node) == fdsa_failed)
    {
        fputs("Fail to insert remove node", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    if (dumpData(listApi, list) == fdsa_failed)
    {
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    listApi->clear(list);
    if (dumpData(listApi, list) == fdsa_success) // here MUST be fail
    {
        fputs("Fail to clean.\n", stderr);
        if (listApi->destory(list) == fdsa_failed)
        {
            fputs("Fail to destory list.\n", stderr);
        }

        return 1;
    }

    if (listApi->destory(list) == fdsa_failed)
    {
        fputs("Fail to destory list.\n", stderr);

        return 1;
    }

    return 0;
}
