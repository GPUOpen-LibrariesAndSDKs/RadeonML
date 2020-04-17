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
 * @brief Machine learning inference API
 */

#include "rml/RadeonML.h"
#include "rml/RadeonML_graph.h"

#include <algorithm>
#include <cstring>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#define RML_CHECK_STATUS(OP) ::rml::details::CheckStatus(OP == RML_OK, #OP)

namespace rml {

namespace details {

template<class T, class HandleType>
class Wrapper
{
public:
    Wrapper() noexcept = default;

    explicit Wrapper(HandleType handle) noexcept : m_handle(handle)
    {
    }

    Wrapper(Wrapper&& other) noexcept : m_handle(other.m_handle)
    {
        other.m_handle = nullptr;
    }

    T& operator=(HandleType handle) noexcept
    {
        T to_release(std::move(*this)); // 'this' is now empty
        m_handle = handle;
        return static_cast<T&>(*this);
    }

    T& operator=(Wrapper&& rhs) noexcept
    {
        T to_release(std::move(static_cast<T&>(*this))); // 'this' is now empty
        std::swap(m_handle, rhs.m_handle);               // 'rhs' is now empty
        return static_cast<T&>(*this);
    }

    ~Wrapper()
    {
        if (m_handle != nullptr)
        {
            T::ReleaseHandle(m_handle);
        }
    }

    operator HandleType() const
    {
        return m_handle;
    }

    /**
     * Releases the C object handle. The wrapper becomes not reponsible for its release anymore.
     */
    HandleType Release()
    {
        auto handle = m_handle;
        m_handle = nullptr;
        return handle;
    }

protected:
    HandleType m_handle = nullptr;
};

inline void CheckStatus(bool status, const char* op_name)
{
    if (!status)
    {
        std::string func_name(op_name);
        const auto end_pos = func_name.find('(');
        func_name.erase(end_pos != std::string::npos ? end_pos : func_name.size());

        throw std::runtime_error(func_name + " failed: " + rmlGetLastError(nullptr));
    }
}

} // namespace details

class Tensor : public details::Wrapper<Tensor, rml_tensor>
{
public:
    using details::Wrapper<Tensor, rml_tensor>::Wrapper;
    using details::Wrapper<Tensor, rml_tensor>::operator=;

    rml_tensor_info Info() const
    {
        rml_tensor_info info;
        RML_CHECK_STATUS(rmlGetTensorInfo(m_handle, &info));
        return info;
    }

    void* Map(size_t* size = nullptr) const
    {
        void* data = nullptr;
        RML_CHECK_STATUS(rmlMapTensor(m_handle, &data, size));
        return data;
    }

    void Unmap(void* data) const
    {
        RML_CHECK_STATUS(rmlUnmapTensor(m_handle, data));
    }

    template<class T>
    void Write(const T& src) const
    {
        size_t byte_size = 0;
        void* ptr = Map(&byte_size);
        size_t size = byte_size / sizeof(typename T::value_type);
        if (size != src.size())
        {
            Unmap(ptr);
            throw std::runtime_error("Bad source data size: " + std::to_string(src.size()) +
                                     ", expected " + std::to_string(size));
        }
        std::memcpy(ptr, src.data(), size);
        Unmap(ptr);
    }

    template<class T>
    void Read(T& dst) const
    {
        size_t byte_size = 0;
        void* ptr = Map(&byte_size);
        size_t size = byte_size / sizeof(typename T::value_type);
        dst.resize(size); // TODO: Exception safety
        std::memcpy(dst.data(), ptr, size);
        Unmap(ptr);
    }

    static void ReleaseHandle(rml_tensor tensor)
    {
        rmlReleaseTensor(tensor);
    }
};

class Model : public details::Wrapper<Model, rml_model>
{
public:
    using details::Wrapper<Model, rml_model>::Wrapper;
    using details::Wrapper<Model, rml_model>::operator=;

    rml_memory_info GetMemoryInfo() const
    {
        rml_memory_info memory_info;
        RML_CHECK_STATUS(rmlGetModelMemoryInfo(m_handle, &memory_info));
        return memory_info;
    }

    size_t GetNumInputs() const
    {
        size_t num_inputs = 0;
        RML_CHECK_STATUS(rmlGetModelNumInputs(m_handle, &num_inputs));
        return num_inputs;
    }

    template<class T>
    void GetInputNames(T& names) const
    {
        size_t num_inputs = GetNumInputs();
        std::vector<const char*> c_names(num_inputs);
        RML_CHECK_STATUS(rmlGetModelInputNames(m_handle, num_inputs, c_names.data()));
        names = {c_names.begin(), c_names.end()};
    }

    rml_tensor_info GetInputInfo(const char* name = nullptr) const
    {
        rml_tensor_info info;
        RML_CHECK_STATUS(rmlGetModelInputInfo(m_handle, name, &info));
        return info;
    }

    rml_tensor_info GetInputInfo(const std::string& name) const
    {
        return GetInputInfo(name.c_str());
    }

    void SetInputInfo(const char* name, const rml_tensor_info& info) const
    {
        RML_CHECK_STATUS(rmlSetModelInputInfo(m_handle, name, &info));
    }

    void SetInputInfo(const std::string& name, const rml_tensor_info& info) const
    {
        SetInputInfo(name.c_str(), info);
    }

