/*****************************************************************************
Copyright (c) 2020 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*****************************************************************************/
#pragma once

/**
 * @file
 * @brief Metal and Metal Performance Shaders interoperation API
 */

#include "RadeonML.h"

#import <Metal/Metal.h>
#import <MetalPerformanceShaders/MetalPerformanceShaders.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a context from an existing Metal command queue.
 *
 * @param[in]  queue    A MTLCommandQueue instance.
 * @param[out] context  A pointer to a resulting context handle.
 *
 * @return A valid context handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if the @p queue is NULL or @p context is NULL,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The context should be released with rmlReleaseContext().
 */
RML_API_ENTRY rml_status rmlCreateContextFromMTLQueue(id<MTLCommandQueue> queue,
                                                      rml_context* context);

/**
 * Retrieves an MPS image from an existing.
 *
 * @param[in]  tensor A valid tensor handle.
 * @param[out] image  A pointer to a resulting MPS image.
 *
 * @return A valid context handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if the @p queue is NULL or @p context is NULL,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlGetMPSImageFromTensor(rml_tensor tensor, MPSImage** image);

#ifdef __cplusplus
} // extern "C"
#endif
