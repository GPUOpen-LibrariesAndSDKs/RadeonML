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
 * @brief A graph handle.
 */
typedef struct _rml_graph* rml_graph;

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
 * UTF-8 encoding is used for Linux and MacOS and UTF-16 is used for Windows.
 */
#if defined(_WIN32)
typedef wchar_t rml_char;
#else
typedef char rml_char;
#endif

/**
 * @brief A storage for multiple strings.
 */
typedef struct _rml_strings
{
    size_t num_items;
    const char* const* items;

} rml_strings;

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
    RML_OK = 0,                        /**< Operation is successful. */
    RML_ERROR_BAD_MODEL = -100,        /**< A model file has errors. */
    RML_ERROR_BAD_PARAMETER = -110,    /**< A parameter is incorrect. */
    RML_ERROR_DEVICE_NOT_FOUND = -120, /**< A device was not found. */
    RML_ERROR_FILE_NOT_FOUND = -130,   /**< A model file does not exist. */
    RML_ERROR_INTERNAL = -140,         /**< An internal library error. */
    RML_ERROR_MODEL_NOT_READY = -150,  /**< A model is not ready for an operation. */
    RML_ERROR_NOT_IMPLEMENTED = -160,  /**< Functionality is not implemented yet. */
    RML_ERROR_OUT_OF_MEMORY = -170,    /**< Memory allocation is failed. */
    RML_ERROR_UNSUPPORTED_DATA = -180, /**< An unsupported scenario. */

} rml_status;

/**
 * @brief Data type.
 */
typedef enum _rml_dtype
{
    RML_DTYPE_UNSPECIFIED = 0, /**< Unspecified data type. */
    RML_DTYPE_FLOAT32 = 100,   /**< Full precision float type. */
    RML_DTYPE_FLOAT16 = 101,   /**< Half precision float type. */
    RML_DTYPE_UINT8 = 110,     /**< Unsigned 8-bit integer type. Unsupported. */
    RML_DTYPE_INT32 = 120,     /**< Signed 32-bit integer type. */

} rml_dtype;

/**
 * @brief Physical memory layout of the tensor data.
 */
