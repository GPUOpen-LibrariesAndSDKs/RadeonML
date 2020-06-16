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
#include "rml/RadeonML.h"
#include "rml/RadeonML_graph.h"

#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RML_CHECK(STATUS)                          \
    do                                             \
    {                                              \
        if (STATUS != RML_OK)                      \
        {                                          \
            printf("%s\n", rmlGetLastError(NULL)); \
            exit(EXIT_FAILURE);                    \
        }                                          \
    } while (0)

#define CHECK(STATUS)           \
    do                          \
    {                           \
        if (!(STATUS))          \
        {                       \
            exit(EXIT_FAILURE); \
        }                       \
    } while (0)

#define NUM_INPUTS 4
#define NHWC_RANK 4

/*
 * Create operation that holds information aboud input data
 *
 * @param graph - graph where operation is created
 * @param name - unique operation name
 * @param shape - shape of input tensor
 * @param row_index - index in array of shape
 * @return - created placeholder operation
 */
rml_op CreatePlaceholderOp(rml_graph graph, const char* name, const uint32_t* shape)
{
    // Create placeholder operation
    rml_op_desc input_desc = {
        RML_OP_PLACEHOLDER, name, .placeholder = {RML_DTYPE_FLOAT32, RML_LAYOUT_NHWC}};
    memcpy(input_desc.placeholder.tensor_info.shape, shape, NHWC_RANK * sizeof(int));
    rml_op op_placeholder = NULL;
    RML_CHECK(rmlCreateOperation(graph, &input_desc, &op_placeholder));
    return op_placeholder;
}

/*
 * Create operation that stores scalar int data
 *
 * @param graph - graph where operation is created
 * @param name - unique operation name
 * @param dtype - tensor data type
 * @param value - scalar value
 * @return - created scalar operation
 */

rml_op CreateScalarOp(rml_graph graph, const char* name, rml_dtype dtype, const void* value)
{
    rml_op_desc op_desc = {RML_OP_CONST, name, .constant = {{dtype, RML_LAYOUT_SCALAR}, value}};
    rml_op op_const = NULL;
    RML_CHECK(rmlCreateOperation(graph, &op_desc, &op_const));
    return op_const;
}
/*
 * Create specilalized unary operation
 *
 * @param graph - graph where operation is created
 * @param name - unique operation name
 * @param op_type - type of unary operation
 * @param input - input operation
 * @return - created unary operation
 */
rml_op CreateUnaryOp(rml_graph graph, const char* name, rml_op_type op_type, rml_op input)
{
    // Create unary operation
    rml_op_desc op_desc = {op_type, name, .unary = {input}};
    rml_op op_unary = NULL;
    RML_CHECK(rmlCreateOperation(graph, &op_desc, &op_unary));
    return op_unary;
}

/*
 * Create specilalized binary operation
 *
 * @param graph - graph where operation is created
 * @param name - unique operation name
 * @param op_type - type of binary operation
 * @param input1 - input1 operation
 * @param input2 - input2 operation
 * @return - created binary operation
 */
rml_op CreateBinaryOp(rml_graph graph,
                      const char* name,
                      rml_op_type op_type,
                      rml_op input1,
                      rml_op input2)
{
    // Create binary operation
    rml_op_desc op_desc = {op_type, name, .binary = {input1, input2}};
    rml_op op_binary = NULL;
    RML_CHECK(rmlCreateOperation(graph, &op_desc, &op_binary));
    return op_binary;
}

/*
 * Create preprocessing graph and connect it with base graph
 *
 * @param graph - base graph to be connected with preprocesssing graph
 * @param input_names - list of unique input names of preprocessing graph
 * @param input_shapes - list of input shapes of preprocessing graph
 * @return - connected graph
 */
