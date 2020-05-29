# Choose the implementation
if(WIN32)
    list(APPEND RML_LIB_PATH "${PROJECT_SOURCE_DIR}/windows")
    list(APPEND SAMPLES_COMPILE_DEFINITION -DWIN32)
    list(APPEND SAMPLES_COMPILE_DEFINITION -D_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(
        _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
        NOMINMAX
    )
    add_compile_options("/std:c++17")

endif(WIN32)

if(UNIX)
    list(APPEND RML_LIB_PATH "${PROJECT_SOURCE_DIR}/Ubuntu18")
endif(UNIX)

if(APPLE)
    list(APPEND RML_LIB_PATH "${PROJECT_SOURCE_DIR}/OSX")
endif(APPLE)


list(APPEND SAMPLES_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
link_directories(${RML_LIB_PATH}) 
include_directories(${SAMPLES_INCLUDE_DIR})
add_definitions(${SAMPLES_COMPILE_DEFINITION})

function(add_sample TARGET_NAME SOURCE_FILE COMPILER)
    project(${TARGET_NAME} LANGUAGES  ${COMPILER})

    add_executable(${PROJECT_NAME} ${SOURCE_FILE})
		
    target_link_libraries(${PROJECT_NAME} PUBLIC RadeonML)

endfunction(add_sample)

add_sample(load_model_c load_model.c C)

add_sample(load_model_cpp load_model.cpp CXX)

add_sample(graph_ops_c graph_ops.c C)

add_sample(graph_ops_cpp graph_ops.cpp CXX)
