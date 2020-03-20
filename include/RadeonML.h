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
 * @brief Machine learning inference API
 */

#include <stddef.h>
#include <stdint.h>

#if defined(_WIN32)
#    ifdef RML_BUILD
#        define RML_API_ENTRY __declspec(dllexport)
#    else
#        define RML_API_ENTRY __declspec(dllimport)
#    endif
#elif defined(__GNUC__)
#    ifdef RML_BUILD
#        define RML_API_ENTRY __attribute__((visibility("default")))
#    else
#        define RML_API_ENTRY
#    endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Maximal supported tensor rank.
 */
#define RML_TENSOR_MAX_RANK ((size_t) 5)

/**
 * @brief A context handle.
 */
typedef struct _rml_context* rml_context;

/**
 * @brief A model handle.
 */
typedef struct _rml_model* rml_model;

/**
 * @brief A tensor handle.
 */
typedef struct _rml_tensor* rml_tensor;

/**
 * @brief An Unicode character.
 * UTF-8 encoding is used for Linux and OSX and UTF-16 is used for Windows.
 */
#if defined(_WIN32)
typedef wchar_t rml_char;
#else
typedef char rml_char;
#endif

/**
 * @brief A boolean value.
 */
typedef enum _rml_bool
{
    RML_FALSE = 0, /**< False value. */
    RML_TRUE = 1,  /**< True value. */

} rml_bool;

/**
 * Operation status.
 */
typedef enum _rml_status
{
    RML_OK = 0,                       /**< Operation is successful. */
    RML_ERROR_BAD_DTYPE = -100,       /**< The data type of an operation is incorrect. */
    RML_ERROR_BAD_LAYOUT = -110,      /**< An inconsistent operation input layout. */
    RML_ERROR_BAD_MODEL_FILE = -120,  /**< A model file has errors. */
    RML_ERROR_BAD_NAME = -130,        /**< A node name is incorrect. */
    RML_ERROR_BAD_PARAMETER = -140,   /**< A parameter is incorrect. */
    RML_ERROR_BAD_SHAPE = -150,       /**< An inconsistent tensor shape. */
    RML_ERROR_FILE_NOT_FOUND = -160,  /**< A model file does not exist. */
    RML_ERROR_INTERNAL = -170,        /**< An internal library error. */
    RML_ERROR_MODEL_NOT_READY = -180, /**< A model is not ready for an operation. */
    RML_ERROR_NOT_IMPLEMENTED = -190, /**< Functionality is not implemented yet. */

} rml_status;

/**
 * @brief Data type.
 */
typedef enum _rml_dtype
{
    RML_DTYPE_UNSPECIFIED = 0, /**< Unspecified data type. */
    RML_DTYPE_FLOAT16 = 130,   /**< Full precision float type. */
    RML_DTYPE_FLOAT32 = 135,   /**< Half precision float type. */
    RML_DTYPE_INT32 = 145,     /**< Signed 32-bit integer type. */

} rml_dtype;

/**
 * @brief Physical memory layout of the tensor data.
 */
typedef enum _rml_data_layout
{
    RML_LAYOUT_UNSPECIFIED = 0, /**< Unspecified layout. */

    RML_LAYOUT_SCALAR = 200, /**< Tensor layout for a scalar value. */

    RML_LAYOUT_C = 210, /**< Tensor layout for a one dimensional tensor. */

    RML_LAYOUT_HW = 220, /**< Tensor layout with the folowing dimensions: height, width */
    RML_LAYOUT_NC = 221, /**< Tensor layout for a two dimensional tensor with data stored in
                          * the row-major order, where C - number of elements in a column,
                          * N - number of elements in a row. */
    RML_LAYOUT_CN = 222, /**< Tensor layout for a two dimensional tensor with data stored in
                          * the column-major order, where C - number of elements in a column,
                          * N - number of elements in a row. */

    RML_LAYOUT_CHW = 230, /**< Tensor layout for a single image in planar format
                           * with the following dimensions: number of channels, height, width. */
    RML_LAYOUT_HWC = 231, /**< Tensor layout for a single image in interleaved format
                           * with the following dimensions: height, width, number of channels. */

    RML_LAYOUT_NCHW = 240, /**< Tensor layout with the following dimensions: number of images (batch
                            * size) , number of channels, height and width. */
    RML_LAYOUT_NHWC = 241, /**< Tensor layout with the following dimensions: number of images (batch
                            * size), height, width, number of channels. */
    RML_LAYOUT_OIHW = 242, /**< Tensor layout with the following dimensions: number of output
                            * channels, number of input channels, height, width. */
    RML_LAYOUT_HWIO = 243, /**< Tensor layout with the following dimensions: height, width, number
                            * of input channels, number of output channels. */

    RML_LAYOUT_NCDHW = 250, /**< Tensor layout with the following dimensions: number of images
                             * (batch size), number of channels, depth, height, width. */
    RML_LAYOUT_NDHWC = 251, /**< Tensor layout with the following dimensions:
                             * number of images (batch size), depth, height, width, number of
                             * channels. */
} rml_layout;

