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

#include "RadeonML.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A mutable operation graph.
 */
typedef struct _rml_graph* rml_graph;

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
    RML_OP_ABS = 1010,                          /**< @brief rml_op_desc#abs */
    RML_OP_ACOS = 1020,                         /**< @brief rml_op_desc#acos */
    RML_OP_ADD = 1030,                          /**< @brief rml_op_desc#add */
    RML_OP_ASIN = 1060,                         /**< @brief rml_op_desc#asin */
    RML_OP_ATAN = 1070,                         /**< @brief rml_op_desc#atan */
    RML_OP_BATCH_NORMALIZATION = 1080,          /**< @brief rml_op_desc#batch_norm */
    RML_OP_BIAS_ADD = 1090,                     /**< @brief rml_op_desc#bias_add */
    RML_OP_CAST = 1100,                         /**< @brief rml_op_desc#cast */
    RML_OP_CEIL = 1110,                         /**< @brief rml_op_desc#ceil */
    RML_OP_CLIP = 1130,                         /**< @brief rml_op_desc#clip */
    RML_OP_CONCAT = 1140,                       /**< @brief rml_op_desc#concat */
    RML_OP_CONST = 1150,                        /**< @brief rml_op_desc#constant */
    RML_OP_CONV_2D = 1180,                      /**< @brief rml_op_desc#conv2d */
    RML_OP_CONV_2D_DEPTHWISE = 1190,            /**< @brief rml_op_desc#conv2d_depthwise */
    RML_OP_COS = 1230,                          /**< @brief rml_op_desc#cos */
    RML_OP_DEPTH_TO_SPACE = 1240,               /**< @brief rml_op_desc#depth2space */
    RML_OP_DIV = 1260,                          /**< @brief rml_op_desc#div */
    RML_OP_ELU = 1280,                          /**< @brief rml_op_desc#elu */
    RML_OP_EXP = 1290,                          /**< @brief rml_op_desc#exp */
    RML_OP_FLATTEN = 1300,                      /**< @brief rml_op_desc#flatten */
    RML_OP_FLOOR = 1310,                        /**< @brief rml_op_desc#floor */
    RML_OP_GEMM = 1330,                         /**< @brief rml_op_desc#gemm */
    RML_OP_IDENTITY = 1340,                     /**< @brief rml_op_desc#identity */
    RML_OP_LEAKY_RELU = 1350,                   /**< @brief rml_op_desc#leaky_relu */
    RML_OP_LOCAL_RESPONSE_NORMALIZATION = 1360, /**< @brief rml_op_desc#lrn */
    RML_OP_LOG_SOFTMAX = 1370,                  /**< @brief rml_op_desc#log_softmax */
    RML_OP_LOGN = 1380,                         /**< @brief rml_op_desc#logn */
    RML_OP_MAX = 1390,                          /**< @brief rml_op_desc#max */
    RML_OP_MEAN = 1400,                         /**< @brief rml_op_desc#mean */
    RML_OP_MIN = 1410,                          /**< @brief rml_op_desc#min */
    RML_OP_MUL = 1420,                          /**< @brief rml_op_desc#mul */
    RML_OP_NEG = 1430,                          /**< @brief rml_op_desc#neg */
    RML_OP_PARAMETRIC_RELU = 1440,              /**< @brief rml_op_desc#parametric_relu */
    RML_OP_PLACEHOLDER = 1450,                  /**< @brief rml_op_desc#placeholder */
    RML_OP_POOL_2D_AVG = 1490,                  /**< @brief rml_op_desc#pool2d_avg */
    RML_OP_POOL_2D_AVG_GLOBAL = 1500,           /**< @brief rml_op_desc#pool2d_avg_global */
    RML_OP_POOL_2D_MAX = 1510,                  /**< @brief rml_op_desc#pool2d_max */
    RML_OP_POOL_2D_MIN = 1520,                  /**< @brief rml_op_desc#pool2d_min */
    RML_OP_POW = 1560,                          /**< @brief rml_op_desc#pow */
    RML_OP_RECIP = 1580,                        /**< @brief rml_op_desc#recip */
    RML_OP_RELU = 1650,                         /**< @brief rml_op_desc#relu */
    RML_OP_RELU6 = 1660,                        /**< @brief rml_op_desc#relu6 */
    RML_OP_RESHAPE = 1670,                      /**< @brief rml_op_desc#reshape */
    RML_OP_RESIZE_2D = 1680,                    /**< @brief rml_op_desc#resize2d */
    RML_OP_RSQRT = 1730,                        /**< @brief rml_op_desc#rsqrt */
    RML_OP_SELU = 1740,                         /**< @brief rml_op_desc#selu */
    RML_OP_SHAPE = 1750,                        /**< @brief rml_op_desc#shape */
    RML_OP_SIGMOID = 1760,                      /**< @brief rml_op_desc#sigmoid */
    RML_OP_SIN = 1770,                          /**< @brief rml_op_desc#sin */
    RML_OP_SLICE = 1780,                        /**< @brief rml_op_desc#slice */
    RML_OP_SOFTMAX = 1790,                      /**< @brief rml_op_desc#softmax */
    RML_OP_SOFTPLUS = 1810,                     /**< @brief rml_op_desc#softplux */
    RML_OP_SOFTSIGN = 1820,                     /**< @brief rml_op_desc#softsign */
    RML_OP_SPACE_TO_DEPTH = 1830,               /**< @brief rml_op_desc#space2depth */
    RML_OP_SQRT = 1850,                         /**< @brief rml_op_desc#sqrt */
    RML_OP_SQUEEZE = 1860,                      /**< @brief rml_op_desc#squeeze */
    RML_OP_STACK = 1870,                        /**< @brief rml_op_desc#stack */
    RML_OP_SUB = 1880,                          /**< @brief rml_op_desc#sub */
    RML_OP_TAN = 1890,                          /**< @brief rml_op_desc#tan */
    RML_OP_TANH = 1900,                         /**< @brief rml_op_desc#tanh */
    RML_OP_THRESHOLDED_RELU = 1910,             /**< @brief rml_op_desc#thresholded_relu */
    RML_OP_UNSQUEEZE = 1940,                    /**< @brief rml_op_desc#unsqueeze */

} rml_op_type;

