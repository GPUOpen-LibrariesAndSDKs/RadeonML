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
#include "rml/RadeonML.hpp"
#include "rml/RadeonML_utils.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/*
 * Create operation that holds information aboud input data
 *
 * @param graph - graph where operation is created
 * @param name - unique operation name
 * @param shape - shape of input tensor
 * @return - created placeholder operation
 */
rml_op CreatePlaceholderOp(rml::Graph& graph,
                           const std::string& name,
                           const std::vector<uint32_t>& shape)
{
    // Create placeholder operation
    rml_op_desc input_desc = {RML_OP_PLACEHOLDER, name.c_str()};
    input_desc.placeholder = {
        RML_DTYPE_FLOAT32, RML_LAYOUT_NHWC, {shape[0], shape[1], shape[2], shape[3]}};
    return graph.CreateOperation(input_desc);
}

/*
 * Create operation that stores scalar data
 *
 * @param graph - graph where operation is created
 * @param name - unique operation name
 * @param dtype - tensor data type
 * @param value - scalar value
 * @return - created scalar operation
 */
template<typename T>
rml_op CreateScalarOp(rml::Graph& graph, const std::string& name, rml_dtype dtype, T value)
{
    // Create constant operation
    rml_op_desc op_desc = {RML_OP_CONST, name.c_str()};
    op_desc.constant = {{dtype, RML_LAYOUT_SCALAR}, &value};
    return graph.CreateOperation(op_desc);
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
rml_op CreateUnaryOp(rml::Graph& graph, const std::string& name, rml_op_type op_type, rml_op input)
{
    // Create unary operation
    rml_op_desc op_desc = {op_type, name.c_str()};
    op_desc.unary = {input};
    return graph.CreateOperation(op_desc);
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
rml_op CreateBinaryOp(rml::Graph& graph,
                      const std::string& name,
                      rml_op_type op_type,
                      rml_op input1,
                      rml_op input2)
{
    // Create binary operation
    rml_op_desc op_desc = {op_type, name.c_str()};
    op_desc.binary = {input1, input2};
    return graph.CreateOperation(op_desc);
}

/*
 * Create preprocessing graph and connect it with base graph
 *
 * @param graph - base graph to be connected with preprocesssing graph
 * @param input_names - list of unique input names of preprocessing graph
 * @param input_shapes - list of input shapes of preprocessing graph
 * @return - connected graph
 */
rml::Graph ConnectPreprocessingGraph(const rml::Graph& graph,
                                     const std::vector<std::string>& input_names,
                                     const std::vector<std::vector<uint32_t>>& input_shapes)
{
    // Preprocessing graph includes exponential tone-mapping
    // ldr_color = beta - exp(alpha * hdr_color)
    // alpha = -1.0
    // beta = 1.0
    auto preprocess_graph = rml::CreateGraph();

    // Create placeholder per input
    rml_op color_op = CreatePlaceholderOp(preprocess_graph, input_names[0], input_shapes[0]);
    rml_op albedo_op = CreatePlaceholderOp(preprocess_graph, input_names[1], input_shapes[1]);
    rml_op depth_op = CreatePlaceholderOp(preprocess_graph, input_names[2], input_shapes[2]);
    rml_op normal_op = CreatePlaceholderOp(preprocess_graph, input_names[3], input_shapes[3]);

    // Create alpha
    rml_op alpha_op = CreateScalarOp<float>(preprocess_graph, "alpha", RML_DTYPE_FLOAT32, -1.0f);

    // Create beta
    rml_op beta_op = CreateScalarOp<float>(preprocess_graph, "beta", RML_DTYPE_FLOAT32, 1.0f);

    // Create mul, exp and sub opearions
    rml_op mul_op = CreateBinaryOp(preprocess_graph, "mul", RML_OP_MUL, alpha_op, color_op);
    rml_op exp_op = CreateUnaryOp(preprocess_graph, "exp", RML_OP_EXP, mul_op);
    rml_op sub_op = CreateBinaryOp(preprocess_graph, "sub", RML_OP_SUB, beta_op, exp_op);

    // Create axis for concatenation
    rml_op axis_op = CreateScalarOp<int32_t>(preprocess_graph, "concat/axis", RML_DTYPE_INT32, -1);

    // Create inputs for concatenation
    std::vector<rml_op> inputs = {sub_op, albedo_op, depth_op, normal_op};

    // Concatenate tone-mapped color with albedo, depth and normal
    rml_op_desc concat_desc = {RML_OP_CONCAT, "concat"};
    concat_desc.concat = {inputs.size(), inputs.data(), axis_op};
    preprocess_graph.CreateOperation(concat_desc);

    // Connect preprocessing graph with base graph
    std::vector<const char*> graph_inputs;
    graph.GetInputNames(graph_inputs);
    return rml::ConnectGraphs(preprocess_graph, graph, graph_inputs.at(0));
}

/*
 * Create postprocessing graph and connect it with base graph
 *
 * @param graph - base graph to be connected with postprocessing graph
 * @param input_name - unique input name of postprocessing graph
 * @param input_shape - input shape of postprocessing graph
 * @return connected graph
 */
rml::Graph ConnectPostprocessingGraph(rml::Graph& graph,
                                      const std::string& input_name,
                                      const std::vector<uint32_t>& input_shape)
{
    // Postprocessing graph includes gamma-correction
    // ldr_color = (clip(ldr_color, 0, 1)) ^ (gamma)
    // gamma = 0.4
    auto postprocess_graph = rml::CreateGraph();

    // Create placeholder for color
    rml_op input_op = CreatePlaceholderOp(postprocess_graph, input_name, input_shape);

    // Clip color
    rml_op_desc clip_desc = {RML_OP_CLIP, "clip"};
    clip_desc.clip = {input_op, 0.f, 1.f};
    rml_op clip_op = postprocess_graph.CreateOperation(clip_desc);

    // Create gamma
    rml_op gamma_op = CreateScalarOp<float>(postprocess_graph, "gamma", RML_DTYPE_FLOAT32, 0.4f);

    // Create pow operation
    CreateBinaryOp(postprocess_graph, "pow", RML_OP_POW, clip_op, gamma_op);

    // Connect base graph with postprocessing graph
    std::vector<const char*> graph_inputs;
    postprocess_graph.GetInputNames(graph_inputs);
    return rml::ConnectGraphs(graph, postprocess_graph, graph_inputs.at(0));
}

/*
 * Read input from file
 *
 * @param input_file - name of input file
 * @return - string content of file
 */
std::string ReadInput(const std::string& input_file)
{
    std::istream* input_stream;
    std::ifstream input_file_stream;

    if (input_file.empty())
    {
        freopen(nullptr, "rb", stdin);
        input_stream = &std::cin;
        std::cout << "Reading data from stdin...\n";
    }
    else
    {
        input_file_stream.open(input_file, std::ios_base::binary);
        if (input_file_stream.fail())
        {
            throw std::runtime_error(std::string("Error reading ") + input_file);
        }
        input_stream = &input_file_stream;
        std::cout << "Reading data from file: " << input_file << "\n";
    }

    std::ostringstream stream;
    stream << input_stream->rdbuf();

    auto input = stream.str();
    std::cout << "Input data size: " << input.size() << " bytes\n";
    return input;
}

/*
 * Write output to file
 *
 * @param output_file - name of output file
 * @param output - output data
 */
void WriteOutput(const std::string& output_file, const std::string& output)
{
    std::cout << "Output data size: " << output.size() << " bytes\n";

    std::ostream* output_stream;
    std::ofstream output_file_stream;

    if (output_file.empty())
    {
        freopen(nullptr, "wb", stdout);
        output_stream = &std::cout;
        std::cout << "Writing result to stdout\n";
    }
    else
    {
        output_file_stream.open(output_file, std::ios_base::binary);
        if (output_file_stream.fail())
        {
            throw std::runtime_error(std::string("Error writing ") + output_file);
        }
        output_stream = &output_file_stream;
        std::cout << "Writing result to file: " << output_file << "\n";
    }

    output_stream->write(output.data(), output.size());
}

/*
 * This sample demonstrates how ldr-denosier could be converted to hdr-denoiser
 * using tone-mapping as preprocessing and gamma-correction as postprocessing
 */
int main() try
{
    // Set model path
#if defined(_WIN32)
    std::wstring model_path(L"path/model.pb");
#else
    std::string model_path("path/model.pb");
#endif

    // Set input files
    const std::vector<std::string> input_files = {
        "path/color.bin",
        "path/albedo.bin",
        "path/depth.bin",
        "path/normal.bin",
    };

    // Set output file
    const std::string output_file = "path/out.bin";

    // Set input names
    const std::vector<std::string> input_names = {"hdr-color", "albedo", "depth", "normal"};

    // Set input shapes
    const std::vector<std::vector<uint32_t>> input_shapes = {
        {1, 600, 800, 3},
        {1, 600, 800, 3},
        {1, 600, 800, 1},
        {1, 600, 800, 2},
    };

    // Create a context
    // The handles are released automatically upon scope exit
    rml::Context context = rml::CreateDefaultContext();

    // Load model as a mutable graph
    // model input - 9-channel 800x600 image (3-channel hdr-color,
    //                                        3-channel albedo,
    //                                        1-channel depth,
    //                                        2-channel normal)
    // model output - 3-channel 800x600 ldr image
    // The handles are released automatically upon scope exit
    rml::Graph graph =
        context.LoadGraph(std::basic_string<rml_char>(model_path.begin(), model_path.end()));

    // Add preprocessing of base model inputs
    // Before we can use ldr-denoiser for hdr-data, we should adjust hdr-color
    // using tone-mapping and concatenate it with albedo, depth and normal
    graph = ConnectPreprocessingGraph(graph, input_names, input_shapes);

    // Add postprocessing of base model outputs
    // We should also apply gamma-correction for denoised image
    graph = ConnectPostprocessingGraph(graph, "input", input_shapes[0]);

    // Create immutable model from connected graphs
    // The handles are released automatically upon scope exit
    rml::Model model = context.CreateModel(graph);

    // Set up input info
    std::vector<rml_tensor_info> input_infos;
    for (size_t i = 0; i < input_shapes.size(); i++)
    {
        rml_tensor_info input_info = {RML_DTYPE_FLOAT32, RML_LAYOUT_NHWC};
        std::memcpy(input_info.shape,
                    input_shapes[i].data(),
                    std::min(input_shapes[i].size(), RML_TENSOR_MAX_RANK) * sizeof(uint32_t));
        input_infos.push_back(input_info);
        std::cout << "Input" << i << ": " << input_info << std::endl;
        model.SetInputInfo(input_names[i], input_info);
    }

    // Allocate all required memory and prepare model for inference
    model.Prepare();

    // Check memory info
    rml_memory_info memory_info = model.GetMemoryInfo();
    std::cout << "Memory allocated: " << memory_info.gpu_total << std::endl;

    // Create and fill the input tensors
    std::vector<rml::Tensor> inputs;
    // The handles are released automatically upon scope exit
    for (size_t i = 0; i < input_shapes.size(); i++)
    {
        rml::Tensor input;
        input = context.CreateTensor(input_infos[i], RML_ACCESS_MODE_WRITE_ONLY);
        input.Write(ReadInput(input_files[i]));
        inputs.push_back(std::move(input));
    }

    // Set model inputs
    for (size_t i = 0; i < inputs.size(); i++)
    {
        model.SetInput(input_names[i], inputs[i]);
    }

    // Get output tensor information
    auto output_info = model.GetOutputInfo();
    std::cout << "Output: " << output_info << std::endl;

    // Create the output tensor
    // The handles are released automatically upon scope exit
    auto output_tensor = context.CreateTensor(output_info, RML_ACCESS_MODE_READ_ONLY);

    // Set model output
    model.SetOutput(output_tensor);

    // Run the inference
    model.Infer();

    // Get data from output tensor
    size_t output_size;
    void* output_data = output_tensor.Map(&output_size);

    // Unmap output data
    const std::string output(static_cast<char*>(output_data), output_size);
    output_tensor.Unmap(output_data);

    // Write the output
    WriteOutput(output_file, output);
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
