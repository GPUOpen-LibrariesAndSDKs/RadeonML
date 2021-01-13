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
 * @brief Graph manipulation API
 */

#include "rml/RadeonML.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A graph operation.
 */
typedef struct _rml_op* rml_op;

/**
 * @brief Operations supported by RadeonML.
 */
typedef enum _rml_op_type
{
    RML_OP_UNSPECIFIED = 0,

    /**
     * @brief Calculates absolute of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#abs, #rml_op_unary_params
     */
    RML_OP_ABS = 1010,

    /**
     * @brief Calculates arccosine of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#acos, #rml_op_unary_params
     */
    RML_OP_ACOS = 1020,

    /**
     * @brief Sums input tensors up, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#add, #rml_op_binary_params
     */
    RML_OP_ADD = 1030,

    /**
     * @brief Calculates arcsine of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#asin, #rml_op_unary_params
     */
    RML_OP_ASIN = 1050,

    /**
     * @brief Calculates arctangent of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#atan, #rml_op_unary_params
     */
    RML_OP_ATAN = 1070,

    /**
     * @brief Calculates average between input tensors, element-wise
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#avg, #rml_op_binary_params
     */
    RML_OP_AVG = 1080,

    /**
     * @brief Performs batch normalization of an input tensor.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#batch_norm, #rml_op_batch_norm_params
     */
    RML_OP_BATCH_NORM = 1090,

    /**
     * @brief Adds bias to an input tensor.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#bias_add, #rml_op_bias_add_params
     */
    RML_OP_BIAS_ADD = 1100,

    /**
     * @brief Casts elements of an input tensor to a specified type.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#cast, #rml_op_cast_params
     */
    RML_OP_CAST = 1110,

    /**
     * @brief Ceils elements of an input tensor.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#ceil, #rml_op_unary_params
     */
    RML_OP_CEIL = 1120,

    /**
     * @brief Applies CeLU activation to an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#celu, #rml_op_celu_params
     */
    RML_OP_CELU = 1130,

    /**
     * @brief Limits input tensor values within a specified range.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#clip, #rml_op_clip_params
     */
    RML_OP_CLIP = 1140,

    /**
     * @brief Concatenates input tensors into a single tensor along a specified axis.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#concat, #rml_op_concat_params
     */
    RML_OP_CONCAT = 1150,

    /**
     * @brief Stores specified tensor data.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#constant, #rml_op_const_params
     */
    RML_OP_CONST = 1160,

    /**
     * @brief Performs 2D convolution of an input tensor.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#conv_2d, #rml_op_conv_2d_params
     */
    RML_OP_CONV_2D = 1190,

    /**
     * @brief Performs depthwise 2D convolution of an input tensor.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#conv_2d_depthwise, #rml_op_conv_2d_params
     */
    RML_OP_CONV_2D_DEPTHWISE = 1200,

    /**
     * @brief Performs transposed 2D convolution of an input tensor.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#conv_2d_transpose, #rml_op_conv_2d_transpose_params
     */
    RML_OP_CONV_2D_TRANSPOSE = 1210,

    /**
     * @brief Calculates cosine of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#cos, #rml_op_unary_params
     */
    RML_OP_COS = 1240,

    /**
     * @brief Rearranges input tensor data from channels into blocks of spatial data
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#depth_to_space, #rml_op_depth_to_space_params
     */
    RML_OP_DEPTH_TO_SPACE = 1250,

    /**
     * @brief Divides input tensors, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#div, #rml_op_binary_params
     */
    RML_OP_DIV = 1270,

    /**
     * @brief Applies ELU activation to an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#elu, #rml_op_elu_params
     */
    RML_OP_ELU = 1300,

    /**
     * @brief Calculates exponent of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#exp, #rml_op_unary_params
     */
    RML_OP_EXP = 1310,

    /**
     * @brief Flattens an input tensor into a 2D matrix.
     *
     * Backend support:
     * - DirectML: Partial. Cannot handle some cases.
     * - MIOpen: Partial. Cannot handle some cases.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#flatten, #rml_op_unary_params
     */
    RML_OP_FLATTEN = 1330,

    /**
     * @brief Floors elements of an input tensor.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#floor, #rml_op_unary_params
     */
    RML_OP_FLOOR = 1340,

    /**
     * @brief Calculates gemm of input tensors, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#gemm, #rml_op_gemm_params
     */
    RML_OP_GEMM = 1360,

    /**
     * @brief Identity operator.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#identity, #rml_op_unary_params
     */
    RML_OP_IDENTITY = 1380,

    /**
     * @brief Applies Leaky ReLU activation to an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#leaky_relu, #rml_op_leaky_relu_params
     */
    RML_OP_LEAKY_RELU = 1390,

    /**
     * @brief Performs local response normalization of an input tensor.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#local_response_norm, #rml_op_local_response_norm_params
     */
    RML_OP_LOCAL_RESPONSE_NORM = 1410,

    /**
     * @brief Calculates LogSoftMax of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#log_softmax, #rml_op_unary_params
     */
    RML_OP_LOG_SOFTMAX = 1420,

    /**
     * @brief Calculates natural logarithm of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#logn, #rml_op_unary_params
     */
    RML_OP_LOGN = 1430,

    /**
     * @brief Calculates maximum of input tensors, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#max, #rml_op_binary_params
     */
    RML_OP_MAX = 1440,

    /**
     * @brief Calculates minimum of input tensors, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#min, #rml_op_binary_params
     */
    RML_OP_MIN = 1450,

    /**
     * @brief Multiplies input tensors, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#mul, #rml_op_binary_params
     */
    RML_OP_MUL = 1460,

    /**
     * @brief Calculates neg of the input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#neg, #rml_op_unary_params
     */
    RML_OP_NEG = 1470,

    /**
     * @brief Describes a data reorganization operator that inflates the input tensor with zeroes
     * (or some other value) on the edges
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#pad, #rml_op_pad_params
     */
    RML_OP_PAD = 1490,

    /**
     * @brief Performs Parametric ReLU activation of an input tensor, elsement-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#parametric_relu, #rml_op_binary_params
     */
    RML_OP_PARAMETRIC_RELU = 1510,

    /**
     * @brief Special operation that holds information about input data.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#placeholder, #rml_op_placeholder_params
     */
    RML_OP_PLACEHOLDER = 1520,

    /**
     * @brief Performs 2D average pooling of an input tensor.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#pool_2d_avg, #rml_op_pool_2d_params
     */
    RML_OP_POOL_2D_AVG = 1560,

    /**
     * @brief Performs 2D global average pooling of an input tensor.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#pool_2d_global_avg, #rml_op_pool_2d_global_params
     */
    RML_OP_POOL_2D_GLOBAL_AVG = 1570,

    /**
     * @brief Performs 2D max pooling of an input tensor.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#pool_2d_max, #rml_op_pool_2d_params
     */
    RML_OP_POOL_2D_MAX = 1580,

    /**
     * @brief Returns a specified output tensor of an input operation.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#port, #rml_op_port_params
     */
    RML_OP_PORT = 1625,

    /**
     * @brief Calculates power of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Partial. Wrong results for negative base values.
     * - MIOpen: Yes.
     * - Apple MPS: Partial. Wrong results for negative base values.
     *
     * @see rml_op_desc#pow, #rml_op_pow_params
     */
    RML_OP_POW = 1630,

    /**
     * @brief Transforms a hig-precision input tensor into a low-precision output tensor.
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc #quantize_linear, #rml_op_quantize_linear_params
     */
    RML_OP_QUANTIZE_LINEAR = 1640,

    /**
     * @brief Calculates reciprocal of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#recip, #rml_op_unary_params
     */
    RML_OP_RECIP = 1650,

    /**
     * @brief Calculates sum of the input tensor elements along provided axes.
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reduce_add, #rml_op_reduce_params
     */
    RML_OP_REDUCE_ADD = 1660,

    /**
     * @brief Calculates sum square of input tensor elements along provided axes.
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reduce_add_square, #rml_op_reduce_params
     */
    RML_OP_REDUCE_ADD_SQUARE = 1670,

    /**
     * @brief Calculates maximum elements indices of input tensor elements along provided axis.
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reduce_argmax, #rml_op_reduce_params
     */
    RML_OP_REDUCE_ARGMAX = 1680,

    /**
     * @brief Calculates minimum elements indices of input tensor elements along provided axis.
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reduce_argmin, #rml_op_reduce_params
     */
    RML_OP_REDUCE_ARGMIN = 1690,

    /**
     * @brief Calculates average of input tensor elements along provided axes.
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reduce_avg, #rml_op_reduce_params
     */
    RML_OP_REDUCE_AVG = 1700,

    /**
     * @brief Calculates L1 norm of input tensor elements along provided axes
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reduce_l1, #rml_op_reduce_params
     */
    RML_OP_REDUCE_L1 = 1710,

    /**
     * @brief Calculates L2 norm of input tensor elements along provided axes
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reduce_l2, #rml_op_reduce_params
     */
    RML_OP_REDUCE_L2 = 1720,

    /**
     * @brief Calculates log sum of input tensor elements along provided axes.
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reduce_logn_add, #rml_op_reduce_params
     */
    RML_OP_REDUCE_LOGN_ADD = 1730,

    /**
     * @brief Calculates log sum exponent of input tensor element along provided axes.
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reduce_logn_add_exp, #rml_op_reduce_params
     */
    RML_OP_REDUCE_LOGN_ADD_EXP = 1740,

    /**
     * @brief Calculates maximum of input tensor elements along provided axes.
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reduce_max, #rml_op_reduce_params
     */
    RML_OP_REDUCE_MAX = 1750,

    /**
     * @brief Calculates minimum of input tensor elements along provided axes.
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reduce_min, #rml_op_reduce_params
     */
    RML_OP_REDUCE_MIN = 1760,

    /**
     * @brief Calculates product of input tensor elements along provided axes.
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reduce_mul, #rml_op_reduce_params
     */
    RML_OP_REDUCE_MUL = 1770,

    /**
     * @brief Applies ReLU activation to an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#relu, #rml_op_unary_params
     */
    RML_OP_RELU = 1780,

    /**
     * @brief Applies ReLU6 activation to an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#relu6, #rml_op_unary_params
     */
    RML_OP_RELU6 = 1790,

    /**
     * @brief Changes the shape of an input tensor without changing tensor data.
     *
     * Backend support:
     * - DirectML: Partial. Cannot handle some cases.
     * - MIOpen: Partial. Cannot handle some cases.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#reshape, #rml_op_reshape_params
     */
    RML_OP_RESHAPE = 1800,

    /**
     * @brief Resizes an input tensor along spatial dims using the nearest neighbor algorithm.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#resize_2d_nearest, #rml_op_resize_2d_params
     */
    RML_OP_RESIZE_2D_NEAREST = 1810,

    /**
     * @brief Resizes an input tensor along spatial dims using the bicubic algorithm.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#resize_2d_bicubic, #rml_op_resize_2d_params
     */
    RML_OP_RESIZE_2D_BICUBIC = 1820,

    /**
     * @brief Calculates reciprocal of square root of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#rsqrt, #rml_op_unary_params
     */
    RML_OP_RSQRT = 1870,

    /**
     * @brief Applies Scaled ELU activation to an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#selu, #rml_op_selu_params
     */
    RML_OP_SELU = 1880,

    /**
     * @brief Outputs the shape of an input tensor.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#shape, #rml_op_unary_params
     */
    RML_OP_SHAPE = 1890,

    /**
     * @brief Applies Sigmoid activation of the input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Partial. Invalid result in case of fp16.
     *
     * @see rml_op_desc#sigmoid, #rml_op_unary_params
     */
    RML_OP_SIGMOID = 1900,

    /**
     * @brief Calculates sine of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#sin, #rml_op_unary_params
     */
    RML_OP_SIN = 1910,

    /**
     * @brief Produces a strided slice of an input tensor along multiple axes.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#slice, #rml_op_slice_params
     */
    RML_OP_SLICE = 1920,

    /**
     * @brief Applies Softmax activation to an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#softmax, #rml_op_unary_params
     */
    RML_OP_SOFTMAX = 1930,

    /**
     * @brief Applies Softplus activation to an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#softplus, #rml_op_unary_params
     */
    RML_OP_SOFTPLUS = 1950,

    /**
     * @brief Applies Softsign activation to an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#softsign, #rml_op_unary_params
     */
    RML_OP_SOFTSIGN = 1960,

    /**
     * @brief Rearranges blocks of spatial tensor data into channels.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#space_to_depth, #rml_op_space_to_depth_params
     */
    RML_OP_SPACE_TO_DEPTH = 1970,

    /**
     * @brief Calculates square root of an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#sqrt, #rml_op_unary_params
     */
    RML_OP_SQRT = 1990,

    /**
     * @brief Removes single-dimensional entries from the shape of a tensor.
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#squeeze, #rml_op_squeeze_params
     */
    RML_OP_SQUEEZE = 2000,

    /**
     * @brief Stacks a list of tensors with rank R into single tensor with rank R+1.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#stack, #rml_op_stack_params
     */
    RML_OP_STACK = 2010,

    /**
     * @brief Subtracts input tensors, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#sub, #rml_op_binary_params
     */
    RML_OP_SUB = 2020,

    /**
     * @brief Calculate tangent of the input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#tan, #rml_op_unary_params
     */
    RML_OP_TAN = 2030,

    /**
     * @brief Applies hyperbolic tangent activation to an input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Partial. Invalid result in case of fp16.
     *
     * @see rml_op_desc#tanh, #rml_op_unary_params
     */
    RML_OP_TANH = 2040,

    /**
     * @brief Applies Thresholded ReLU activation of the input tensor, element-wise.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: Yes.
     * - Apple MPS: Yes.
     *
     * @see rml_op_desc#thresholded_relu, #rml_op_thresholded_relu_params
     */
    RML_OP_THRESHOLDED_RELU = 2050,

     /**
     * @brief Retrieve the top-K largest or smallest elements along a specified axis.
     *
     * Backend support:
     * - DirectML: Yes.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#top_k, #rml_op_top_k_params
     */
    RML_OP_TOP_K = 2060,

    /**
     * @brief Transposes an input tensor.
     *
     * Backend support:
     * - DirectML: Partial.
     * - MIOpen: Partial.
     * - Apple MPS: Partial.
     *
     * @see rml_op_desc#transpose, #rml_op_transpose_params
     */
    RML_OP_TRANSPOSE = 2070,

    /**
     * @brief Inserts dimensions of size 1 at specified axis indices.
     *
     * Backend support:
     * - DirectML: Yes. Some layout transitions are unsupported.
     * - MIOpen: No.
     * - Apple MPS: No.
     *
     * @see rml_op_desc#unsqueeze, #rml_op_unsqueeze_params
     */
    RML_OP_UNSQUEEZE = 2080,

} rml_op_type;

