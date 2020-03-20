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
 * @brief OpenCL ineroperation API
 */

#include "RadeonML.h"

#if defined(__APPLE__) || defined(__MACOSX)
#    include <OpenCL/cl.h>
#else
#    include <CL/cl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a context from an OpenCL context.
 *
 * @param[in]  queue   An OpenCL command queue handle.
 * @param[out] context A pointer to a resulting context handle.
 *
 * @return A valid tensor handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p queue is NULL or @p context is NULL.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The context should be released with rmlReleaseContext().
 */

RML_API_ENTRY rml_status rmlCreateContextFromClQueue(cl_command_queue queue, rml_context* context);

/**
 * Creates a tensor from an OpenCL buffer.
 *
 * @param[in]  context A valid context handle.
 * @param[in]  buffer  A valid OpenCL memory object handle.
 * @param[in]  info    A tensor description with all dimensions specified.
 * @param[in]  mode    A tensor data access mode.
 * @param[out] tensor  A pointer to a resulting tensor handle.
 *
 * @return A valid tensor handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p context is invalid, @p buffer,
 *      @p info or @ p tensor is NULL or @p mode is invalid.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The context should be released with rmlReleaseTensor().
 */

RML_API_ENTRY rml_status rmlCreateTensorFromClBuffer(rml_context context,
                                                     cl_mem buffer,
                                                     const rml_tensor_info* info,
                                                     rml_access_mode mode,
                                                     rml_tensor* tensor);

/**
 * MIOpen can run in 2 different ways.
 * It can either search for the best convolution algorithm for a given problem size
 * or avoid that search.
 * This mechanism is called auto tuning.
 * By default it is off as it is a costly operation. But an application may want to use for
 * performance reason
 * https://github.com/ROCmSoftwarePlatform/MIOpen/blob/master/doc/src/perfdatabase.md#auto-tuning-the-kernels
 *
 * @param[in] context A valid context handle.
 * @param[in] on      A boolean indicating if the auto tuning state for MIOpen convolution is on.
 *
 * @return A valid tensor handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p context is invalid.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlSetMIOpenAutoTuningOn(rml_context context, rml_bool on);

#ifdef __cplusplus
} // extern "C"
#endif
