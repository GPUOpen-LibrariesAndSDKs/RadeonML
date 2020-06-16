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

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

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

int main() try
{
    // Create a context
    rml::Context context = rml::CreateDefaultContext();

    // Set model path
#if defined(_WIN32)
    std::wstring model_path(L"path/model");
#else
    std::string model_path("path/model");
#endif

    // Set input file
    const std::string input_file = "path/input";

    // Set output file
    const std::string output_file = "path/output";

    // Load model
    rml::Model model = context.LoadModel(model_path);

    // Get initial input tensor information
    std::vector<std::string> input_names;
    model.GetInputNames(input_names);
    if (input_names.empty())
    {
        throw std::runtime_error("No model inputs were found");
    }

    // Set unspecified input tensor dimensions if required
    rml_tensor_info input_info = model.GetInputInfo(input_names[0]);
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
    else
    {
        throw std::runtime_error("Only NCHW or NHWC data layout is supported");
    }
    model.SetInputInfo(input_names[0], input_info);

    // Prepare model for inference
    model.Prepare();

    // Create the input tensor
    // The handle is released automatically upon scope exit
    rml::Tensor input_tensor = context.CreateTensor(input_info, RML_ACCESS_MODE_WRITE_ONLY);
    input_tensor.Write(ReadInput(input_file));

    // Set up inputs
    model.SetInput(input_names[0], input_tensor);

    // Get output tensor information
    rml_tensor_info output_info = model.GetOutputInfo();

    // Create the output tensor
    // The handle is released automatically upon scope exit
    rml::Tensor output_tensor = context.CreateTensor(output_info, RML_ACCESS_MODE_READ_ONLY);

    // Set model output
    model.SetOutput(output_tensor);

    // Run inference
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