/**
 * @brief The padding calculation algorithm.
 *
 * If padding type is one of {RML_PADDING_SAME_LOWER, RML_PADDING_SAME_UPPER, RML_PADDING_SAME},
 * then paddings will be added to output_spatial_size[i] = ceil(input_spatial_size[i] /
 * strides[i]). In case of odd number add the extra padding at the end for
 * RML_PADDING_SAME_UPPER and at the beginning for RML_PADDING_SAME_LOWER.
 *
 * Padding type RML_PADDING_VALID actually means no padding and in this case
 * output_spatial_size[i] = ceil((input_spatial_size[i] - (kernel_size[i]-1) * dilations[i]) /
 * strides[i]).
 *
 * Padding type RML_PADDING_EXPLICIT means explicit start and end paddings are used.
 */
typedef enum _rml_padding_type
{
    RML_PADDING_UNSPECIFIED = 0,
    RML_PADDING_EXPLICIT = 605,
    RML_PADDING_SAME_LOWER = 610,
    RML_PADDING_SAME_UPPER = 615,
    RML_PADDING_SAME = RML_PADDING_SAME_UPPER,
    RML_PADDING_VALID = 620

} rml_padding_type;

/**
 * @brief Defines constants that specify a mode for the DirectML pad operator.
 *
 * Padding mode RML_PADDING_MODE_CONSTANT means padding with a constant.
 * Padding mode RML_PADDING_MODE_EDGE indicates edge mode for padding.
 * Padding mode RML_PADDING_MODE_REFLECTION indicates reflection mode for padding.
 *
 *
 */