    void SetInputInfo(const rml_tensor_info& info) const
    {
        SetInputInfo(nullptr, info);
    }

    void SetOutputNames(const std::initializer_list<const char*>& names) const
    {
        std::vector<const char*> c_strings(names.begin(), names.end());
        RML_CHECK_STATUS(rmlSetModelOutputNames(m_handle, c_strings.size(), c_strings.data()));
    }

    void SetOutputNames(const std::vector<std::string>& names) const
    {
        std::vector<std::string> strings(names.begin(), names.end());
        std::vector<const char*> c_strings(names.size());
        std::transform(strings.begin(), strings.end(), c_strings.begin(), [](const std::string& s) {
            return s.c_str();
        });
        RML_CHECK_STATUS(rmlSetModelOutputNames(m_handle, c_strings.size(), c_strings.data()));
    }

    rml_tensor_info GetOutputInfo(const char* name = nullptr) const
    {
        rml_tensor_info info;
        RML_CHECK_STATUS(rmlGetModelOutputInfo(m_handle, name, &info));
        return info;
    }

    void SetInput(const char* name, const rml::Tensor& info) const
    {
        RML_CHECK_STATUS(rmlSetModelInput(m_handle, name, info));
    }

    void SetInput(const std::string& name, const rml::Tensor& info) const
    {
        SetInput(name.c_str(), info);
    }

    void SetInput(const rml::Tensor& info) const
    {
        SetInput(nullptr, info);
    }

    void SetOutput(const char* name, const rml::Tensor& info) const
    {
        RML_CHECK_STATUS(rmlSetModelOutput(m_handle, name, info));
    }

    void SetOutput(const std::string& name, const rml::Tensor& info) const
    {
        SetOutput(name.c_str(), info);
    }

    void SetOutput(const rml::Tensor& info) const
    {
        SetOutput(nullptr, info);
    }

    void Prepare() const
    {
        RML_CHECK_STATUS(rmlPrepareModel(m_handle));
    }

    void Infer() const
    {
        RML_CHECK_STATUS(rmlInfer(m_handle));
    }

    static void ReleaseHandle(rml_model model)
    {
        rmlReleaseModel(model);
    }

private:
    mutable std::vector<rml_tensor> m_inputs;
};

class Graph : public details::Wrapper<Graph, rml_graph>
{
public:
    using details::Wrapper<Graph, rml_graph>::Wrapper;
    using details::Wrapper<Graph, rml_graph>::operator=;

    rml_op CreateOperation(const rml_op_desc& desc) const
    {
        rml_op op = NULL;
        RML_CHECK_STATUS(rmlCreateOperation(m_handle, &desc, &op));
        return op;
    }

    size_t GetNumInputs() const
    {
        size_t num_inputs;
        RML_CHECK_STATUS(rmlGetGraphNumInputs(m_handle, &num_inputs));
        return num_inputs;
    }

    void GetInputNames(std::vector<const char*>& names) const
    {
        size_t num_inputs = GetNumInputs(); 
        names.resize(num_inputs);
        GetInputNames(num_inputs, names.data());
    }

    void GetInputNames(size_t num_inputs, const char* names[]) const
    {
        RML_CHECK_STATUS(rmlGetGraphInputNames(m_handle, num_inputs, names));
    }

    static void ReleaseHandle(rml_graph graph)
    {
        rmlReleaseGraph(graph);
    }
};

inline Graph CreateGraph()
{
    rml_graph graph = NULL;
    RML_CHECK_STATUS(rmlCreateGraph(&graph));
    return Graph(graph);
}

inline Graph ConnectGraphs(rml_graph head_graph, rml_graph tail_graph, const char* tail_node)
{
    rml_graph graph = NULL;
    RML_CHECK_STATUS(rmlConnectGraphs(head_graph, tail_graph, tail_node, &graph));
    return Graph(graph);
}

class Context : public details::Wrapper<Context, rml_context>
{
public:
    using details::Wrapper<Context, rml_context>::Wrapper;
    using details::Wrapper<Context, rml_context>::operator=;

    Model LoadModel(const std::basic_string<rml_char>& path) const
    {
        rml_model model = nullptr;
        RML_CHECK_STATUS(rmlLoadModel(m_handle, path.c_str(), &model));
        return Model(model);
    }

    Tensor CreateTensor(const rml_tensor_info& info, rml_access_mode mode) const
    {
        rml_tensor tensor = nullptr;
        RML_CHECK_STATUS(rmlCreateTensor(m_handle, &info, mode, &tensor));
        return Tensor(tensor);
    }

    Graph LoadGraph(const std::basic_string<rml_char>& path) const
    {
        rml_graph graph = nullptr;
        RML_CHECK_STATUS(rmlLoadGraph(path.c_str(), &graph));
        return Graph(graph);
    }

    Model CreateModel(const Graph& graph) const
    {
        rml_model model = NULL;
        RML_CHECK_STATUS(rmlCreateModelFromGraph(m_handle, graph, &model));
        return Model(model);
    }

    static void ReleaseHandle(rml_context context)
    {
        rmlReleaseContext(context);
    }
};

inline Context CreateDefaultContext()
{
    rml_context context = nullptr;
    RML_CHECK_STATUS(rmlCreateDefaultContext(&context));
    return Context(context);
}

} // namespace rml

#undef RML_CHECK_STATUS
