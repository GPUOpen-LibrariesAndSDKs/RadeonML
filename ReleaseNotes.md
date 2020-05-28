# release note 0.9.5
##Features:
* Added extensive working sample of Graph C API usage.
* Added `RML_OP_TRANSPOSE` operation support for MPS backend.
* Added fusion of `RML_OP_MAX` and `RML_OP_MIN` into single `RML_OP_CLIP` operation.
* Added fusion of `RML_OP_GEMM` operation with activations.
* Updated `RML_OP_RESIZE_2D_NEAREST` operation for MPS backend to support ONNX models.
* Method Wrapper::Release() was renamed to Wrapper::Detach() in the C++ API.
##Bug Fixes:
* Fixed memory leak in MPS backend.
* Added support of missing layouts for following operations:
    * `RML_OP_BATCH_NORMALIZATION`
    * `RML_OP_BIAS_ADD`.