typedef enum _rml_interpolation_type
{
    RML_INTERPOLATION_UNSPECIFIED = 0,
    RML_INTERPOLATION_NEAREST = 520,

} rml_interpolation_type;

/**
 * @brief The padding calculation algorithm.
 *
 * If padding type is one of {RML_PADDING_SAME_LOWER, RML_PADDING_SAME_UPPER, RML_PADDING_SAME},
 * then paddings will be added to output_spatial_size[i] = ceil(input_spatial_size[i] / strides[i]).
 * In case of odd number add the extra padding at the end for RML_PADDING_SAME_UPPER
 * and at the beginning for RML_PADDING_SAME_LOWER.
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
 * @brief Represents any of 2D height/width values
 */
typedef struct _rml_size_2d
{
    uint32_t h;
    uint32_t w;

} rml_size_2d;

/**
 * Params for element-wise operation RML_OP_ELU
 * ELU(x) = max(0, x) + min(0, alpha * (exp(x) - 1))
 */
typedef struct _rml_op_elu_params
{
    rml_op input; /**< Operation that outputs tensor containing input data */

    float alpha;  /**< The alpha value for the ELU formulation. Usually is 1.0 */

} rml_op_elu_params;

#define RML_OP_ELU_DEFAULT_ALPHA 1.f

/**
 * Params for element-wise operation RML_OP_LEAKY_RELU
 * LeakyReLU(x) = max(0, x) + alpha * min(0, x)
 */
typedef struct _rml_op_leaky_relu_params
{
    rml_op input; /**< Operation that outputs tensor containing input data */

    float alpha;  /**< The alpha value for the LeakyReLU formulation. Controls the angle of the
                       negative slope. Usually is 0.2f */

} rml_op_leaky_relu_params;

#define RML_OP_LEAKY_RELU_DEFAULT_ALPHA 0.2f

/**
 * Params for element-wise operation RML_OP_SELU
 * SeLU(x) = gamma * (alpha * e^x - alpha)
 */
