# RadeonML: AMD Inference Library

## 1. C/C++ API


* [RadeonML.h](include/RadeonML.h) - main _C_ API
* [RadeonML.hpp](include/RadeonML.hpp) - _C++_ wrappers
* [RadeonML_cl.h](include/RadeonML_cl.h) - _C_ functions for _OpenCL_ interop
* [RadeonML_cl.hpp](include/RadeonML_d3d12.hpp) - _C++_ wrappers for _OpenCL_ interop
* [RadeonML_d3d12.h](include/RadeonML_d3d12.h) - _C_ functions for _Direct3D 12_ interop
* [RadeonML_d3d12.hpp](include/RadeonML_d3d12.hpp) - _C++_ wrappers for _Direct3D 12_ interop
* [RadeonML_tf.h](include/RadeonML_tf.h) - _C_ functions for _TensorFlow_ interop
* [RadeonML_tf.hpp](include/RadeonML_tf.hpp) - _C++_ wrappers for _TensorFlow_ interop
* [RadeonML_mps.h](include/RadeonML_mps.h) - _C_ functions for _MPS_ interop


### Code examples

* [Loading a model from a file (C)](samples/load_model.c)
* [Loading a model from a file (C++)](samples/load_model.cpp)


### 2.1. System requirements
* Windows 10 19H1 or later (for DirectML backend)
* Ubuntu 18.04
* CentOS/RHEL 7.6, 7.7
* OSX Mojave and Catalina

### 3.1 Features supported
* ONNX support (opset 6-10)
* New operations for DirectML backend added:
    * Acos
    * Asin
    * Atan
    * BatchNorm
    * Cast
    * Ceil
    * Clip
    * Cos
    * Flatten
    * Floor
    * Gemm
    * LocalResponseNormalization
    * LogSoftmax
    * Pool2DAvgGlobal
    * Recip
    * Relu6
    * Rsqrt
    * Selu
    * Selu
    * Sin
    * Softmax
    * SpaceToDepth
    * Sqrt
    * Squeeze
    * Tan
    * Tanh
    * ThresholdedRelu
    * Unsqueeze
* Graph manipulation API
    * rmlLoadGraph
    * rmlConnectGraphs
    * rmlCreateGraph
    * rmlCreateOperation
    * rmlCreateModel
    * rmlReleaseGraph
* Apple MPS backend:
    * Add
    * Concat
    * Exp
    * Div
    * Min
    * Max
    * Conv2D
    * MaxPool2D
    * Relu
    * Reshape
    * Slice
    * Concat
    * Shape
    * Sub
    * Upsample
* Multiple inputs/outputs support

### 3.2 Features supported by OS
* Windows DirectML supports our denoisers, upscalers and common models like resnet, VGG etc..
* Miopen backend for Windows and Linux only supports our denoisers and upscalers
* MPS backend only supports our denoisers

### 3.3 DirectML and Directx12 interop
* A device and command queue can be passed when creating a RML context. We support both async compute queue(D3D12_COMMAND_LIST_TYPE_COMPUTE) and the default command queue(D3D12_COMMAND_LIST_TYPE_DIRECT).
Compute queue are preferred as it will run asynchronously with any graphics job.
If no queue are passed, RML will create a compute queue

### 4 future
We aim at providing the same level of feature for every back end and will provide updates monthly for that

