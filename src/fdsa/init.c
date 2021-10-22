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

FDSA_API fdsa_exitstate fdsa_init(fDSA *ret)
{
    if (!ret)
    {
        return fdsa_failed;
    }

    if (fdsa_ptrMap_init(&ret->ptrMap) == fdsa_failed)
    {
        return fdsa_failed;
    }

    if (fdsa_ptrVector_init(&ret->ptrVector) == fdsa_failed)
    {
        return fdsa_failed;
    }

    if (fdsa_vector_init(&ret->vector) == fdsa_failed)
    {
        return fdsa_failed;
    }

    ret->version = fdsa_version;
    return fdsa_success;
}
