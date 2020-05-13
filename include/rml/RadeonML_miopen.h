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
 * @brief MIOpen interoperation API
 */

#include "rml/RadeonML.h"

#ifdef __cplusplus
extern "C" {
#endif

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
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p context is invalid.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlSetMIOpenAutoTuningOn(rml_context context, rml_bool on);

#ifdef __cplusplus
} // extern "C"
#endif
