# release note 0.9.7
## Features :
* Manage multiple outputs
* Add softplus activation in MIopen backend
* add trigonometric operators for MPS and MIOpen backend
* add abs, Ceil, floor for MPS and MIOpen backend
* add batchnorm in Miopen backend
* add fused conv op in MPS
* various performance and log optimization


## Bug Fixes :
* Workaround crash on linux for nvidia with OpenCL sub buffer
* fix slice op reinitialization
* Multiple parsers issues fixed for tensorflow and onnx
* Fix issue with the transpose operator

## know issues :
FP16 tensor casting in onnx is not working properly


