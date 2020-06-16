# release note 0.9.6
Features:
* Added `RML_OP_PAD` operation support for DirectML backend.
* Renamed RadeonML-... libraries to RadeonML_...
* Refactored load_model sample to work with most of the supported models.
* Updated documentation on supported models and their usage.
Bug Fixes:
* Fixed error in `RML_OP_SQUEEZE` operation.
* Fixed double context creation.
* Fixed graph_ops sample not working with MIOpen backend.