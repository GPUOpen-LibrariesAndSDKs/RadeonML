 cmake_minimum_required(VERSION 3.10)
 project(SamplesRML)

# Choose the implementation
if(WIN32)
    set(RML_DIRECTML ON CACHE BOOL "Use DirectML backend")
endif(WIN32)

if(UNIX)
    set(RML_MIOPEN ON CACHE BOOL "Use MIOpen backend")
endif(UNIX)

if(APPLE)
    set(RML_MPS ON CACHE BOOL "Use Metal Performance Shader (MPS) backend")
endif(APPLE)

add_subdirectory(samples)