/**
 * @brief Tensor access mode.
 */
typedef enum _rml_access_mode
{
    RML_ACCESS_MODE_UNSPECIFIED = 0,  /**< Unspecified access mode. */
    RML_ACCESS_MODE_READ_ONLY = 300,  /**< Allow reading from a tensor. */
    RML_ACCESS_MODE_READ_WRITE = 310, /**< Allow reading from and writing to a tensor. */
    RML_ACCESS_MODE_WRITE_ONLY = 320  /**< Allow writing from a tensor. */

} rml_access_mode;

/**
 * @brief Memory information.
 */
typedef struct _rml_memory_info
{
    size_t gpu_total;

} rml_memory_info;

/**
 * @brief N-dimensional tensor description.
 */
typedef struct _rml_tensor_info
{
    rml_dtype dtype;
    rml_layout layout;
    uint32_t shape[RML_TENSOR_MAX_RANK];

} rml_tensor_info;

/**
 * @brief Unspecified dimension value (a placeholder value)
 */
#define RML_DIM_UNSPECIFIED 0

/**
 * Creates a context.
 *
 * @param[out] context A pointer to a resulting context handle.
 *
 * @return A valid model handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if the @p context is NULL,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The context should be released with rmlReleaseContext().
 */
RML_API_ENTRY rml_status rmlCreateDefaultContext(rml_context* context);

/**
 * Releases a context created with rmlCreateDefaultContext(), invalidates the handle.
 *
 * @param[in] context A valid context handle.
 */
RML_API_ENTRY void rmlReleaseContext(rml_context context);

/**
 * Creates an N-dimentional tensor with a given description.
 *
 * @param[in]  context A valid context handle.
 * @param[in]  info    Tensor description with all dimensions specified.
 * @param[in]  mode    Tensor data access mode.
 * @param[out] tensor  A pointer to a resulting tensor handle.
 *
 * @return A valid tensor handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p context or @p info is NULL or @p mode has unknown value,
 * - #RML_ERROR_BAD_DTYPE if @p info has unknown @p dtype value,
 * - #RML_ERROR_BAD_LAYOUT if @p info has unknown @p layout value,
 * - #RML_ERROR_BAD_SHAPE if @p info has inconsistent @p shape values,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The context should be released with rmlReleaseTensor().
 */
RML_API_ENTRY rml_status rmlCreateTensor(rml_context context,
                                         const rml_tensor_info* info,
                                         rml_access_mode mode,
                                         rml_tensor* tensor);

/**
 * Returns a tensor description.
 *
 * @param[in]  tensor A valid tensor handle.
 * @param[out] info   A pointer to a resulting info structure.
 *
 * @return A valid tensor handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p tensor is invalid or @p info is NULL.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlGetTensorInfo(rml_tensor tensor, rml_tensor_info* info);

/**
 * Map the tensor data into the host address and returns a pointer to the mapped region.
 *
 * @param[in]  tensor A valid tensor handle.
 * @param[out] data   A pointer to a resulting data pointer.
 * @param[out] size   A pointer pointer to a resulting size. If not NULL, the referenced
 *                    size value is set to image size, in bytes.
 *
 * @return A data pointer, size (if @p size is not NULL) in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p tensor is invalid or @p data is NULL.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The mapped data must be unmapped with rmlUnmapTensor().
 */
