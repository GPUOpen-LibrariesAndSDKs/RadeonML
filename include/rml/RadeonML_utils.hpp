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
#pragma once

/**
 * @file
 * @brief Utilities
 */

#include "rml/RadeonML.h"

#include <iostream>
#include <string>
#include <unordered_map>

namespace rml {

inline size_t GetLayoutNumDims(rml_layout layout)
{
    static const std::unordered_map<rml_layout, size_t> kLayoutToDims = {
        {RML_LAYOUT_UNSPECIFIED, 0},
        {RML_LAYOUT_SCALAR, 0},
        {RML_LAYOUT_C, 1},
        {RML_LAYOUT_HW, 2},
        {RML_LAYOUT_NC, 2},
        {RML_LAYOUT_CHW, 3},
        {RML_LAYOUT_HWC, 3},
        {RML_LAYOUT_NCHW, 4},
        {RML_LAYOUT_NHWC, 4},
        {RML_LAYOUT_HWIO, 4},
        {RML_LAYOUT_OIHW, 4},
        {RML_LAYOUT_NCDHW, 5},
        {RML_LAYOUT_NDHWC, 5},
    };

    auto i = kLayoutToDims.find(layout);

    return i != kLayoutToDims.end() ? i->second : RML_TENSOR_MAX_RANK;
}

} // namespace rml

inline std::ostream& operator<<(std::ostream& lhs, rml_dtype rhs)
{
    switch (rhs)
    {
    case RML_DTYPE_FLOAT32:
        lhs << "float32";
        break;

    case RML_DTYPE_FLOAT16:
        lhs << "float16";
        break;

    case RML_DTYPE_INT32:
        lhs << "int32";
        break;

    default:
        lhs << "unknown dtype " << static_cast<int>(rhs);
        break;
    }

    return lhs;
}

inline std::ostream& operator<<(std::ostream& lhs, rml_layout rhs)
{
    static const std::unordered_map<rml_layout, std::string> kLayoutToString = {
        {RML_LAYOUT_UNSPECIFIED, "Undefined"},
        {RML_LAYOUT_SCALAR, "Scalar"},
        {RML_LAYOUT_C, "C"},
        {RML_LAYOUT_HW, "HW"},
        {RML_LAYOUT_NC, "NC"},
        {RML_LAYOUT_CHW, "CHW"},
        {RML_LAYOUT_HWC, "HWC"},
        {RML_LAYOUT_NHWC, "NHWC"},
        {RML_LAYOUT_NCHW, "NCHW"},
        {RML_LAYOUT_NDHWC, "NDHWC"},
        {RML_LAYOUT_NCDHW, "NCDHW"},
        {RML_LAYOUT_OIHW, "OIHW"},
        {RML_LAYOUT_HWIO, "HWIO"},
    };

    if (auto iter = kLayoutToString.find(rhs); iter != kLayoutToString.end())
    {
        lhs << iter->second;
    }
    else
    {
        lhs << "Unknown(" << static_cast<int>(rhs) << ")";
    }
    return lhs;
}

inline std::ostream& operator<<(std::ostream& lhs, const rml_tensor_info& rhs)
{
    lhs << "(" << rhs.dtype << ", " << rhs.layout << ", [";
    const char* delimiter = "";
    size_t num_dims = rml::GetLayoutNumDims(rhs.layout);
    for (size_t i = 0; i < num_dims; ++i)
    {
        lhs << delimiter;
        if (rhs.shape[i] == RML_DIM_UNSPECIFIED)
        {
            lhs << "?";
        }
        else
        {
            lhs << rhs.shape[i];
        }
        delimiter = ", ";
    }
    lhs << "])";
    return lhs;
}

inline bool operator==(const rml_tensor_info& lhs, const rml_tensor_info& rhs)
{
    return lhs.dtype == rhs.dtype && lhs.layout == rhs.layout &&
           std::equal(lhs.shape, lhs.shape + rml::GetLayoutNumDims(lhs.layout), rhs.shape);
}

inline bool operator!=(const rml_tensor_info& lhs, const rml_tensor_info& rhs)
{
    return !(lhs == rhs);
}
