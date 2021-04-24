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

#include "config.h"
#include "fdsa.h"
#include "types.h"
#include "ptrmap.h"
#include "vector.h"
#include "ptrvector.h"

fdsa_exitstate fdsa_getHandleType(fdsa_handle in, fdsa_types *dst)
{
    if (!in)
    {
        return fdsa_failed;
    }

    ID *id = (ID *)in;
    *dst = id->id;

    return fdsa_success;
}

fdsa_exitstate fdsa_closeHandle(fdsa_handle in)
{
    if (!in)
    {
        return fdsa_failed;
    }

    ID *id = (ID *)in;
    switch(id->id)
    {
    case fdsa_types_ptrMap:
    {
        return fdsa_ptrMap_destroy(in);
    }
    case fdsa_types_ptrVector:
    {
        return fdsa_ptrVector_destroy(in);
    }
    case fdsa_types_vector:
    {
        return fdsa_vector_destroy(in);
    }
    default:
    {
        return fdsa_failed;
    }
    }
}

const char *fdsa_version()
{
    return PROJ_NAME" r"PROJ_VERSION;
}
