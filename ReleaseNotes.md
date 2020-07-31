# release note 0.9.7
## Features:
* Added `RML_OP_AVG` operation support for MIOpen backend.
* Added `RML_OP_CONST` operation support for ONNX models.
* Added `RML_OP_SIGMOID` operation support for tensorflow and ONNX models.
* Added support of parsing ONNX models in text format (.onnxtxt).
## Bug Fixes:
* Fixed error in binary operations for MIOpen backend.
* Fixed error in `RML_OP_TRANSPOSE` operation in case of 4-D tensor for MIOpen backend.
* Fixed error in `RML_OP_CONV_2D_TRANSPOSE` operation for ONNX models.
* Fixed error of double loading backends.
* Fixed failing macro checks in load_model sample for C API (GPUOpen 3).
* Fixed unsupported tensor attribute type in ONNX models (GPUOpen 6).
* Fixed error in case of mismatch of node name and node output name for ONNX models (GPUOpen 7).