typedef enum _rml_layout
{
    RML_LAYOUT_UNSPECIFIED = 0, /**< Unspecified layout. */

    RML_LAYOUT_SCALAR = 200, /**< Tensor layout for a scalar value. */

    RML_LAYOUT_C = 210, /**< Tensor layout for a one dimensional tensor. */

    RML_LAYOUT_HW = 220, /**< Tensor layout with the folowing dimensions: height, width */
    RML_LAYOUT_NC = 221, /**< Tensor layout for a two dimensional tensor with data stored in
                          * the row-major order, where C - number of elements in a column,
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

} rml_layout;

/**
 * @brief Tensor access mode.
 *
 * Mode indicates abilities to access tensor contents on a CPU.
 */
typedef enum _rml_access_mode
{
    RML_ACCESS_MODE_UNSPECIFIED = 0,   /**< Unspecified access mode. */
    RML_ACCESS_MODE_READ_ONLY = 300,   /**< Allow reading from a tensor. */
    RML_ACCESS_MODE_READ_WRITE = 310,  /**< Allow reading from and writing to a tensor. */
    RML_ACCESS_MODE_WRITE_ONLY = 320,  /**< Allow writing from a tensor. */
    RML_ACCESS_MODE_DEVICE_ONLY = 330, /**< No reading from and writing to a tensor. */

} rml_access_mode;

/**
 * @brief Defines graph format required for loading model from buffer.
 */
typedef enum _rml_graph_format
{
    RML_GRAPH_FORMAT_UNSPECIFIED = 0, /**< Unspecified graph format. */
    RML_GRAPH_FORMAT_TF = 400,        /**< Tensorflow 1.x binary graph format. */
    RML_GRAPH_FORMAT_TF_TXT = 410,    /**< Tensorflow text graph format. */
    RML_GRAPH_FORMAT_ONNX = 420,      /**< ONNX binary graph format. */
    RML_GRAPH_FORMAT_ONNX_TXT = 430,  /**< ONNX text graph format. */

} rml_graph_format;

/**
 * @brief Memory information.
 */
typedef struct _rml_memory_info
{
    /**
     * Total amount of allocated GPU memory.
     */
    size_t gpu_total;

} rml_memory_info;

/**
 * @brief N-dimensional tensor description.
 */
typedef struct _rml_tensor_info
{
    /**
     * Tensor data type.
     */
    rml_dtype dtype;

    /**
     * Physical tensor data layout.
     */
    rml_layout layout;

    /**
     * Tensor shape where axes order must correspond to the data layout.
     */
    uint32_t shape[RML_TENSOR_MAX_RANK];

} rml_tensor_info;

/**
 * @brief Unspecified dimension value (a placeholder value)
 */
#define RML_DIM_UNSPECIFIED 0u

/**
 * @brief Device index for automatic device selection
 */
#define RML_DEVICE_IDX_UNSPECIFIED 0u

/**
 * Context creation parameters.
 */
typedef struct _rml_context_params
{
    /**
     * Device index, corresponding to the backend device query result.
     * Enumeration is started with 1. Use RML_DEVICE_IDX_UNSPECIFIED (0)
     * for auto device selection.
     */
    uint32_t device_idx;

} rml_context_params;

/**
 * Creates a context.
 *
 * @param[in]  params  Context creation parameters, optional, @see #rml_context_params.
 * @param[out] context A pointer to a resulting context handle.
 *
 * @return A valid context handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if the @p params->device_idx is incorrect or the @p context is NULL,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The context should be released with rmlReleaseContext().
 */
RML_API_ENTRY rml_status rmlCreateDefaultContext(const rml_context_params* params,
                                                 rml_context* context);

/**
 * Releases a context created with rmlCreateDefaultContext() or another context creating function,
 * invalidates the handle.
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
 * - #RML_ERROR_BAD_PARAMETER if @p context, @p info or @p mode is invalid or @p tensor is NULL,
 * - #RML_ERROR_OUT_OF_MEMORY if memory allocation is failed,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The tensor should be released with rmlReleaseTensor().
 */
RML_API_ENTRY rml_status rmlCreateTensor(rml_context context,
                                         const rml_tensor_info* info,
                                         rml_access_mode mode,
                                         rml_tensor* tensor);

/**
 * Returns a tensor information.
 *
 * @param[in]  tensor A valid tensor handle.
 * @param[out] info   A pointer to a resulting info structure.
 *
 * @return Tensor information in case of success and status:
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
 * Load graph from a ptotobuf file.
 *
 * @param[in]  path  Path to a graph in the TF or ONNX formats.
 * @param[out] graph The pointer to a resulting graph handle.
 *
 * @return A valid graph handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p path or @p graph is NULL,
 * - #RML_ERROR_FILE_NOT_FOUND if the model file is not found,
 * - #RML_ERROR_BAD_MODEL if the model contains an error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The graph should be released with rmlReleaseGraph().
 */
RML_API_ENTRY rml_status rmlLoadGraphFromFile(const rml_char* path, rml_graph* graph);

/**
 * Loads graph from a protobuf buffer.
 *
 * @param[in]  size   The buffer size.
 * @param[in]  buffer The buffer pointer.
 * @param[in]  format The buffer format.
 * @param[out] graph  The pointer to a resulting graph handle.
 *
 * @return A valid graph handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p buffer or @p graph is NULL.
 * - #RML_ERROR_BAD_MODEL if the model contains an error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 * The graph should be released with rmlReleaseGraph().
 */
RML_API_ENTRY rml_status rmlLoadGraphFromBuffer(size_t size,
                                                const void* buffer,
                                                rml_graph_format format,
                                                rml_graph* graph);

/**
 * Creates a model from a supplied graph.
 *
 * @param[in]  context A valid context handle.
 * @param[in]  graph   A valid graph handle.
 * @param[out] model   A pointer to a resulting model handle.
 *
 * @return A model handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p context or @p graph is invalid or @p model is NULL.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlCreateModelFromGraph(rml_context context,
                                                 rml_graph graph,
                                                 rml_model* model);

/**
 * Sets up model output node names.
 *
 * If this function is not called, all leaf graph nodes are considered to be output.
 *
 * @param[in]  model A valid model handle.
 * @param[out] names A pointer to a structure with output names.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model or @p names is invalid.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlSetModelOutputNames(rml_model model, const rml_strings* names);

/**
 * Returns input tensor information by a node name.
 *
 * The @p name my be NULL if there is a single input (placeholder) node.
 *
 * @param[in]  model A valid model handle.
 * @param[in]  name  An optional input node name, in ASCII encoding.
 * @param[out] info  A pointer to a resulting input info structure.
 *                   If rmlSetModelInputInfo() was not previously called,
 *                   some dimensions may be unspecified.
 *
 * @return Input tensor information in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model or @p name is invalid or @p info is NULL.
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
 * The @p name my be NULL if there is a single input (placeholder) node.
 *
 * @param[in] model A valid model handle.
 * @param[in] name  An input node name, in ASCII encoding.
 * @param[in] info  A pointer to a input info structure.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model or @p name is invalid or @p info is NULL.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlSetModelInputInfo(rml_model model,
                                              const char* name,
                                              const rml_tensor_info* info);

/**
 * Returns output tensor information.
 *
 * All input dimensions must be specified before this call.
 * The @p name my be NULL if there is a single output node.
 *
 * @param[in]  model A valid model handle.
 * @param[in]  name  A optional output node name, in ASCII encoding.
 * @param[out] info  A pointer to a resulting output info structure.
 *
 * @return Output tensor information in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model or @p name is invalid or @p info is NULL,
 * - #RML_ERROR_MODEL_NOT_READY if some inputs have unspecified dimensions.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlGetModelOutputInfo(rml_model model,
                                               const char* name,
                                               rml_tensor_info* info);

/**
 * Returns memory usage information.
 *
 * All input dimensions must be specified before this call.
 *
 * @param[in]  model A valid model handle.
 * @param[out] info  A pointer to a resulting #rml_memory_info structure.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid or @p info is NULL,
 * - #RML_ERROR_MODEL_NOT_READY if some inputs have unspecified dimensions.
 */
RML_API_ENTRY rml_status rmlGetModelMemoryInfo(rml_model model, rml_memory_info* info);

/**
 * Sets up an input tensor for a node with a specified name.
 *
 * All input dimensions must be specified before this call.
 * The @p name my be NULL if there is a single input (placeholder) node.
 *
 * @param[in] model A valid model handle.
 * @param[in] name  A optional input node name, in ASCII encoding.
 * @param[in] input A valid input tensor handle.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model, @p name or @p input is invalid,
 * - #RML_ERROR_MODEL_NOT_READY if some inputs have unspecified dimensions.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlSetModelInput(rml_model model, const char* name, rml_tensor input);

/**
 * Sets up an input tensor for a node with a specified name.
 *
 * All input dimensions must be specified before this call.
 * The @p name my be NULL if there is a single output node.
 *
 * @param[in] model  A valid model handle.
 * @param[in] name   An optional output node name, in ASCII encoding.
 * @param[in] output A valid output tensor handle.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model, @p name or @p output is invalid,
 * - #RML_ERROR_MODEL_NOT_READY if some inputs have unspecified dimensions.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlSetModelOutput(rml_model model, const char* name, rml_tensor output);

/**
 * Prepares a model for inference.
 *
 * All model inputs must be set with rmlSetModelInput() and all model outputs
 * must be set with rmlSetModelOutput() before this function is called.
 *
 * @param[in] model A valid model handle.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid,
 * - #RML_ERROR_MODEL_NOT_READY if any input or output tensor is not specified,
 * - #RML_ERROR_OUT_OF_MEMORY if memory allocation is failed,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlPrepareModel(rml_model model);

/**
 * Runs inference.
 *
 * All model inputs must be set with rmlSetModelInput() and all model outputs
 * must be set with rmlSetModelOutput() before this function is called.
 *
 * @param[in] model A valid model handle.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid,
 * - #RML_ERROR_MODEL_NOT_READY if any input or output tensor is not specified,
 * - #RML_ERROR_OUT_OF_MEMORY if memory allocation is failed,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlInfer(rml_model model);

/**
 * Resets internal model states to their initial values.
 *
 * All model inputs must be set with rmlSetModelInput() and all model outputs
 * must be set with rmlSetModelOutput() before this function is called.
 *
 * @param[in] model A valid model handle.
 *
 * @return Status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p model is invalid,
 * - #RML_ERROR_MODEL_NOT_READY if any input or output tensor is not specified,
 * - #RML_ERROR_INTERNAL in case of an internal error.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlResetModelStates(rml_model model);

/**
 * Releases a model created with rmlCreateModelFromGraph(), invalidates the handle.
 *
 * @param[in] model A valid model handle.
 */
RML_API_ENTRY void rmlReleaseModel(rml_model model);

/**
 * Returns a null-terminated string containing the last operation error message.
 * May be called after some operation returns a status other than #RML_OK.
 * The error message is owned by the library and must NOT be freed by a client.
 * The message is stored in a thread local storage, so this function
 * should be called from the thread where the failure occured.
 *
 * @return A pointer to the formatted message, in ASCII-encoding.
 */
RML_API_ENTRY const char* rmlGetLastError();

/**
 * Set whether logging is enabled.
 *
 * @param[in] enabled Whether logging is enabled. Logging is enabled by default.
 */
RML_API_ENTRY void rmlSetLoggingEnabled(rml_bool enabled);

#ifdef __cplusplus
} // extern "C"
#endif
