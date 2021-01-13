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
 * @brief Direct3D 12 interoperation API
 */

#include "rml/RadeonML.hpp"
#include "rml/RadeonML_d3d12.h"

#define RML_CHECK_STATUS(OP) ::rml::details::CheckStatus(OP == RML_OK, #OP)

namespace rml {

inline Context CreateContextFromD3DQueue(void* command_queue /* ID3D12CommandQueue* */)
{
    rml_context context = nullptr;
    RML_CHECK_STATUS(rmlCreateContextFromD3DQueue(command_queue, &context));
    return Context(context);
}

inline Tensor CreateTensorFromD3DResource(const Context& context,
                                          void* resource /* ID3D12Resource* */,
                                          const rml_tensor_info& info)
{
    rml_tensor tensor = nullptr;
    RML_CHECK_STATUS(rmlCreateTensorFromD3DResource(context(), resource, &info, &tensor));
    return Tensor(tensor);
}

inline void* GetD3DResourceFromTensor(const Tensor& tensor)
{
    void* resource = nullptr;
    RML_CHECK_STATUS(rmlGetD3DResourceFromTensor(tensor(), &resource));
    return resource /* ID3D12Resource* */;
}

inline void SetD3DCommandList(const Context& context,
                              void* command_list /* ID3D12GraphicsCommandList* */,
                              void* command_allocator /* ID3D12CommandAllocator* */)
{
    RML_CHECK_STATUS(rmlSetD3DCommandList(context(), command_list, command_allocator));
}

inline void SetNumD3DCommandLists(const Context& context, unsigned int num_command_lists)
{
    RML_CHECK_STATUS(rmlSetNumD3DCommandLists(context(), num_command_lists));
}

} // namespace rml

#undef RML_CHECK_STATUS
