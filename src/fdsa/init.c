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

#include "include/fdsa.h"
#include "fdsa.h"
#include "ptrmap.h"
#include "ptrvector.h"
#include "vector.h"

FDSA_API fDSA *fdsa_init()
{
    fDSA *ret = calloc(1, sizeof(fDSA));
    if (!ret)
    {
        return NULL;
    }

    ret->ptrMap = NULL;
    ret->ptrVector = NULL;
    ret->vector = NULL;

    ret->ptrMap = fdsa_ptrMap_init();
    if (!ret->ptrMap)
    {
        fdsa_destroy(ret);
        return NULL;
    }

    ret->ptrVector = fdsa_ptrVector_init();
    if (!ret->ptrVector)
    {
        fdsa_destroy(ret);
        return NULL;
    }

    ret->vector = fdsa_vector_init();
    if (!ret->vector)
    {
        fdsa_destroy(ret);
        return NULL;
    }

    ret->getHandleType = fdsa_getHandleType;
    ret->closeHandle = fdsa_closeHandle;
    ret->version = fdsa_version;
    return ret;
}

FDSA_API void fdsa_destroy(fDSA *in)
{
    if (!in)
    {
        return;
    }

    if (in->ptrMap) free(in->ptrMap);
    if (in->ptrVector) free(in->ptrVector);
    if (in->vector) free(in->vector);

    free(in);
}
