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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RML_CHECK(STATUS)                      \
    do                                         \
    {                                          \
        if (STATUS != RML_OK)                  \
        {                                      \
            printf("%s\n", rmlGetLastError()); \
            exit(EXIT_FAILURE);                \
        }                                      \
    } while (0)

#define CHECK(STATUS)           \
    do                          \
    {                           \
        if (!(STATUS))          \
        {                       \
            exit(EXIT_FAILURE); \
        }                       \
    } while (0)

#define MAX_INPUTS 1

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
    printf("Input data size: %zu\n", num_read);

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
    printf("Output data size: %zu\n", count_written);

    fclose(file);
}

int main(int argc, char* argv[])
{
    // Create a context
    rml_context context = NULL;
    RML_CHECK(rmlCreateDefaultContext(NULL, &context));

    // Set model parameters
#if defined(_WIN32)
    const rml_char* model_path = L"path/model";
#else
    const rml_char* model_path = "path/model";
#endif

    // Set input file
    const char* input_file = "path/input";

    // Set output file
    const char* output_file = "path/output";

    // Load model
    rml_graph graph = NULL;
    RML_CHECK(rmlLoadGraphFromFile(model_path, &graph));
    rml_model model = NULL;
    RML_CHECK(rmlCreateModelFromGraph(context, graph, &model));

    // Get initial input tensor information
    rml_strings input_names;
    RML_CHECK(rmlGetGraphInputNames(graph, &input_names));
    CHECK(input_names.num_items == MAX_INPUTS);

    rml_tensor_info input_info;
    RML_CHECK(rmlGetModelInputInfo(model, input_names.items[0], &input_info));
    CHECK(input_info.layout == RML_LAYOUT_NHWC || input_info.layout == RML_LAYOUT_NCHW);

    // Set unspecified input tensor dimensions if required
    if (input_info.layout == RML_LAYOUT_NHWC)
    {
        input_info.shape[0] = 1;
        input_info.shape[1] = 600;
        input_info.shape[2] = 800;
    }
    else if (input_info.layout == RML_LAYOUT_NCHW)
    {
        input_info.shape[0] = 1;
        input_info.shape[2] = 600;
        input_info.shape[3] = 800;
    }
    RML_CHECK(rmlSetModelInputInfo(model, input_names.items[0], &input_info));

    // Prepare model for inference
    RML_CHECK(rmlPrepareModel(model));

    // Create input tensor
    rml_tensor input_tensor = NULL;
    RML_CHECK(rmlCreateTensor(context, &input_info, RML_ACCESS_MODE_WRITE_ONLY, &input_tensor));

    // Map tensor data
    size_t data_size = 0;
    void* data = NULL;
    RML_CHECK(rmlMapTensor(input_tensor, &data, &data_size));

    // Copy data
    void* file_data = ReadInput(input_file);
    memcpy(data, file_data, data_size);

    // Unmap tensor data
    free(file_data);
    RML_CHECK(rmlUnmapTensor(input_tensor, data));

    // Set model input
    RML_CHECK(rmlSetModelInput(model, input_names.items[0], input_tensor));

    // Get output tensor information
    rml_tensor_info output_info;
    RML_CHECK(rmlGetModelOutputInfo(model, NULL, &output_info));

    // Create output tensor
    rml_tensor output_tensor = NULL;
    RML_CHECK(rmlCreateTensor(context, &output_info, RML_ACCESS_MODE_READ_ONLY, &output_tensor));

    // Set model output
    RML_CHECK(rmlSetModelOutput(model, NULL, output_tensor));

    // Run inference
    RML_CHECK(rmlInfer(model));

    // Get data from output tensor
    size_t output_size;
    void* output_data = NULL;
    RML_CHECK(rmlMapTensor(output_tensor, &output_data, &output_size));

    // Unmap output data
    RML_CHECK(rmlUnmapTensor(output_tensor, &output_data));

    // Write the output
    WriteOutput(output_file, output_data, output_size);

    // Release the input and output tensors
    rmlReleaseTensor(input_tensor);
    rmlReleaseTensor(output_tensor);

    // Release the model
    rmlReleaseModel(model);

    // Release the context
    rmlReleaseContext(context);
}
