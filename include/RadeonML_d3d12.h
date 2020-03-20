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

#include "RadeonML.h"

#include <d3d12.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a context from an Direct3D 12 device and queue.
 *
 * @param[in]  command_queue A Direct3D abstraction is used to submit command lists for execution.
 * @param[out] context       A pointer to a resulting context handle.
 *
 * @return A valid context handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p command_queue or @context is NULL,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The context should be released with rmlReleaseContext().
 */
RML_API_ENTRY rml_status rmlCreateContextFromD3DQueue(ID3D12CommandQueue* command_queue,
                                                      rml_context* context);

/**
 * Creates a tensor from an Direct3D 12 resource.
 *
 * @param[in]  context  A valid context handle.
 * @param[in]  resource A Direct3D resource with state D3D12_RESOURCE_STATE_COMMON.
 * @param[in]  info     A tensor description with all dimensions specified.
 * @param[out] context  A pointer to a resulting tensor handle.
 *
 * @return A valid tensor handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p context is invalid, @p resource,
 *      @p info or @ p tensor is NULL or @p mode is invalid.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The context should be released with rmlReleaseTensor().
 */
RML_API_ENTRY rml_status rmlCreateTensorFromD3DResource(rml_context context,
                                                        ID3D12Resource* resource,
                                                        const rml_tensor_info* info,
                                                        rml_tensor* tensor);

/**
 * Get underlying Direct3D 12 resource from a tensor.
 *
 * @param[in]  tensor   A valid tensor handle.
 * @param[out] resource A pointer to a resulting resourcve pointer.
 *
 * @return A non-NULL resource pointer in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p tensor is invalid or @p resource is NULL.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The resulting resource is reference counted by the @p tensor.
 */
RML_API_ENTRY rml_status rmlGetD3DResourceFromTensor(rml_tensor tensor, ID3D12Resource** resource);

/**
 * Set command list for interop.
 *
 * @param[in] context           A valid context handle.
 * @param[in] command_list      A valid D3D12 command list or NULL.
 *                              A NULL command list switches to use internal command buffer.
 *                              A non-NULL command list are only populated with DirectML commands in
 *                              append mode without resetting or running. Client provided non-NULL
 *                              command list MUST restore its own descriptor heaps and root
 *                              signature right after rmlInfer() call, because RadeonML sets own
 *                              descriptor heaps and own compute root signature during this call.
 * @param[in] command_allocator A memory backing class associated with the specified command list
 *                              or NULL.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p context is invalid or both @p command_list and
 *      @p command_allocator are NULL.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlSetD3DCommandList(rml_context context,
                                              ID3D12GraphicsCommandList* command_list,
                                              ID3D12CommandAllocator* command_allocator);

/**
 * Set number of internal command lists to use for execution if interop command list is not specified.
 *
 * @param[in] context           A valid context handle.
 * @param[in] num_command_lists A number of internal command lists that will be created.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p context is invalid or both @p num_command_lists is 0.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlSetNumD3DCommandLists(rml_context context,
                                                  unsigned int num_command_lists);

/**
* Copies image content.
*
* @param[in] src A valid source tensor handle.
* @param[in] dst A valid destination tensor handle.
*
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p src or @p dst is invalid.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlCopyTensor(rml_tensor src, rml_tensor dst);

#ifdef __cplusplus
} // extern "C"
#endif