typedef struct _rml_op_selu_params
{
    rml_op input; /**< Operation that outputs tensor containing input data */

    float alpha; /**< The alpha value for the SeLU formulation.*/
    float gamma; /**< The gamma value for the SeLU formulation.*/

} rml_op_selu_params;

#define RML_OP_SELU_DEFAULT_ALPHA 1.6733f
#define RML_OP_SELU_DEFAULT_GAMMA 1.0507f

/**
 * Params for element-wise operation RML_OP_THRESHOLDED_RELU
 * ThReLU(x) = x, x > alpha
 * ThReLU(x) = 0, x <= alpha
 */
typedef struct _rml_op_thresholded_relu_params
{
    rml_op input; /**< Operation that outputs tensor containing input data */

    float alpha;  /**< The threshold value for the ThReLU formulation.*/

} rml_op_thresholded_relu_params;

#define RML_OP_THRESHOLDED_RELU_DEFAULT_ALPHA 0.f

/**
 * Params for operation RML_OP_BATCH_NORMALIZATION
 * f(x) = scale * (x - mean) / sqrt(variance + epsilon) + bias.
 */
typedef struct _rml_op_batch_norm_params
{
    rml_op input;    /**< Operation that outputs tensor containing input data */

    rml_op mean;     /**< Constant operation that outputs tensor containing the batch mean */
    rml_op variance; /**< Constant operation that outputs tensor containing the batch variance */
    rml_op scale;    /**< Constant operation that outputs tensor containing the scale */
    rml_op bias;     /**< Constant operation that outputs tensor containing the batch bias */

    float epsilon;   /**< Small value to use to avoid division by zero */

} rml_op_batch_norm_params;

#define RML_OP_BATCH_NORMALIZATION_DEFAULT_EPSILON 1e-5f

/**
 * Params for operation RML_OP_CAST
 * Performs the cast function f(x) = cast(x),
 * casting each element in the input to the data type of the output tensor
 */
typedef struct _rml_op_cast_params
{
    rml_op input;      /**< Operation that outputs tensor containing input data */
    rml_dtype cast_to;  /**< Output data type */

} rml_op_cast_params;

/**
 * Params for operation RML_OP_CLIP
 * f(x) = clamp(x, min, max)
 */
typedef struct _rml_op_clip_params
{
    rml_op input; /**< Operation that outputs tensor containing input data */
    float min;    /**< \brief The minimum value, below which the operator replaces the value with */
    float max;    /**< \brief The maximum value, above which the operator replaces the value with */

} rml_op_clip_params;

#define RML_OP_CLIP_DEFAULT_MIN 0.f
#define RML_OP_CLIP_DEFAULT_MAX 1.f

typedef struct _rml_op_const_params
{
    rml_tensor_info tensor_info;
    void const* tensor_data;

} rml_op_const_params;

/**
 * @brief The convolution operation parameters
 */
typedef struct _rml_op_convolution_2d_params
{
    rml_op input;           /**< Operation that outputs tensor containing input data */
    rml_op weights;         /**< Constant operation that outputs tensor containing the filter weights */

    rml_padding_type padding_type; /**< Padding calculation algorithm. See @ref rml_padding_type*/

    rml_size_2d strides;        /**< Stride along each spatial axis.
                                     If not present, the stride defaults is 1 along each spatial axis. */
    rml_size_2d dilations;      /**< Dilation value along each spatial axis of the kernel.
                                     If not present, the dilation defaults is 1 along each spatial axis. */
    rml_size_2d start_paddings; /**< Number of pixels added to the beginning of the each spatialaxis. 
                                     Only make sense if padding type is RML_PADDING_EXPLICIT.
                                     If not present, the padding defaults to 0 along start of each spatial axis. */
    rml_size_2d end_paddings;   /**< Number of pixels added to the end of the each spatial axis.
                                     Only make sense if padding type is RML_PADDING_EXPLICIT.
                                     If not present, the padding defaults to 0 along end of each spatial axis. */
    uint32_t num_groups;        /**< Number of groups input channels and output channels are divided into */
} rml_op_convolution_2d_params;

#define RML_OP_CONV_2D_DEFAULT_NUM_GROUPS   1

typedef struct _rml_op_depth_to_space_params
{
    rml_op input; /**< \brief Operation that outputs tensor containing input data */

    uint32_t block_size;

} rml_op_depth_to_space_params;

