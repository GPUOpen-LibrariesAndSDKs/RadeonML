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

#include <stdio.h>
#include <string.h>

#define CHECK(STATUS)                              \
    do                                             \
    {                                              \
        if (!(STATUS))                             \
        {                                          \
            printf("%s\n", rmlGetLastError(NULL)); \
            return 1;                              \
        }                                          \
    } while (0)

#define MAX_INPUTS 5

int main(int argc, char* argv[])
{
    /* Create a context */
    rml_context context = NULL;
    CHECK(rmlCreateDefaultContext(&context));

    /* Set model parameters */
#if defined(_WIN32)
    const rml_char* model_path = L"model/path.onnx";
#else
    const rml_char* model_path = "model/path.onnx";
#endif
    /* and possibly set other parameters... */

    /* Load a model using the parameters */
    rml_model model = NULL;
    CHECK(rmlLoadModel(context, model_path, &model) == RML_OK);

    /* Get initial input tensor information */
    size_t num_inputs = 0;
    CHECK(rmlGetModelNumInputs(model, &num_inputs) == RML_OK);
    CHECK(num_inputs <= MAX_INPUTS);
    CHECK(num_inputs == 2);

    const char* input_names[MAX_INPUTS];
    CHECK(rmlGetModelInputNames(model, num_inputs, input_names) == RML_OK);

    rml_tensor_info color_info;
    CHECK(rmlGetModelInputInfo(model, "color", &color_info) == RML_OK);
    CHECK(color_info.layout == RML_LAYOUT_NCHW);

    rml_tensor_info exposure_info;
    CHECK(rmlGetModelInputInfo(model, "exposure", &exposure_info) == RML_OK);
    CHECK(exposure_info.layout == RML_LAYOUT_SCALAR);

    /* Set unspecified input tensor dimensions if required */
    color_info.shape[2] = 600;
    color_info.shape[3] = 800;
    CHECK(rmlSetModelInputInfo(model, "color", &color_info) == RML_OK);

    /* Prepare model for inference */
    CHECK(rmlPrepareModel(model));

    /* Create input tensors */

    rml_tensor color = NULL;
    CHECK(rmlCreateTensor(context, &color_info, RML_ACCESS_MODE_WRITE_ONLY, &color) == RML_OK);

    rml_tensor exposure = NULL;
    CHECK(rmlCreateTensor(context, &exposure_info, RML_ACCESS_MODE_WRITE_ONLY, &exposure) ==
          RML_OK);

    /* Set up inputs */
    CHECK(rmlSetModelInput(model, "color", color));
    CHECK(rmlSetModelInput(model, "exposure", exposure));

    /* Get output tensor information */
    rml_tensor_info output_info;
    CHECK(rmlGetModelOutputInfo(model, NULL, &output_info) == RML_OK);

    /* Create the output tensor */
    rml_tensor output = NULL;
    CHECK(rmlCreateTensor(context, &output_info, RML_ACCESS_MODE_READ_ONLY, &output) == RML_OK);

    /* Set up output */
    CHECK(rmlSetModelOutput(model, NULL, output) == RML_OK);

    /* Fill in input tensors */

    size_t data_size = 0;
    void* data = NULL;
    CHECK(rmlMapTensor(color, &data, &data_size) == RML_OK);
    memset(data, 1, data_size); /* Fill in something useful */
    CHECK(rmlUnmapTensor(color, data));

    CHECK(rmlMapTensor(exposure, &data, &data_size) == RML_OK);
    float exposure_data = 0.2f;
    memcpy(data, &exposure_data, data_size);
    CHECK(rmlUnmapTensor(exposure, data) == RML_OK);

    /* Run inference */
    CHECK(rmlInfer(model) == RML_OK);

    /* Get output data */
    CHECK(rmlMapTensor(output, &data, &data_size) == RML_OK);

    /* Use the output data */

    CHECK(rmlUnmapTensor(output, data) == RML_OK);

    /* Release the input and output tensors */
    rmlReleaseTensor(color);
    rmlReleaseTensor(exposure);
    rmlReleaseTensor(output);

    /* Release the model */
    rmlReleaseModel(model);

    /* Release the context */
    rmlReleaseContext(context);
}
