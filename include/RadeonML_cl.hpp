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

#include "rml/RadeonML.hpp"
#include "rml/RadeonML_cl.h"

#define RML_CHECK_STATUS(OP) ::rml::details::CheckStatus(OP == RML_OK, #OP)

namespace rml {

inline Context CreateContextFromClQueue(void* queue /* cl_command_queue */)
{
    rml_context context = nullptr;
    RML_CHECK_STATUS(rmlCreateContextFromClQueue(queue, &context));
    return Context(context);
}

inline Tensor CreateTensorFromClBuffer(const Context& context,
                                       const rml_tensor_info& info,
                                       void* buffer /* cl_mem */,
                                       rml_access_mode mode)
{
    rml_tensor tensor = nullptr;
    RML_CHECK_STATUS(rmlCreateTensorFromClBuffer(context, buffer, &info, mode, &tensor));
    return Tensor(tensor);
}

} // namespace rml

#undef RML_CHECK_STATUS