typedef struct _rml_op_placeholder_params
{
    rml_tensor_info tensor_info;

} rml_op_placeholder_params;

/**
* @brief The poolng operation parameters
*/
typedef struct _rml_op_pooling_2d_params
{
    rml_op input; /**< Operation that outputs tensor containing input data */

    rml_padding_type padding_type; /**< Padding calculation algorithm. See @ref rml_padding_type*/

    rml_size_2d kernel_size;    /**< Spatial sizes of polling kernel */
    rml_size_2d strides;        /**< Stride along each spatial axis. 
                                     If not present, the stride defaults is 1 along each spatial axis. */
    rml_size_2d dilations;      /**< Dilation value along each spatial axis of the kernel. 
                                     If not present, the dilation defaults is 1 along each spatial axis. */
    rml_size_2d start_paddings; /**< Number of pixels added to the beginning of the each spatial axis.
                                     Only make sense if padding type is RML_PADDING_EXPLICIT. 
                                     If not present, the padding defaults to 0 along start of each spatial axis. */
    rml_size_2d end_paddings;   /**< Number of pixels added to the end of the each spatial axis.
                                     Only make sense if padding type is RML_PADDING_EXPLICIT. 
                                     If not present, the padding defaults to 0 along end of each spatial
                                     axis. */
    rml_bool ceil_mode;         /**< Whether to use ceil or floor (default) to compute the output shape. */
} rml_op_pooling_2d_params;

typedef struct _rml_op_space_to_depth_params
{
    rml_op input; /**< \brief Operation that outputs tensor containing input data */

    uint32_t block_size;

} rml_op_space_to_depth_params;

typedef struct _rml_op_resize_2d_params
{
    rml_op input; /**< Operation that outputs tensor containing input data */
    rml_op size;

    rml_interpolation_type interpolation_type;
    rml_bool align_corners;

} rml_op_resize_2d_params;

/**
 * @brief Parameters for all RML_OP_* operations
 */
typedef struct _rml_op_unary_params
{
    rml_op input; /**< Operation that outputs tensor containing input data */

} rml_op_unary_params;

/**
 * @brief Parameters for operations RML_OP_CEIL and RML_OP_FLOOR
 */
typedef struct _rml_op_round_params
{
    rml_op input; /**< Operation that outputs tensor containing input data */

} rml_op_round_params;

/**
 * Params for element-wise binary operations Add, Sub, Mul, Div, Min, Max, Mean
 */
typedef struct _rml_op_binary_params
{
    rml_op input1; /**< Operation that outputs tensor containing the first input data */
    rml_op input2; /**< Operation that outputs tensor containing the second input data */

} rml_op_binary_params;

/**
 * Params for reshape operation
 */
typedef struct _rml_op_reshape_params
{
    rml_op input;
    rml_op shape;

    rml_layout axes_layout;

} rml_op_reshape_params;

/**
 * Params for bias_add operation
 */
typedef struct _rml_op_bias_add_params
{
    rml_op input;
    rml_op bias;

} rml_op_bias_add_params;

/**
 * @brief Parameters for RML_OP_CONCAT operation.
 *
 * RML_OP_CONCAT operation concatenates a list of tensors along the specified dimension.
 * All tensors MUST have the same shape and type, except for the dimension size of the axis to concatenate on.
 */
typedef struct _rml_op_concat_params
{
    rml_op* inputs;          /**< A list of operations that output tensors containing input data. */
    size_t num_inputs;       /**< A number of input operations. */
    rml_op axis;             /**< The axis to stack along. A negative value means counting dimensions from back. 
                                  Accepted range is [-R, R-1] where R = rank(inputs[i]). */
} rml_op_concat_params;

/**
 * @brief Parameters for RML_OP_STACK operation. 
 * 
 * RML_OP_STACK operation Stacks a list of tensors with rank R 
   into single tensor with rank R+1. All tensors MUST have the same shape and type.
 */
