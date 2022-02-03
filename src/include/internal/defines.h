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

#ifdef __cplusplus
extern "C"
{
#endif
/**
 * @enum fdsa_exitstate
 * It defines the return state from fdsa APIs.
 */
typedef enum fdsa_exitstate
{
    fdsa_success, /**< success */
    fdsa_successWithWarning, /**< success but with warning */
    fdsa_failed /**< failed */
} fdsa_exitstate;

/**
 * @typedef fdsa_cmpFunc
 * A function for comparison
 * @param lhs
 * @param rhs
 * @return if the return value is less than 0, indicate lhs is less than rhs;
 *         if the return value is equal to 0, indicate lhs is equal to rhs;
 *         else the return value is greater than 0.
 */
typedef int (*fdsa_cmpFunc)(const void *lhs, const void *rhs);

/**
 * @typedef fdsa_freeFunc
 * A function for free memory
 * @param toBeFreed the pointer to be freed
 */
typedef void (*fdsa_freeFunc)(void *toBeFreed);

#if defined _WIN32 || defined __CYGWIN__
#ifdef __MINGW32__
#define FDSA_API __attribute__((dllexport))
#else
#define FDSA_API __declspec(dllexport)
#endif // __MINGW32__
#else
#define FDSA_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
}
#endif