typedef enum _rml_pad_mode
{
    RML_PAD_MODE_UNSPECIFIED = 0,
    RML_PAD_MODE_CONSTANT = 1,
    RML_PAD_MODE_EDGE = 2,
    RML_PAD_MODE_REFLECTION = 3

} rml_pad_mode;

/**
 * @brief Represents any of 2D height/width values
 */
typedef struct _rml_size_2d
{
    uint32_t h;
    uint32_t w;

} rml_size_2d;

/**
 * @brief Parameters for the #RML_OP_ELU operation
 *
 * ELU(x) = max(0, x) + min(0, alpha * (exp(x) - 1))
 */
typedef struct _rml_op_elu_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

    /**
     * The alpha value for the ELU formulation. Usually is 1.0
     */
    float alpha;

} rml_op_elu_params;

#define RML_OP_ELU_DEFAULT_ALPHA 1.f

/**
 * @brief Parameters for the #RML_OP_LEAKY_RELU operation
 *
 * LeakyReLU(x) = max(0, x) + alpha * min(0, x)
 */
typedef struct _rml_op_leaky_relu_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

    /**
     * The alpha value for the LeakyReLU formulation.
     * Controls the angle of the negative slope. Usually is 0.2f
     */
    float alpha;

} rml_op_leaky_relu_params;

#define RML_OP_LEAKY_RELU_DEFAULT_ALPHA 0.2f

/**
 * @brief Parameters for the #RML_OP_SELU operation
 *
 * SeLU(x) = gamma * (alpha * e^x - alpha)
 */
typedef struct _rml_op_selu_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

    /**
     * The alpha value for the SeLU formulation.
     */
    float alpha;

    /**
     * The gamma value for the SeLU formulation.
     */
    float gamma;

} rml_op_selu_params;

#define RML_OP_SELU_DEFAULT_ALPHA 1.6733f
#define RML_OP_SELU_DEFAULT_GAMMA 1.0507f

/**
 * @brief Parameters for the #RML_OP_THRESHOLDED_RELU operation
 *
 * ThReLU(x) = x, x > alpha
 * ThReLU(x) = 0, x <= alpha
 */
typedef struct _rml_op_thresholded_relu_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

    /**
     * The threshold value for the ThReLU formulation.
     */
    float alpha;

} rml_op_thresholded_relu_params;

#define RML_OP_THRESHOLDED_RELU_DEFAULT_ALPHA 0.f

/**
 * @brief Parameters for the #RML_OP_BATCH_NORM operation
 *
 * f(x) = scale * (x - mean) / sqrt(variance + epsilon) + bias.
 */
typedef struct _rml_op_batch_norm_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

    /**
     * Constant operation that outputs tensor containing the batch mean
     */
    rml_op mean;

    /**
     * Constant operation that outputs tensor containing the batch variance
     */
    rml_op variance;

    /**
     * Constant operation that outputs tensor containing the scale
     */
    rml_op scale;

    /**
     * Constant operation that outputs tensor containing the batch bias
     */
    rml_op bias;

    /**
     * Small value to use to avoid division by zero
     */
    float epsilon;

} rml_op_batch_norm_params;

#define RML_OP_BATCH_NORMALIZATION_DEFAULT_EPSILON 1e-5f

/**
 * @brief Parameters for the #RML_OP_CAST operation
 *
 * Performs the cast function f(x) = cast(x),
 * casting each element in the input to the data type of the output tensor
 */
typedef struct _rml_op_cast_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

    rml_dtype cast_to; /**< Output data type */

} rml_op_cast_params;

