# RadeonML: AMD Inference Library

## 1. C/C++ API


* [RadeonML.h](include/rml/RadeonML.h) - main C API
* [RadeonML.hpp](include/rml/RadeonML.hpp) - main and graph manipulation C++ API
* [RadeonML_cl.h](include/rml/RadeonML_cl.h) - OpenCL interoperation C API
* [RadeonML_cl.hpp](include/rml/RadeonML_cl.hpp) - OpenCL interoperation C++ API
* [RadeonML_d3d12.h](include/rml/RadeonML_d3d12.h) - Direct3D 12 interoperation C API
* [RadeonML_d3d12.hpp](include/rml/RadeonML_d3d12.hpp) - Direct3D 12 interoperation C++ API
* [RadeonML_dml.h](include/rml/RadeonML_dml.h) - DirectML interoperation C API
* [RadeonML_dml.hpp](include/rml/RadeonML_dml.hpp) - DirectML interoperation C++ API
* [RadeonML_graph.h](include/rml/RadeonML_graph.h) - graph manipulation C API
* [RadeonML_miopen.h](include/rml/RadeonML_miopen.h) - MIOpen interoperation C API
* [RadeonML_miopen.hpp](include/rml/RadeonML_miopen.hpp) - MIOpen interoperation C++ API
* [RadeonML_mtl.h](include/rml/RadeonML_mtl.h) - Metal interoperation C API
* [RadeonML_mtl.hpp](include/rml/RadeonML_mtl.hpp) - Metal interoperation C++ API



### Code examples

* [Loading a model from a file (C)](samples/load_model.c)
* [Loading a model from a file (C++)](samples/load_model.cpp)
* [graph operation example (C++)](samples/graph_ops.cpp)
* [graph operation example (C)](samples/graph_ops.c)

## 2.1. System requirements
* Windows 10 19H1 or later (for DirectML backend)
* Windows 7 uses the MIOpen backend
* Ubuntu 18.04
* CentOS/RHEL 7.6, 7.7
* OSX Mojave and Catalina

## 3.1 Features supported
* ONNX support (opset 6-11)
* TensforFlow frozen graph pb files
* FP32 and FP16 for ONNX

    
For more information, see documentation at this link 
https://radeon-pro.github.io/RadeonProRenderDocs/rml/about.html

## 3.2 Features supported by OS
* Windows DirectML supports our denoisers, upscalers and common models like resnet, VGG etc..
* Miopen backend for Windows and Linux only supports our denoisers and upscalers. When creating a RML context if DML is not supported we will fallback automatically to MIOpen
* MPS backend only supports our denoisers

* Model supported by the different backend

|    | DIRECTML | MIOPEN | MPS |
| ------------- | ------------- |------------- |------------- |
| Inception V1 | Yes  | No  | No |
| Inception V2 | Yes  | No  | No |
| Inception V3 | Yes  | No  | No |
| Inception V4 | Yes  | No  | No |
| MobileNet V1 | Yes  | No  | No |
| MobileNet V2 | Yes  | No  | No |
| ResNet V1 50 | Yes  | No  | No |
| ResNet V2 50 | Yes  | No  | No |
| VGG 16 | Yes  | No  | No |
| VGG 19 | Yes  | No  | No |
| UNet(denoiser) | Yes  | Yes  | Yes |
| ESRGAN | Yes  | Yes  | Yes|
| RTUnet | Yes  | Yes  | Yes |

Others models may work as they will have similar operators, but we haven't checked them

## 3.3 DirectML and Directx12 interop
* A device and command queue can be passed when creating a RML context. We support both async compute queue(D3D12_COMMAND_LIST_TYPE_COMPUTE) and the default command queue(D3D12_COMMAND_LIST_TYPE_DIRECT).
Compute queue are preferred as it will run asynchronously with any graphics job.
If no queue are passed, RML will create a compute queue

## 4. Future
We aim at providing the same level of feature for every back end and will provide updates monthly for that

## 5. Building the samples
You will need CMake 3.10 at least to build the sample.

