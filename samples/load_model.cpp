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
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

int main() try
{
    // Create a context
    rml::Context context = rml::CreateDefaultContext();

    // Set model parameters
#if defined(_WIN32)
    std::wstring model_path(L"model/path.onnx");
#else
    std::string model_path("model/path.onnx");
#endif
    // and possibly set other parameters...

    // Load a model using the parameters
    rml::Model model = context.LoadModel(model_path);

    // Get initial input tensor information
    std::set<std::string> input_names;
    model.GetInputNames(input_names);
    if (input_names != std::set<std::string>{"color", "exposure"})
    {
        throw std::runtime_error("Bad inputs, expected 'color' and 'exposure'");
    }

    rml_tensor_info color_info = model.GetInputInfo("color");
    if (color_info.layout != RML_LAYOUT_NCHW)
    {
        throw std::runtime_error("'color' is not NCHW");
    }

    rml_tensor_info exposure_info = model.GetInputInfo("exposure");
    if (exposure_info.layout != RML_LAYOUT_SCALAR)
    {
        throw std::runtime_error("'exposure' is not a scalar");
    }

    // Set unspecified input tensor dimensions if required
    color_info.shape[2] = 600;
    color_info.shape[3] = 800;
    model.SetInputInfo("color", color_info);

    // Prepare model for inference
    model.Prepare();

    // Create input tensors
    rml::Tensor color = context.CreateTensor(color_info, RML_ACCESS_MODE_WRITE_ONLY);
    rml::Tensor exposure = context.CreateTensor(exposure_info, RML_ACCESS_MODE_WRITE_ONLY);

    // Set up inputs
    model.SetInput("color", color);
    model.SetInput("exposure", exposure);

    // Get output tensor information
    rml_tensor_info output_info = model.GetOutputInfo();

    // Create the output tensor
    rml::Tensor output = context.CreateTensor(output_info, RML_ACCESS_MODE_READ_ONLY);

    // Fill in input tensors
    size_t data_size = 0;
    void* data = color.Map(&data_size);
    std::memset(data, 1, data_size); /* Fill in something useful */
    color.Unmap(data);

    data = exposure.Map(&data_size);
    float exposure_data = 0.2f;
    std::memcpy(data, &exposure_data, data_size);
    exposure.Unmap(data);

    // Run inference
    model.Infer();

    // Get output data
    data = output.Map(&data_size);

    // Use the output data
    output.Unmap(data);
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