RML_API_ENTRY rml_status rmlMapTensor(rml_tensor tensor, void** data, size_t* size);

/**
 * Unmaps a previously mapped tensor data.
 *
 * @param[in] tensor A valid tensor handle.
 * @param[in] data   A pointer to the previously mapped data.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p data is invalid.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlUnmapTensor(rml_tensor tensor, void* data);

/**
 * Releases an tensor created with rmlCreateTensor(), invalidates the handle.
 *
 * @param[in] tensor A valid tensor handle.
 */
RML_API_ENTRY void rmlReleaseTensor(rml_tensor tensor);

/**
 * Loads model data from a file.
 *
 * @param[in]  context A valid context handle.
 * @param[in]  path    Path to a model in the TF or ONNX formats, in UTF-8 (Linux, OSX)
 *                     or UTF-16 (Windows) encoding.
 * @param[out] model   A pointer to a resulting model handle.
 *
 * @return A valid model handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p context is invalid or @p path is NULL or @p model is NULL.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The model should be released with rmlReleaseModel().
 */
RML_API_ENTRY rml_status rmlLoadModel(rml_context context, const rml_char* path, rml_model* model);

/**
 * Returns model input node names.
 *
 * Memory for result name strings belongs to the model, so it must NOT be freed by a client.
 *
 * @param[in]  model      A valid model handle.
 * @param[in]  num_inputs The number of input (placeholder) nodes.
 * @param[out] names      A string array for resulting input node names, in ASCII encoding.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid or @p num_inputs is invalid or @p names is
 * NULL.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlGetModelInputNames(rml_model model,
                                               size_t num_inputs,
                                               const char* names[]);

/**
 * Sets up model output node names.
 *
 * If this function is not called, an output node is the last node in a model.
 *
 * @param[in]  model       A valid model handle.
 * @param[in]  num_outputs The number of output nodes.
 * @param[out] names       A string array with output node names, in ASCII encoding.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid or @p names is NULL,
 * - #RML_ERROR_BAD_NAME if @p name is not found in the model.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlSetModelOutputNames(rml_model model,
                                                size_t num_outputs,
                                                const char* const names[]);

/**
 * Returns input tensor information by a node name.
 * The @p name my be NULL if there is a single input node.
 *
 * @param[in]  model A valid model handle.
 * @param[in]  name  An input node name, in ASCII encoding.
 * @param[out] info  A pointer to a resulting input info structure.
 *                   If rmlSetModelInputInfo() was not previously called,
 *                   some dimensions may be unspecified.
 *
 * @return Input tensor information in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid or @p name is NULL,
 * - #RML_ERROR_BAD_NAME if on operation with @p name is not found in the model.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlGetModelInputInfo(rml_model model,
                                              const char* name,
                                              rml_tensor_info* info);

/**
 * Sets input tensor information for a node name.
 *
 * This call is optional if all model input dimensions are initially specified.
 * The @p name my be NULL if there is a single input node.
 *
 * @param[in] model A valid model handle.
 * @param[in] name  An input node name, in ASCII encoding.
 * @param[in] info  A pointer to a resulting input info structure.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid or @p name is NULL or info is NULL,
 * - #RML_ERROR_BAD_NAME if a name from @p name is not found in the model.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlSetModelInputInfo(rml_model model,
                                              const char* name,
                                              const rml_tensor_info* info);

/**
 * Prepares a model for inference.
 *
 * All unspecified model input dimensions must be set with rmlSetModelInputInfo()
 * before this function is called.
 *
 * @param[in] model A valid model handle.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid,
 * - #RML_ERROR_MODEL_NOT_READY if some inputs have unspecified dimensions,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlPrepareModel(rml_model model);

/**
 * Returns output tensor information.
 *
 * The @p name my be NULL if there is a single output node.
 *
 * @param[in]  model A valid model handle.
 * @param[in]  name  A nullable output node name, in ASCII encoding.
 * @param[out] info  A pointer to a resulting output info structure.
 *                   If rmlSetModelInputInfoTensor() was not previously called,
 *                   some dimensions may be unspecified.
 *
 * @return Output tensor information in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid or @p name is NULL,
 * - #RML_ERROR_MODEL_NOT_READY if rmlPrepareModel() was not called on the model,
 * - #RML_ERROR_BAD_NAME if on operation with @p name is not found in the model.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlGetModelOutputInfo(rml_model model,
                                               const char* name,
                                               rml_tensor_info* info);

/**
 * Sets up an input tensor for a node with a specified name.
 *
 * The model must be prepared with rmlPrepareModel() before this call.
 * The @p name my be NULL if there is a single input (placeholder) node.
 *
 * @param[in] model A valid model handle.
 * @param[in] name  A nullable input node name, in ASCII encoding.
 * @param[in] input A valid input tensor handle.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid or @p name is NULL or @p input is invalid,
 * - #RML_ERROR_MODEL_NOT_READY if rmlPrepareModel() was not called on the model,
 * - #RML_ERROR_BAD_NAME if on operation with @p name is not found in the model.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlSetModelInput(rml_model model, const char* name, rml_tensor input);

/**
 * Sets up an input tensor for a node with a specified name.
 *
 * The model must be prepared with rmlPrepareModel() before this call.
 * The @p name my be NULL if there is a single output node.
 *
 * @param[in] model  A valid model handle.
 * @param[in] name   An output node name nullable.
 * @param[in] output A valid output tensor handle.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid or @p name is NULL or @p output is invalid,
 * - #RML_ERROR_MODEL_NOT_READY if rmlPrepareModel() was not called on the model,
 * - #RML_ERROR_BAD_NAME if on operation with @p name is not found in the model.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlSetModelOutput(rml_model model, const char* name, rml_tensor output);

/**
 * Runs inference.
 *
 * All model inputs must be set with rmlSetModelInput() and all model ouputs
 * must be set with rmlSetModelOutput() before this function is called.
 *
 * @param[in] model A valid model handle.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid,
 * - #RML_ERROR_MODEL_NOT_READY if rmlPrepareModel() was not called on the model,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlInfer(rml_model model);

/**
 * Releases a model loaded with rmlLoadModel() or created with rmlCreateModel(), invalidates the
 * handle.
 *
 * @param[in] model A valid model handle.
 */
