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
 * @brief TensorFlow specific API
 */

#include <RadeonML.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Enable access by a context and its models to the specified devices only.
 * The function affects every model subsequently created with the context.
 *
 * @param[in] context     A valid context handle.
 * @param[in] num_devices Number of devices in the array.
 * @param[in] devices     A pointer to an array with zero-based device indices.
 *
 * @return RML_OK in case of success, RML_FAIL otherwise.
 *         To get more details in case of failure, call rmlGetLastError().
 */

RML_API_ENTRY rml_status rmlSetVisibleDevices(rml_context context,
                                              size_t num_devices,
                                              const unsigned* devices);

/**
 * Set a fraction of GPU memory allowed to use by a context and its models.
 * The function affects every model subsequently created with the context.
 *
 * @param[in] context  A valid context handle.
 * @param[in] fraction Memory fraction to use, in the range (0, 1].
 *
 * @return RML_OK in case of success, RML_FAIL otherwise.
 *         To get more details in case of failure, call rmlGetLastError().
 */

RML_API_ENTRY rml_status rmlSetGPUMemoryFraction(rml_context context, float fraction);

#ifdef __cplusplus
} // extern "C"
#endif
