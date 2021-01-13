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
 * @brief Metal interoperation API
 */

#include "rml/RadeonML.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a context from an existing Metal command queue.
 *
 * @param[in]  queue   An MTLCommandQueue instance (id<MTLCommandQueue>).
 * @param[out] context A pointer to a resulting context handle.
 *
 * @return A valid context handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if the @p queue is NULL or the @p context is NULL,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The context should be released with rmlReleaseContext().
 */
RML_API_ENTRY rml_status rmlCreateContextFromMTLQueue(void* queue, rml_context* context);

/**
 * Creates a tensor from an existing Metal buffer.
 *
 * @param[in]  context A valid context handle.
 * @param[in]  buffer  An MTLBuffer instance (id<MTLBuffer>).
 * @param[in]  info    A tensor description with all dimensions specified.
 * @param[in]  mode    A tensor access mode.
 * @param[out] tensor  A pointer to a resulting tensor handle.
 *
 * @return A valid tensor handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if the @p context is invalid, the @p buffer is NULL, the @p info is
 * NULL or invalid, the @p mode is invalid or the @p tensor is NULL,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The tensor should be released with rmlReleaseTensor().
 */
RML_API_ENTRY rml_status rmlCreateTensorFromMTLBuffer(rml_context context,
                                                      void* buffer,
                                                      const rml_tensor_info* info,
                                                      rml_access_mode mode,
                                                      rml_tensor* tensor);

/**
 * Get underlying Metal buffer from a tensor.
 *
 * @param[in]  tensor  A valid tensor handle.
 * @param[out] buffer  A pointer to a resulting Metal buffer (id<MTLBuffer>).
 *
 * @return A non-NULL Metal buffer in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p tensor is invalid or @p buffer is NULL.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The resulting buffer is reference counted by the @p tensor.
 */
RML_API_ENTRY rml_status rmlGetMTLBufferFromTensor(rml_tensor tensor,
                                                   void** buffer /* id<MTLBuffer>* */);

#ifdef __cplusplus
} // extern "C"
#endif