rml_graph ConnectPreprocessingGraph(const rml_graph graph,
                                    const char* input_names[],
                                    const rml_tensor_info* input_infos)
{
    // Preprocessing graph includes exponential tone-mapping
    // ldr_color = beta - exp(alpha * hdr_color)
    // alpha = -1.0
    // beta = 1.0
    /* Create a context */
    rml_graph preprocess_graph = NULL;
    RML_CHECK(rmlCreateGraph(&preprocess_graph));

    // Create placeholder per input
    rml_op color_op = CreatePlaceholderOp(preprocess_graph, input_names[0], input_infos[0].shape);
    rml_op albedo_op = CreatePlaceholderOp(preprocess_graph, input_names[1], input_infos[1].shape);
    rml_op depth_op = CreatePlaceholderOp(preprocess_graph, input_names[2], input_infos[2].shape);
    rml_op normal_op = CreatePlaceholderOp(preprocess_graph, input_names[3], input_infos[3].shape);

    // Create alpha
    float alpha_const = -1.0f;
    rml_op alpha_op = CreateScalarOp(preprocess_graph, "alpha", RML_DTYPE_FLOAT32, &alpha_const);

    // Create beta
    float beta_const = 1.0f;
    rml_op beta_op = CreateScalarOp(preprocess_graph, "beta", RML_DTYPE_FLOAT32, &beta_const);

    // Create mul, exp and sub opearions
    rml_op mul_op = CreateBinaryOp(preprocess_graph, "mul", RML_OP_MUL, alpha_op, color_op);
    rml_op exp_op = CreateUnaryOp(preprocess_graph, "exp", RML_OP_EXP, mul_op);
    rml_op sub_op = CreateBinaryOp(preprocess_graph, "sub", RML_OP_SUB, beta_op, exp_op);

    // Create axis for concatenation
    int axis = -1;
    rml_op axis_op = CreateScalarOp(preprocess_graph, "concat/axis", RML_DTYPE_INT32, &axis);

    // Create inputs for concatenation
    rml_op inputs[NUM_INPUTS] = {sub_op, albedo_op, depth_op, normal_op};
    // Concatenate tone-mapped color with albedo, depth and normal
    rml_op_desc concat_desc = {RML_OP_CONCAT, "concat", .concat = {NUM_INPUTS, inputs, axis_op}};

    rml_op op_concat = NULL;
    RML_CHECK(rmlCreateOperation(preprocess_graph, &concat_desc, &op_concat));

    // Connect preprocessing graph with base graph
    const char* graph_inputs[1];
    RML_CHECK(rmlGetGraphInputNames(graph, 1, graph_inputs));
    rml_graph connected_graph = NULL;
    RML_CHECK(rmlConnectGraphs(preprocess_graph, graph, graph_inputs[0], &connected_graph));

    return connected_graph;
}

/*
 * Create postprocessing graph and connect it with base graph
 *
 * @param graph - base graph to be connected with postprocessing graph
 * @param input_name - unique input name of postprocessing graph
 * @param input_shape - input shape of postprocessing graph
 * @return connected graph
 */
rml_graph ConnectPostprocessingGraph(rml_graph graph,
                                     const char* input_name,
                                     const uint32_t* input_shape)
{
    // Postprocessing graph includes gamma-correction
    // ldr_color = (clip(ldr_color, 0, 1)) ^ (gamma)
    // gamma = 0.4
    rml_graph postprocess_graph = NULL;
    RML_CHECK(rmlCreateGraph(&postprocess_graph));

    // Create placeholder for color
    rml_op input_op = CreatePlaceholderOp(postprocess_graph, input_name, input_shape);

    // Clip color
    rml_op_desc clip_desc = {RML_OP_CLIP, "clip", .clip = {input_op, 0.f, 1.f}};

    rml_op clip_op = NULL;
    RML_CHECK(rmlCreateOperation(postprocess_graph, &clip_desc, &clip_op));

    // Create gamma
    float gamma_const = 0.4f;
    rml_op gamma_op = CreateScalarOp(postprocess_graph, "gamma", RML_DTYPE_FLOAT32, &gamma_const);

    // Create pow operation
    rml_op pow_op = CreateBinaryOp(postprocess_graph, "pow", RML_OP_POW, clip_op, gamma_op);

    // Connect base graph with postprocessing graph
    const char* graph_inputs[1];
    RML_CHECK(rmlGetGraphInputNames(postprocess_graph, 1, graph_inputs) == RML_OK);
    rml_graph connected_graph = NULL;
    RML_CHECK(rmlConnectGraphs(graph, postprocess_graph, graph_inputs[0], &connected_graph));
    return connected_graph;
}

/*
 * Read input from file
 * Must be free memory after caller of this function
 *
 * @param input_file - name of input file
 * @return - string content of file
 */
void* ReadInput(const char* input_file)
{
    void* buffer;
    FILE* file = fopen(input_file, "rb");
    CHECK(file != NULL);
    printf("Reading data from file: %s\n", input_file);

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    buffer = malloc((length) * sizeof(char));
    CHECK(buffer != NULL);
    size_t num_read = fread(buffer, sizeof(char), length, file);
    CHECK(num_read == length);
    printf("Input data size: %d\n", num_read);

    fclose(file);
    return buffer;
}

/*
 * Write output to file
 *
 * @param output_file - name of the output file
 * @param output - output data
 * @param count - number of element in output
 */
void WriteOutput(const char* output_file, const void* output, const size_t count)
{
    FILE* file = fopen(output_file, "wb");
    CHECK(file != NULL);
    printf("Writing result to file: %s\n", output_file);

    size_t count_written = fwrite(output, sizeof(char), count, file);
    CHECK(count_written == count);
    printf("Output data size: %d\n", count_written);

    fclose(file);
}

/*
 * This sample demonstrates how ldr-denosier could be converted to hdr-denoiser
 * using tone-mapping as preprocessing and gamma-correction as postprocessing
 */