/**
 * @brief Parameters for the #RML_OP_CLIP operation
 *
 * f(x) = clip(x, min, max)
 */
typedef struct _rml_op_clip_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

    /**
     * The minimum value, below which the operator replaces the value with
     */
    float min;

    /**
     * The maximum value, above which the operator replaces the value with
     */
    float max;

} rml_op_clip_params;

#define RML_OP_CLIP_DEFAULT_MIN 0.f
#define RML_OP_CLIP_DEFAULT_MAX 1.f

/**
 * @brief Parameters for the #RML_OP_CONST operation
 *
 * RML_OP_CONST operation copy constant tensor data from host to device
 */
typedef struct _rml_op_const_params
{
    /**
     * Information about tensor. It must have all dimensions specified.
     */
    rml_tensor_info tensor_info;

    /**
     * Constant data.
     */
    void const* tensor_data;

} rml_op_const_params;

/**
 * @brief Parameters for the #RML_OP_CONV_2D operation
 */
typedef struct _rml_op_conv_2d_params
{
    /**
     * Operation that outputs tensor with input data.
     */
    rml_op input;

    /**
     * Constant operation that outputs tensor with the filter weights.
     */
    rml_op weights;

    /**
     * Padding calculation algorithm. See #rml_padding_type.
     */
    rml_padding_type padding_type;

    /**
     * Stride along each spatial axis.
     * If it holds zeros, the stride is set to 1 along each spatial axis.
     */
    rml_size_2d strides;

    /**
     * Dilation value along each spatial axis of the kernel.
     * If it holds zeros, the dilation is set to 1 along each spatial axis.
     */
    rml_size_2d dilations;

    /**
     * Number of pixels added to the beginning of the each spatial axis.
     * Only make sense if padding type is #RML_PADDING_EXPLICIT.
     */
    rml_size_2d start_paddings;

    /**
     * Number of pixels added to the end of the each spatial axis.
     * Only make sense if padding type is #RML_PADDING_EXPLICIT.
     */
    rml_size_2d end_paddings;

    /**
     * Number of groups input channels and output channels are divided into.
     * A zero value is replaced with the default value 1.
     */
    uint32_t num_groups;

} rml_op_conv_2d_params;

/**
 * @brief Parameters for the #RML_OP_CONV_2D_TRANSPOSE operation
 */
typedef struct _rml_op_conv_2d_transpose_params
{
    /**
     * Operation that outputs tensor with input data.
     */
    rml_op input;

    /**
     * Constant operation that outputs tensor with the filter weights.
     */
    rml_op weights;

    /**
     * Padding calculation algorithm. See #rml_padding_type.
     */
    rml_padding_type padding_type;

    /**
     * Stride along each spatial axis.
     * If it holds zeros, the stride is set to 1 along each spatial axis.
     */
    rml_size_2d strides;

    /**
     * Dilation value along each spatial axis of the kernel.
     * If it holds zeros, the dilation is set to 1 along each spatial axis.
     */
    rml_size_2d dilations;

    /**
     * Number of pixels added to the beginning of the each spatial axis.
     * Only make sense if padding type is #RML_PADDING_EXPLICIT.
     */
    rml_size_2d start_paddings;

    /**
     * Number of pixels added to the end of the each spatial axis.
     * Only make sense if padding type is #RML_PADDING_EXPLICIT.
     */
    rml_size_2d end_paddings;

    /**
     * Number of groups input channels and output channels are divided into.
     * A zero value is replaced with the default value 1.
     */
    uint32_t num_groups;

    /**
     * The shape of the output tensor.
     */
    rml_size_2d output_shape;

    /**
     * The zero-padding added to one side of the output.
     */
    rml_size_2d output_paddings;

} rml_op_conv_2d_transpose_params;

#define RML_OP_CONV_2D_DEFAULT_NUM_GROUPS 1

/**
 * @brief Parameters for the #RML_OP_DEPTH_TO_SPACE operation
 * that rearranges tensor data from channels into blocks of spatial data.
 *
 * More specifically, this op outputs a copy of the input tensor
 * where values from the channel dimension are moved in spatial blocks
 * to the height and width dimensions. For example,
 * rearranges tensor of shape (N, C * bs^2, H, W) to a tensor of shape (N, C, H * bs, W * bs).
 */
typedef struct _rml_op_depth_to_space_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

    /**
     * Size of data blocks are rearranged
     */
    uint32_t block_size;

} rml_op_depth_to_space_params;

/**
 * @brief Parameters for the #RML_OP_PLACEHOLDER operation
 *
 * Represents placeholder for a tensor that will be fed as model input
 */
typedef struct _rml_op_placeholder_params
{
    /**
     * Information about input tensor. It may have some dimension unspecified.
     */
    rml_tensor_info tensor_info;

} rml_op_placeholder_params;

/**
 * @brief Parameters for the #RML_OP_POOL_2D_AVG, #RML_OP_POOL_2D_MIN and #RML_OP_POOL_2D_MAX
 * operations.
 *
 * RML_OP_POOL_2D* operations apply pooling across the tensor according to
 * kernel size, strides and dilations.
 */
typedef struct _rml_op_pool_2d_params
{
    /**
     * Operation that outputs tensor with input data.
     */
    rml_op input;

    /**
     * Padding calculation algorithm. See #rml_padding_type.
     */
    rml_padding_type padding_type;

    /**
     * Spatial sizes of pooling kernel.
     */
    rml_size_2d kernel_size;

    /**
     * Stride along each spatial axis.
     * If it holds zeros, the stride is set to 1 along each spatial axis.
     */
    rml_size_2d strides;

    /**
     * Dilation value along each spatial axis of the kernel.
     * If it holds zeros, the dilation is set to 1 along each spatial axis.
     */
    rml_size_2d dilations;

    /**
     * Number of pixels added to the beginning of the each spatial axis.
     * Only make sense if padding type is #RML_PADDING_EXPLICIT.
     */
    rml_size_2d start_paddings;

    /**
     * Number of pixels added to the end of the each spatial axis.
     * Only make sense if padding type is #RML_PADDING_EXPLICIT.
     */
    rml_size_2d end_paddings;

    /**
     * Whether to use ceil (RML_TRUE) or floor (RML_FALSE) to compute the output shape.
     */
    rml_bool ceil_mode;

} rml_op_pool_2d_params;

/**
 * @brief The global poolng operation parameters
 */
typedef struct _rml_op_pool_2d_global_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

} rml_op_pool_2d_global_params;

/**
 * @brief The port operation parameters.
 *
 * By default every operation uses the first (0) port of its inputs.
 * The #RML_OP_PORT allows to access also other ports.
 */