typedef struct _rml_op_stack_params
{
    rml_op* inputs;    /**< A list of operations that output tensors containing input data. */
    size_t num_inputs; /**< A number of input operations. */
    int axis;          /**< The axis to stack along. A negative value means counting dimensions from back. 
                            Accepted range is [-R, R-1] where R = rank(inputs[i]). */
} rml_op_stack_params;

/**
 *
 * Params for squeeze operation
 */
typedef struct _rml_op_squeeze_params
{
    rml_op input;
    rml_layout axes_layout;
    const int32_t* axes;
    size_t num_axes;

} rml_op_squeeze_params;

/**
 * Semantic is the same as for the start:stop:step notation in Numpy.
 * Additionally, "axes" specifies axes to apply slice
 * https://docs.scipy.org/doc/numpy/reference/arrays.indexing.html
 */
typedef struct _rml_op_slice_params
{
    rml_op input;
    rml_op axes;
    rml_op starts;
    rml_op ends;
    rml_op strides;
    rml_layout layout;

} rml_op_slice_params;

/**
 * Params for unsqueeze operation
 this operation inserts a dimension of size 1 at the dimension index axis of input's shape. 
 */
typedef struct _rml_op_unsqueeze_params
{
    rml_op input; 
    rml_op axis;
    rml_layout axes_layout;

} rml_op_unsqueeze_params;

/**
 * Params for Flatten operation
 * Flattens the input. Does not affect the batch size.
 */
typedef struct _rml_op_flatten_params
{
    rml_op input; /**< Operation that outputs tensor containing input */

} rml_op_flatten_params;

/**
 * Params for GEMM operation
 * Y = alpha * transA(A) * transB(B) + beta * C
 */
typedef struct _rml_op_gemm_params
{
    rml_op input_a; /**< Operation that outputs tensor containing input A */
    rml_op input_b; /**< Operation that outputs tensor containing input B */
    rml_op input_c; /**< Operation that outputs tensor containing input C, optional. */

    float alpha;          /**< A scalar multiplier for the product of inputs A and B. */
    float beta;           /**< A scalar multiplier for the optional input C. */
    rml_bool transpose_a; /**< A transform to be applied to the A tensor; either a transpose, or no
                             transform.*/
    rml_bool transpose_b; /**< A transform to be applied to the B tensor; either a transpose, or no
                             transform.*/

} rml_op_gemm_params;

#define RML_OP_GEMM_DEFAULT_ALPHA 1.f
#define RML_OP_GEMM_DEFAULT_BETA 1.f

/**
 * @brief Parameters for RML_OP_LRN operation (Local Response Normalization)
 */
typedef struct _rml_op_lrn
{
    rml_op input; /**< Operation that outputs tensor containing input */
    uint32_t size; /**< the number of channels to sum over (for cross channel LRN) or
                   the side length of the square region to sum over (for within channel LRN) */
    float alpha; /**< Scaling parameter */
    float beta; /**< Exponent parameter */
    float bias; /**< Bias parameter */
    rml_bool cross_channel; /**< whether to sum over adjacent channels (for cross channel LRN) or
                            nearby spatial locations (for within channel LRN) */
} rml_op_lrn;

#define RML_OP_LRN_DEFAULT_ALPHA 0.0001f
#define RML_OP_LRN_DEFAULT_BETA 0.75f
#define RML_OP_LRN_DEFAULT_BIAS 1.0f