RML_API_ENTRY void rmlReleaseModel(rml_model model);

/**
 * Returns a null-terminated string containing the last operation error message.
 * May be called after some operation returns NULL or a status different fror #RML_OK.
 * The error message is owned by the library and must NOT be freed by a client.
 * The message is stored in a thread local storage, so this function
 * should be called from the thread where the failure occured.
 *
 * @param[out] size Optional, the size of the error message (excluding the null-terminator).
 *
 * @return A pointer to the formatted message, in ASCII-encoding.
 */
RML_API_ENTRY const char* rmlGetLastError(size_t* size);

/**
 * Set whether logging is enabled.
 *
 * @param[in] enabled Whether logging is enabled. Logging is enabled by default.
 */
RML_API_ENTRY void rmlSetLoggingEnabled(rml_bool enabled);

/**
 * Returns memory usage information.
 *
 * The model must be prepared for inference with rmlPrepareModel() before this call.
 *
 * @param[in]  model A valid model handle.
 * @param[out] info  A pointer to a resulting #rml_memory_info structure.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid or @p info is NULL,
 * - #RML_ERROR_MODEL_NOT_READY if rmlPrepareModel() was not called on the model.
 */
RML_API_ENTRY rml_status rmlGetModelMemoryInfo(rml_model model, rml_memory_info* info);

/**
 * Returns the number of input nodes (placeholders) in the model.
 *
 * @param[in]  model  A valid model handle.
 * @param[out] num_inputs A pointer to a resulting value.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid or @p num_inputs is NULL.
 */
RML_API_ENTRY rml_status rmlGetModelNumInputs(rml_model model, size_t* num_inputs);

#ifdef __cplusplus
} // extern "C"
#endif