typedef struct _rml_op_port_params
{
    /**
     * Operation that outputs one or more tensors.
     */
    rml_op input;

    /**
     * Index of an output (port) of the input operation.
     */
    uint32_t index;

} rml_op_port_params;

/**
 * @brief The pow operation parameters
 */
typedef struct _rml_op_pow_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

    /**
     * Operation that outputs tensor containing pow data
     */
    rml_op pow;

} rml_op_pow_params;

/**
 * @brief The linear quantization parameters
 */
typedef struct _rml_op_quantize_linear_params
{
    /**
     * Operation that outputs tensor containing hight-precision input data
     */
    rml_op input;

    /**
     * Operation that outputs tensor containing scale data
     */
    rml_op scale;

    /**
     * Operation that outputs tensor containing zero point data
     */
    rml_op zero_point;

} rml_op_quantize_linear_params;

/**
 * @brief Parameters for the #RML_OP_SPACE_TO_DEPTH operation.
 *
 * RML_OP_SPACE_TO_DEPTH operation rearranges blocks of spatial tensor data into channels.
 * More specifically, this op outputs a copy of the input tensor
 * where values from the height and width dimensions are moved to the channel dimension.
 * E.g., rearranges tensor of shape (N, C, H * bs, W * bs) to a tensor of shape (N, C * bs^2, H, W).
 */

typedef struct _rml_op_space_to_depth_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

    /**
     * Size of data blocks are rearranged
     */
    uint32_t block_size;

} rml_op_space_to_depth_params;

/**
 * @brief Parameters for the #RML_OP_RESIZE_2D_NEAREST operation.
 *
 * RML_OP_RESIZE_2D_NEAREST operation resize input tensor to output spatial shape specified
 * by @p size using nearest neighbour algorithm.
 */
typedef struct _rml_op_resize_2d_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

    /**
     * Output spatial size
     */
    rml_op size;

    /**
     * Output spatial scales
     */
    rml_op scales;

} rml_op_resize_2d_params;

/**
 * @brief Parameters for all the RML_OP_* operations
 */
typedef struct _rml_op_unary_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

} rml_op_unary_params;

/**
 * @brief Parameters for the #RML_OP_CEIL and #RML_OP_FLOOR operations.
 */
typedef struct _rml_op_round_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

} rml_op_round_params;

/**
 * Parameters for the element-wise binary operations #RML_OP_ADD, #RML_OP_DIV, etc
 */
typedef struct _rml_op_binary_params
{
    /**
     * Operation that outputs tensor containing the first input data
     */
    rml_op input1;

    /**
     * Operation that outputs tensor containing the second input data
     */
    rml_op input2;

} rml_op_binary_params;

/**
 * @brief Parameters for the #RML_OP_RESHAPE operation.
 *
 * RML_OP_RESHAPE operation reshapes the input tensor similar to numpy.reshape.
 * The @p input is the data tensor, the @p shape is a tensor that specifies the output shape.
 */
typedef struct _rml_op_reshape_params
{
    /**
     * Operation that outputs tensor containing data to reshape
     */
    rml_op input;

    /**
     * Operation that outputs tensor containing shape data
     */
    rml_op shape;

} rml_op_reshape_params;

/**
 *  @brief Parameters for the #RML_OP_BIAS_ADD operation.
 */
typedef struct _rml_op_bias_add_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;
    /**
     * Operation that outputs tensor containing bias data
     */
    rml_op bias;

} rml_op_bias_add_params;

/**
 * @brief Parameters for the #RML_OP_CONCAT operation.
 *
 * RML_OP_CONCAT operation concatenates a list of tensors along the specified dimension.
 * All tensors MUST have the same shape and type, except for the dimension size of the axis to
 * concatenate on.
 */
typedef struct _rml_op_concat_params
{
    /**
     * A number of input operations
     */
    size_t num_inputs;

    /**
     * A list of operations that outputs tensors containing input data
     */
    rml_op* inputs;

    /**
     * The axis to stack along.
     * A negative value means counting dimensions from back.
     * Accepted range is [-R, R-1] where R = rank(inputs[i]).
     */
    rml_op axis;

} rml_op_concat_params;

/**
 * @brief Parameters for the #RML_OP_STACK operation.
 *
 * RML_OP_STACK operation Stacks a list of tensors with rank R
 * into single tensor with rank R+1. All tensors MUST have the same shape and type.
 */
typedef struct _rml_op_stack_params
{
    /**
     * A number of input operations.
     */
    size_t num_inputs;

    /**
     * A list of operations that output tensors containing input data.
     */
    rml_op* inputs;

    /**
     * The axis to stack along. A negative value means counting dimensions from back.
     * Accepted range is [-R, R-1] where R = rank(inputs[i]).
     */
    int axis;

} rml_op_stack_params;

/**
 * @brief Parameters for the #RML_OP_SQUEEZE operation.
 *
 * RML_OP_SQUEEZE operation removes single-dimensional entries from the shape of a tensor.
 */
typedef struct _rml_op_squeeze_params
{
    /**
     * Operation that outputs tensor containing the input data
     */
    rml_op input;

    /**
     * Number of axes to squeeze.
     */
    size_t num_axes;

    /**
     * List of integers indicating the dimensions to squeeze.
     * Negative value means counting dimensions from the back.
     * Accepted range is [-R, R-1] where R = rank(data).
     */
    int32_t axes[RML_TENSOR_MAX_RANK];

} rml_op_squeeze_params;

/**
 * @brief Parameters for the #RML_OP_SLICE operation.
 *
 * Semantic is the same as for the start:stop:step notation in Numpy.
 * Additionally, "axes" specifies axes to apply slice
 * https://docs.scipy.org/doc/numpy/reference/arrays.indexing.html
 */
typedef struct _rml_op_slice_params
{
    /**
     * Operation that outputs tensor of data to extract slices from
     */
    rml_op input;

    /**
     * Operaton that outputs 1-D tensor of axes that `starts` and `ends` apply to.
     * Negative value means counting dimensions from the back.
     * Accepted range is [-R, R-1] where R = rank(data).
     */
    rml_op axes;

    /**
     * Operaton that outputs 1-D tensor of starting indices of corresponding axis in
     * rml_op_slice_params#axes.
     */
    rml_op starts;

    /**
     * Operaton that outputs 1-D tensor of ending indices (exclusive) of corresponding axis in
     * rml_op_slice_params#axes.
     */
    rml_op ends;

    /**
     * Operation that outputs tensor of slice step of corresponding axis in `axes`.
     * Negative value means slicing backward. 'steps' cannot be 0.
     * Only stride=1 is supported now
     */
    rml_op strides;

} rml_op_slice_params;