typedef struct _rml_op_desc
{
    rml_op_type op_type;
    char const* op_name;

    union
    {
        rml_op_unary_params abs;                         /**< @brief #RML_OP_ABS */
        rml_op_unary_params acos;                        /**< @brief #RML_OP_ACOS */
        rml_op_binary_params add;                        /**< @brief #RML_OP_ADD */
        rml_op_unary_params asin;                        /**< @brief #RML_OP_ASIN */
        rml_op_unary_params atan;                        /**< @brief #RML_OP_ATAN */
        rml_op_batch_norm_params batch_norm;             /**< @brief #RML_OP_BATCH_NORMALIZATION */
        rml_op_bias_add_params bias_add;                 /**< @brief #RML_OP_BIAS_ADD */
        rml_op_cast_params cast;                         /**< @brief #RML_OP_CAST */
        rml_op_unary_params ceil;                        /**< @brief #RML_OP_CEIL */
        rml_op_clip_params clip;                         /**< @brief #RML_OP_CLIP */
        rml_op_concat_params concat;                     /**< @brief #RML_OP_CONCAT */
        rml_op_const_params constant;                    /**< @brief #RML_OP_CONST */
        rml_op_convolution_2d_params conv2d;             /**< @brief #RML_OP_CONV_2D */
        rml_op_convolution_2d_params conv2d_depthwise;   /**< @brief #RML_OP_CONV_2D_DEPTHWISE */
        rml_op_unary_params cos;                         /**< @brief #RML_OP_COS */
        rml_op_depth_to_space_params depth2space;        /**< @brief #RML_OP_DEPTH_TO_SPACE */
        rml_op_binary_params div;                        /**< @brief #RML_OP_DIV */
        rml_op_elu_params elu;                           /**< @brief #RML_OP_ELU */
        rml_op_unary_params exp;                         /**< @brief #RML_OP_EXP */
        rml_op_flatten_params flatten;                   /**< @brief #RML_OP_FLATTEN */
        rml_op_unary_params floor;                       /**< @brief #RML_OP_FLOOR */
        rml_op_gemm_params gemm;                         /**< @brief #RML_OP_GEMM */
        rml_op_unary_params identity;                    /**< @brief #RML_OP_IDENTITY */
        rml_op_leaky_relu_params leaky_relu;             /**< @brief #RML_OP_LEAKY_RELU */
        rml_op_lrn lrn;                                  /**< @brief #RML_OP_LOCAL_RESPONSE_NORMALIZATION */
        rml_op_unary_params log_softmax;                 /**< @brief #RML_OP_LOG_SOFTMAX */
        rml_op_unary_params logn;                        /**< @brief #RML_OP_LOGN */
        rml_op_binary_params max;                        /**< @brief #RML_OP_MAX */
        rml_op_unary_params mean;                        /**< @brief #RML_OP_MEAN */
        rml_op_binary_params min;                        /**< @brief #RML_OP_MIN */
        rml_op_binary_params mul;                        /**< @brief #RML_OP_MUL */
        rml_op_unary_params neg;                         /**< @brief #RML_OP_NEG */
        rml_op_binary_params parametric_relu;            /**< @brief #RML_OP_PARAMETRIC_RELU */
        rml_op_placeholder_params placeholder;           /**< @brief #RML_OP_PLACEHOLDER */
        rml_op_pooling_2d_params pool2d_avg;             /**< @brief #RML_OP_POOL_2D_AVG */
        rml_op_pooling_2d_params pool2d_avg_global;      /**< @brief #RML_OP_POOL_2D_AVG_GLOBAL */
        rml_op_pooling_2d_params pool2d_max;             /**< @brief #RML_OP_POOL_2D_MAX */
        rml_op_pooling_2d_params pool2d_min;             /**< @brief #RML_OP_POOL_2D_MIN */
        rml_op_unary_params pow;                         /**< @brief #RML_OP_POW */
        rml_op_unary_params recip;                       /**< @brief #RML_OP_RECIP */
        rml_op_unary_params relu;                        /**< @brief #RML_OP_RELU */
        rml_op_unary_params relu6;                       /**< @brief #RML_OP_RELU_6 */
        rml_op_reshape_params reshape;                   /**< @brief #RML_OP_RESHAPE */
        rml_op_resize_2d_params resize2d;                /**< @brief #RML_OP_RESIZE_2D */
        rml_op_unary_params rsqrt;                       /**< @brief #RML_OP_RSQRT */
        rml_op_selu_params selu;                         /**< @brief #RML_OP_SELU */
        rml_op_unary_params shape;                       /**< @brief #RML_OP_SHAPE */
        rml_op_unary_params sigmoid;                     /**< @brief #RML_OP_SIGMOID */
        rml_op_unary_params sin;                         /**< @brief #RML_OP_SIN */
        rml_op_slice_params slice;                       /**< @brief #RML_OP_SLICE */
        rml_op_unary_params softmax;                     /**< @brief #RML_OP_SOFTMAX */
        rml_op_unary_params softplux;                    /**< @brief #RML_OP_SOFTPLUS */
        rml_op_unary_params softsign;                    /**< @brief #RML_OP_SOFTSIGN */
        rml_op_space_to_depth_params space2depth;        /**< @brief #RML_OP_SPACE_TO_DEPTH */
        rml_op_unary_params sqrt;                        /**< @brief #RML_OP_SQRT */
        rml_op_squeeze_params squeeze;                   /**< @brief #RML_OP_SQUEEZE */
        rml_op_stack_params stack;                       /**< @brief #RML_OP_STACK */
        rml_op_binary_params sub;                        /**< @brief #RML_OP_SUB */
        rml_op_unary_params tan;                         /**< @brief #RML_OP_TAN */
        rml_op_unary_params tanh;                        /**< @brief #RML_OP_TANH */
        rml_op_thresholded_relu_params thresholded_relu; /**< @brief #RML_OP_THRESHOLDED_RELU */
        rml_op_unsqueeze_params unsqueeze;               /**< @brief #RML_OP_UNSQUEEZE */

        rml_op_binary_params binary;
        rml_op_pooling_2d_params pool2d;
        rml_op_unary_params unary;
    };

} rml_op_desc;