int main()
{
    /* Set model parameters */
#if defined(_WIN32)
    const rml_char* model_path = L"path/model";
#else
    const rml_char* model_path = "path/model";
#endif

    // Set input files
    const char* input_files[] = {
        "path/color",
        "path/albedo",
        "path/depth",
        "path/normal",
    };

    // Set output file
    const char* output_file = "path/output";

    // Set input names
    const char* input_names[] = {"hdr-color", "albedo", "depth", "normal"};

    // Set input shapes
    const rml_tensor_info input_infos[NUM_INPUTS] = {
        {RML_DTYPE_FLOAT32, RML_LAYOUT_NHWC, {1, 600, 800, 3}},
        {RML_DTYPE_FLOAT32, RML_LAYOUT_NHWC, {1, 600, 800, 3}},
        {RML_DTYPE_FLOAT32, RML_LAYOUT_NHWC, {1, 600, 800, 1}},
        {RML_DTYPE_FLOAT32, RML_LAYOUT_NHWC, {1, 600, 800, 2}}};

    // Create a context
    rml_context context = NULL;
    RML_CHECK(rmlCreateDefaultContext(&context));

    // Load model as a mutable graph
    // model input - 9-channel 800x600 image (3-channel ldr-color,
    //                                        3-channel albedo,
    //                                        1-channel depth,
    //                                        2-channel normal)
    // model output - 3-channel 800x600 ldr image
    rml_graph graph = NULL;
    RML_CHECK(rmlLoadGraph(model_path, &graph));

    // Add preprocessing of base model inputs
    // Before we can use ldr-denoiser for hdr-data, we should adjust hdr-color
    // using tone-mapping and concatenate it with albedo, depth and normal
    rml_graph graph_with_preprocess = ConnectPreprocessingGraph(graph, input_names, input_infos);
    rmlReleaseGraph(graph);

    // Add postprocessing of base model outputs
    // We should also apply gamma-correction for denoised image
    rml_graph full_graph =
        ConnectPostprocessingGraph(graph_with_preprocess, "input", input_infos[0].shape);
    rmlReleaseGraph(graph_with_preprocess);

    // Create immutable model from connected graphs
    rml_model model = NULL;
    RML_CHECK(rmlCreateModelFromGraph(context, full_graph, &model));

    /* Release the graph */
    rmlReleaseGraph(full_graph);

    // Set up input info
    size_t i;
    for (i = 0; i < NUM_INPUTS; i++)
    {
        RML_CHECK(rmlSetModelInputInfo(model, input_names[i], &input_infos[i]));
    }

    // Allocate all required memory and prepare model for inference
    RML_CHECK(rmlPrepareModel(model));

    // Check memory info
    rml_memory_info memory_info;
    RML_CHECK(rmlGetModelMemoryInfo(model, &memory_info));

    // Create and fill the input tensors
    rml_tensor inputs[NUM_INPUTS];
    for (i = 0; i < NUM_INPUTS; i++)
    {
        rml_tensor input;
        RML_CHECK(rmlCreateTensor(context, &input_infos[i], RML_ACCESS_MODE_WRITE_ONLY, &input));
        size_t data_size = 0;
        void* data = NULL;
        RML_CHECK(rmlMapTensor(input, &data, &data_size));
        void* file_data = ReadInput(input_files[i]);
        memcpy(data, file_data, data_size);
        free(file_data);
        RML_CHECK(rmlUnmapTensor(input, data));
        inputs[i] = input;
    }

    // Set model inputs
    for (i = 0; i < NUM_INPUTS; i++)
    {
        RML_CHECK(rmlSetModelInput(model, input_names[i], inputs[i]));
    }

    // Get output tensor information
    rml_tensor_info output_info;
    RML_CHECK(rmlGetModelOutputInfo(model, NULL, &output_info));

    // Create the output tensor
    rml_tensor output_tensor = NULL;
    RML_CHECK(rmlCreateTensor(context, &output_info, RML_ACCESS_MODE_READ_ONLY, &output_tensor));

    // Set model output
    RML_CHECK(rmlSetModelOutput(model, NULL, output_tensor));

    // Run the inference
    RML_CHECK(rmlInfer(model));

    // Get data from output tensor
    size_t output_size;
    void* output_data = NULL;
    RML_CHECK(rmlMapTensor(output_tensor, &output_data, &output_size));

    // Unmap output data
    RML_CHECK(rmlUnmapTensor(output_tensor, &output_data));

    // Write the output
    WriteOutput(output_file, output_data, output_size);

    /* Release the input and output tensors */
    rmlReleaseTensor(output_tensor);
    for (i = 0; i < NUM_INPUTS; i++)
    {
        rmlReleaseTensor(inputs[i]);
    }

    /* Release the model */
    rmlReleaseModel(model);

    /* Release the context */
    rmlReleaseContext(context);

    return 0;
}