/**
 * @brief Params for reduce operation: #RML_OP_REDUCE_AVG, #RML_OP_REDUCE_L1,
 * #RML_OP_REDUCE_L2, #RML_OP_REDUCE_MUL, #RML_OP_REDUCE_LOGN_ADD, #RML_OP_REDUCE_LOGN_ADD_EXP,
 * #RML_OP_REDUCE_MAX, #RML_OP_REDUCE_MIN, #RML_OP_REDUCE_ADD_SQUARE.
 */
typedef struct _rml_op_reduce_params
{
    /**
     * Operation that outputs tensor containing the input data
     */
    rml_op input;

    /**
     * If keepdims is true, the reduced dimensions are retained with length 1.
     */
    rml_bool keep_dims;

    /**
     * Number of axes to reduce.
     */
    size_t num_axes;

    /**
     * List of integers indicating the dimensions to reduce.
     * Negative value means counting dimensions from the back.
     * Accepted range is [-R, R-1] where R = rank(data).
     */
    int32_t axes[RML_TENSOR_MAX_RANK];

} rml_op_reduce_params;

/**
 * @brief Parameters for the #RML_OP_TRANSPOSE operation.
 *
 * Transpose the @p input tensor similar to numpy.transpose. For example, when perm=(1, 0, 2), given
 * an input tensor of shape (1, 2, 3), the output shape will be (2, 1, 3).
 *
 * NOTE: This operator currently is ONLY for internal usage. Don't use it directly.
 */
typedef struct _rml_op_transpose_params
{
    /**
     * Operation that outputs tensor containing input data.
     */
    rml_op input;

    /**
     * Number of axes that MUST be equal to rank of input tensor.
     */
    size_t num_axes;

    /** A nullable list of axis.
     * If null, reverse the dimensions,
     * otherwise permute the axes according to the values given.
     */
    int32_t axes[RML_TENSOR_MAX_RANK];

} rml_op_transpose_params;

/**
 * @brief Parameters for the #RML_OP_UNSQUEEZE operation.
 *
 * Inserts a dimension of size 1 at the dimension index axis of input's shape.
 */
typedef struct _rml_op_unsqueeze_params
{
    /**
     * Operation that outputs tensor containing input data.
     */
    rml_op input;

    /**
     * Number of axes to unsqueeze.
     */
    size_t num_axes;

    /**
     * List of integers indicating the dimensions to unsqueeze.
     * Negative value means counting dimensions from the back.
     * Accepted range is [-R, R-1] where R = rank(data).
     */
    int32_t axes[RML_TENSOR_MAX_RANK];

} rml_op_unsqueeze_params;

/**
 * @brief Parameters for the #RML_OP_PAD operation.
 *
 * Describes a data reorganization operator that inflates the input tensor with zeroes (or some
 * other value) on the edges.
 */
typedef struct _rml_op_pad_params
{
    /**
     * Operation that outputs tensor containing input data.
     */
    rml_op input;

    /**
     * The padding mode to use.
     */
    rml_pad_mode mode;

    /**
     * The value with which to pad.
     */
    float value;

    /**
     * The number of dimensions.
     * Determines the size of the start_padding and end_padding arrays.
     */
    size_t num_dims;

    /**
     * An array of UINT containing the padding (number of pixels added)
     * to the start of the corresponding axis. Padding defaults to 0 along the start and end of each
     * axis
     */
    uint32_t start_padding[RML_TENSOR_MAX_RANK];

    /**
     * An array containing the padding (number of pixels added)
     * to the end of the corresponding axis. Padding defaults to 0 along the start and end of each
     * axis.
     */
    uint32_t end_padding[RML_TENSOR_MAX_RANK];

} rml_op_pad_params;

/**
 * @brief Parameters for the #RML_OP_GEMM operation.
 *
 * Y = alpha * transA(A) * transB(B) + beta * C
 */
typedef struct _rml_op_gemm_params
{
    /**
     * Operation that outputs tensor containing input A
     */
    rml_op input_a;

    /**
     * Operation that outputs tensor containing input B
     */
    rml_op input_b;

    /**
     * Operation that outputs tensor containing input C, optional
     */
    rml_op input_c;

    /**
     * A scalar multiplier for the product of inputs A and B
     */
    float alpha;

    /**
     * A scalar multiplier for the optional input C
     */
    float beta;

    /**
     * A transform to be applied to the A tensor; either a transpose, or no transform
     */
    rml_bool transpose_a;

    /**
     * A transform to be applied to the B tensor; either a transpose, or no transform
     */
    rml_bool transpose_b;

} rml_op_gemm_params;

#define RML_OP_GEMM_DEFAULT_ALPHA 1.f
#define RML_OP_GEMM_DEFAULT_BETA 1.f

/**
 * @brief Parameters for the #RML_OP_LOCAL_RESPONSE_NORM operation.
 */
typedef struct _rml_op_local_response_norm_params
{
    /**
     * Operation that outputs tensor containing input tensor data
     */
    rml_op input;

    /**
     * The number of channels to sum over (for cross channel LRN) or
     * the side length of the square region to sum over (for within channel LRN)
     */
    uint32_t size;

    /**
     * Scaling parameter
     */
    float alpha;

    /**
     * Exponent parameter
     */
    float beta;

    /**
     * Bias parameter
     */
    float bias;

    /**
     * Whether to sum over adjacent channels (for cross channel LRN) or
     * nearby spatial locations (for within channel LRN)
     */
    rml_bool cross_channel;

} rml_op_local_response_norm_params;

/**
 * @brief Parameters for the #RML_OP_TOP_K operation
 *
 */
typedef struct _rml_op_top_k_params
{
    /**
     * Operation that outputs tensor containing input data
     */
    rml_op input;

    /**
     * Dimension on which to do the sort. Negative value means counting dimensions from the back. 
     * Accepted range is [-r, r-1] where r = rank(input).
     */
    int32_t axis;

     /**
      * Number of returned elements.
      */
    uint32_t k;

} rml_op_top_k_params;

#define RML_OP_LRN_DEFAULT_ALPHA 0.0001f
#define RML_OP_LRN_DEFAULT_BETA 0.75f
#define RML_OP_LRN_DEFAULT_BIAS 1.0f

typedef struct _rml_op_celu_params
{
    /**
     * Operation that outputs tensor containing the input data
     */
    rml_op input;

    /**
     * Scaling and exponent parameter.
     */
    float alpha;

} rml_op_celu_params;