/**
 * Load graph from file.
 *
 * @param[in]  path  Path to a graph in the TF or ONNX formats.
 * @param[out] graph The pointer to a resulting graph.
 *
 * @return A valid graph handle in case of success, NULL otherwise.
 *         To get more details in case of failure, call rmlGetLastError().
 *         The graph should be released with rmlReleaseGraph().
 */
RML_API_ENTRY rml_status rmlLoadGraph(const rml_char* path, rml_graph* graph);

/**
 * Creates empty graph.
 *
 * @return A valid graph handle in case of success, NULL otherwise.
 *         To get more details in case of failure, call rmlGetLastError().
 *         The graph should be released with rmlReleaseGraph().
 */
RML_API_ENTRY rml_status rmlCreateGraph(rml_graph* graph);

/**
 * Returns the number of input nodes (placeholders) in a graph.
 *
 * @param[in]  graph  A valid graph handle.
 * @param[out] num_inputs A pointer to the number of input nodes value.
 *
 * @return RML_OK in case of success, RML_FAIL otherwise.
 *         To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlGetGraphNumInputs(rml_graph graph, size_t* num_inputs);

/**
 * Returns input nodes (placeholders) in the graph.
 *
 * @param[in]  graph  A valid graph handle.
 * @param[in]  num_inputs A Number of input nodes.
 * @param[out] inputs A pointer to an array of inputs.
 *
 * @return RML_OK in case of success, RML_FAIL otherwise.
 *         To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlGetGraphInputNames(rml_graph graph, size_t num_inputs, const char* names[]);

/**
 * Combines supplied graphs to a single graph connecting output and input of the neighbouring graphs.
 * So, graphs in different formats (i.e. TF and ONNX) can be combined into single graph.
 *
 * @param[in] head_graph        A graph to be inserted in tail_graph.
 * @param[in] tail_graph        A graph with a node to be replaced after connection.
 * @param[in] tail_input        A node in tail_graph where head graph will be inserted.
 * @param[out] connected_graph  Result graph containing tail_graph and head_graph.
 *
 * @return RML_OK in case of success, RML_FAIL otherwise.
 *         To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlConnectGraphs(rml_graph head_graph,
                                          rml_graph tail_graph,
                                          const char* tail_input,
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
RML_API_ENTRY rml_status rmlCreateOperation(rml_graph graph, const rml_op_desc* op_desc, rml_op* op);

/**
 * Creates immutable model from supplied graph.
 *
 * @param[in] context A valid context handle.
 * @param[in] graph A valid graph handle.
 *
 * @return A valid model handle in case of success, NULL otherwise.
 *         To get more details in case of failure, call rmlGetLastError().
 */
RML_API_ENTRY rml_status rmlCreateModelFromGraph(rml_context context, rml_graph graph, rml_model* model);

/**
 * Releases a graph created with rmlCreateGraph() or rmlLoadgraph(), invalidates the handle.
 *
 * @param[in] graph A valid graph handle.
 */
RML_API_ENTRY void rmlReleaseGraph(rml_graph graph);

#ifdef __cplusplus
} // extern "C"
#endif