typedef struct _rml_op_desc
{
    rml_op_type op_type;
    char const* op_name;

    union
    {
        /** @see #RML_OP_ABS, #rml_op_unary_params */
        rml_op_unary_params abs;

        /** @see #RML_OP_ACOS, #rml_op_unary_params */
        rml_op_unary_params acos;

        /** @see #RML_OP_ADD, #rml_op_binary_params */
        rml_op_binary_params add;

        /** @see #RML_OP_ASIN, #rml_op_unary_params */
        rml_op_unary_params asin;

        /** @see #RML_OP_ATAN, #rml_op_unary_params */
        rml_op_unary_params atan;

        /** @see #RML_OP_AVG, #rml_op_binary_params */
        rml_op_binary_params avg;

        /** @see #RML_OP_BATCH_NORM, #rml_op_batch_norm_params */
        rml_op_batch_norm_params batch_norm;

        /** @see #RML_OP_BIAS_ADD, #rml_op_bias_add_params */
        rml_op_bias_add_params bias_add;

        /** @see #RML_OP_CAST, #rml_op_cast_params */
        rml_op_cast_params cast;

        /** @see #RML_OP_CEIL, #rml_op_unary_params */
        rml_op_unary_params ceil;

        /** @see #RML_OP_CELU, #rml_op_celu_params */
        rml_op_celu_params celu;

        /** @see #RML_OP_CLIP, #rml_op_clip_params */
        rml_op_clip_params clip;

        /** @see #RML_OP_CONCAT, #rml_op_concat_params */
        rml_op_concat_params concat;

        /** @see #RML_OP_CONST, #rml_op_const_params */
        rml_op_const_params constant;

        /** @see #RML_OP_CONV_2D, #rml_op_conv_2d_params */
        rml_op_conv_2d_params conv_2d;

        /** @see #RML_OP_CONV_2D_DEPTHWISE, #rml_op_conv_2d_params */
        rml_op_conv_2d_params conv_2d_depthwise;

        /** @see #RML_OP_CONV_2D_TRANSPOSE, #rml_op_conv_2d_transpose_params */
        rml_op_conv_2d_transpose_params conv_2d_transpose;

        /** @see #RML_OP_COS, #rml_op_unary_params */
        rml_op_unary_params cos;

        /** @see #RML_OP_DEPTH_TO_SPACE, #rml_op_depth_to_space_params */
        rml_op_depth_to_space_params depth_to_space;

        /** @see #RML_OP_DIV, #rml_op_binary_params */
        rml_op_binary_params div;

        /** @see #RML_OP_ELU, #rml_op_elu_params */
        rml_op_elu_params elu;

        /** @see #RML_OP_EXP, #rml_op_unary_params */
        rml_op_unary_params exp;

        /** @see #RML_OP_FLATTEN, #rml_op_unary_params */
        rml_op_unary_params flatten;

        /** @see #RML_OP_FLOOR, #rml_op_unary_params */
        rml_op_unary_params floor;

        /** @see #RML_OP_GEMM, #rml_op_gemm_params */
        rml_op_gemm_params gemm;

        /** @see #RML_OP_IDENTITY, #rml_op_unary_params */
        rml_op_unary_params identity;

        /** @see #RML_OP_LEAKY_RELU, #rml_op_leaky_relu_params */
        rml_op_leaky_relu_params leaky_relu;

        /** @see #RML_OP_LOCAL_RESPONSE_NORM, #rml_op_local_response_norm_params */
        rml_op_local_response_norm_params local_response_norm;

        /** @see #RML_OP_LOG_SOFTMAX, #rml_op_unary_params */
        rml_op_unary_params log_softmax;

        /** @see #RML_OP_LOGN, #rml_op_unary_params */
        rml_op_unary_params logn;

        /** @see #RML_OP_MAX, #rml_op_binary_params */
        rml_op_binary_params max;

        /** @see #RML_OP_MIN, #rml_op_binary_params */
        rml_op_binary_params min;

        /** @see #RML_OP_MUL, #rml_op_binary_params */
        rml_op_binary_params mul;

        /** @see #RML_OP_NEG, #rml_op_unary_params */
        rml_op_unary_params neg;

        /** @see #RML_OP_PAD, #rml_op_pad_params */
        rml_op_pad_params pad;

        /** @see #RML_OP_PARAMETRIC_RELU, #rml_op_binary_params */
        rml_op_binary_params parametric_relu;

        /** @see #RML_OP_PLACEHOLDER, #rml_op_placeholder_params */
        rml_op_placeholder_params placeholder;

        /** @see #RML_OP_POOL_2D_AVG, #rml_op_pool_2d_params */
        rml_op_pool_2d_params pool_2d_avg;

        /** @see #RML_OP_POOL_2D_GLOBAL_AVG, #rml_op_pool_2d_global_params */
        rml_op_pool_2d_global_params pool_2d_global_avg;

        /** @see #RML_OP_POOL_2D_MAX, #rml_op_pool_2d_params */
        rml_op_pool_2d_params pool_2d_max;

        /** @see #RML_OP_PORT, #rml_op_port_params */
        rml_op_port_params port;

        /** @see #RML_OP_POW, #rml_op_pow_params */
        rml_op_pow_params pow;

        /** @see #RML_OP_QUANTIZE_LINEAR, #rml_op_quantize_linear_params */
        rml_op_quantize_linear_params quantize_linear;

        /** @see #RML_OP_RECIP, #rml_op_unary_params */
        rml_op_unary_params recip;

        /** @see #RML_OP_REDUCE_ADD, #rml_op_reduce_params */
        rml_op_reduce_params reduce_add;

        /** @see #RML_OP_REDUCE_ADD_SQUARE, #rml_op_reduce_params */
        rml_op_reduce_params reduce_add_square;

        /** @see #RML_OP_REDUCE_ARGMAX, #rml_op_reduce_params */
        rml_op_reduce_params reduce_argmax;

        /** @see #RML_OP_REDUCE_ARGMIN, #rml_op_reduce_params */
        rml_op_reduce_params reduce_argmin;

        /** @see #RML_OP_REDUCE_AVG, #rml_op_reduce_params */
        rml_op_reduce_params reduce_avg;

        /** @see #RML_OP_REDUCE_L1, #rml_op_reduce_params */
        rml_op_reduce_params reduce_l1;

        /** @see #RML_OP_REDUCE_L2, #rml_op_reduce_params */
        rml_op_reduce_params reduce_l2;

        /** @see #RML_OP_REDUCE_LOGN_ADD, #rml_op_reduce_params */
        rml_op_reduce_params reduce_logn_add;

        /** @see #RML_OP_REDUCE_LOGN_ADD_EXP, #rml_op_reduce_params */
        rml_op_reduce_params reduce_logn_add_exp;

        /** @see #RML_OP_REDUCE_MAX, #rml_op_reduce_params */
        rml_op_reduce_params reduce_max;

        /** @see #RML_OP_REDUCE_MIN, #rml_op_reduce_params */
        rml_op_reduce_params reduce_min;

        /** @see #RML_OP_REDUCE_MUL, #rml_op_reduce_params */
        rml_op_reduce_params reduce_mul;

        /** @see #RML_OP_RELU, #rml_op_unary_params */
        rml_op_unary_params relu;

        /** @see #RML_OP_RELU6, #rml_op_unary_params */
        rml_op_unary_params relu6;

        /** @see #RML_OP_RESHAPE, #rml_op_reshape_params */
        rml_op_reshape_params reshape;

        /** @see #RML_OP_RESIZE_2D_NEAREST, #rml_op_resize_2d_params */
        rml_op_resize_2d_params resize_2d_nearest;

        /** @see #RML_OP_RESIZE_2D_BICUBIC, #rml_op_resize_2d_params */
        rml_op_resize_2d_params resize_2d_bicubic;

        /** @see #RML_OP_RSQRT, #rml_op_unary_params */
        rml_op_unary_params rsqrt;

        /** @see #RML_OP_SELU, #rml_op_selu_params */
        rml_op_selu_params selu;

        /** @see #RML_OP_SHAPE, #rml_op_unary_params */
        rml_op_unary_params shape;

        /** @see #RML_OP_SIGMOID, #rml_op_unary_params */
        rml_op_unary_params sigmoid;

        /** @see #RML_OP_SIN, #rml_op_unary_params */
        rml_op_unary_params sin;

        /** @see #RML_OP_SLICE, #rml_op_slice_params */
        rml_op_slice_params slice;

        /** @see #RML_OP_SOFTMAX, #rml_op_unary_params */
        rml_op_unary_params softmax;

        /** @see #RML_OP_SOFTPLUS, #rml_op_unary_params */
        rml_op_unary_params softplus;

        /** @see #RML_OP_SOFTSIGN, #rml_op_unary_params */
        rml_op_unary_params softsign;

        /** @see #RML_OP_SPACE_TO_DEPTH, #rml_op_space_to_depth_params */
        rml_op_space_to_depth_params space_to_depth;

        /** @see #RML_OP_SQRT, #rml_op_unary_params */
        rml_op_unary_params sqrt;

        /** @see #RML_OP_SQUEEZE, #rml_op_squeeze_params */
        rml_op_squeeze_params squeeze;

        /** @see #RML_OP_STACK, #rml_op_stack_params */
        rml_op_stack_params stack;

        /** @see #RML_OP_SUB, #rml_op_binary_params */
        rml_op_binary_params sub;

        /** @see #RML_OP_TAN, #rml_op_unary_params */
        rml_op_unary_params tan;

        /** @see #RML_OP_TANH, #rml_op_unary_params */
        rml_op_unary_params tanh;

        /** @see #RML_OP_THRESHOLDED_RELU, #rml_op_thresholded_relu_params */
        rml_op_thresholded_relu_params thresholded_relu;

        /** @see #RML_OP_TOP_K, #rml_op_top_k_params */
        rml_op_top_k_params top_k;

        /** @see #RML_OP_TRANSPOSE, #rml_op_transpose_params */
        rml_op_transpose_params transpose;

        /** @see #RML_OP_UNSQUEEZE, #rml_op_unsqueeze_params */
        rml_op_unsqueeze_params unsqueeze;

        rml_op_binary_params binary;
        rml_op_pool_2d_params pool_2d;
        rml_op_pool_2d_global_params pool_2d_global;
        rml_op_unary_params unary;
        rml_op_reduce_params reduce;
        rml_op_resize_2d_params resize_2d;
    };

} rml_op_desc;

/**
 * Creates empty graph.
 *
 * @return A valid graph handle in case of success, NULL otherwise.
 *         To get more details in case of failure, call rmlGetLastError().
 *         The graph should be released with rmlReleaseGraph().
 */
RML_API_ENTRY rml_status rmlCreateGraph(rml_graph* graph);

/**
 * Returns input nodes (placeholders) in the graph.
 *
 * @param[in]  graph  A valid graph handle.
 * @param[out] names  A pointer to a structure with resulting input names.
 *
 * @return A model handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p graph is invalid or @p names is NULL.
 *
 * The returned strings memory is owned by the graph and must not be freed.
 * The memory may be invalidated an a next call involving the graph.
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlGetGraphInputNames(rml_graph graph, rml_strings* names);

/**
 * Returns output nodes in the graph.
 *
 * @param[in]  graph  A valid graph handle.
 * @param[out] names  A pointer to a structure with resulting output names.
 *
 * @return A model handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p graph is invalid or @p names is NULL.
 *
 * The returned strings memory is owned by the graph and must not be freed.
 * The memory may be invalidated an a next call involving the graph.
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlGetGraphOutputNames(rml_graph graph, rml_strings* names);

/**
 * Combines supplied graphs to a single graph connecting outputs and inputs of the specified
 * graphs. So, graphs in different formats (i.e. TF and ONNX) can be combined into single graph.
 *
 * @param[in] head_graph        A graph to be inserted in tail_graph.
 * @param[in] tail_graph        A graph with a nodes to be replaced after connection.
 * @param[in] num_connections   A number of nodes in head/tail graph to be connected.
 * @param[in] head_outputs      A list of nodes in head_graph to be connected.
 * @param[in] tail_inputs       A list of nodes in tail_graph where head graph output nodes
 *                              will be inserted.
 * @param[out] connected_graph  Result graph containing tail_graph and head_graph.
 *
 * @return A model handle in case of success and status:
 * - #RML_OK if the operation is successful,
 * - #RML_ERROR_BAD_PARAMETER if @p head_graph, @p tail_graph is invalid,
 *                            if @p num_connections is 0 or
 *                            if @p head_outputs, @p tail_inputs or @p connected_graph is NULL.
 *
 * To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlConnectGraphs(rml_graph head_graph,
                                          rml_graph tail_graph,
                                          size_t num_connections,
                                          const char* const* head_outputs,
                                          const char* const* tail_inputs,
                                          rml_graph* connected_graph);

/**
 * Creates constant tensor in the graph context and graph owns it.
 *
 * @param[in]  graph   A valid graph handle.
 * @param[in]  op_desc A pointer to operation description structure.
 * @param[out] op      The pointer to a resulting operation handle.
 *
 * @return A valid operation handle in case of success, NULL otherwise.
 *         To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlCreateOperation(rml_graph graph,
                                            const rml_op_desc* op_desc,
                                            rml_op* op);

/**
 * Releases a graph created with rmlCreateGraph() or rmlLoadgraph(), invalidates the handle.
 *
 * @param[in] graph A valid graph handle.
 */
RML_API_ENTRY void rmlReleaseGraph(rml_graph graph);

#ifdef __cplusplus
} // extern "C"
#endif
