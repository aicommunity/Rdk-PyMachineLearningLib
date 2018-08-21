#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API
#ifndef RDK_TPythonIntegrationUtilCPP
#define RDK_TPythonIntegrationUtilCPP

#ifndef BOOST_PYTHON_STATIC_LIB
#define BOOST_PYTHON_STATIC_LIB
#endif
#include <boost/python.hpp>
#include "boost/python/stl_iterator.hpp"
#include <boost/python/detail/wrap_python.hpp>
#include <vector>
#include <iostream>

namespace py = boost::python;

namespace RDK {
    std::string parse_python_exception() {
        PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
        PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);
        std::string ret("Unfetchable Python error");

    if (type_ptr != NULL) {
        py::handle<> h_type(type_ptr);
        py::str type_pstr(h_type);
        py::extract<std::string> e_type_pstr(type_pstr);

        if (e_type_pstr.check()) {
            ret = e_type_pstr();
        } else {
            ret = "Unknown exception type";
        }
    }

    if (value_ptr != NULL) {
        py::handle<> h_val(value_ptr);
        py::str a(h_val);
        py::extract<std::string> returned(a);

        if (returned.check()) {
            ret += ": " + returned();
        } else {
            ret += std::string(": Unparseable Python error: ");
        }
    }


    if (traceback_ptr != NULL) {
        py::handle<> h_tb(traceback_ptr);
        py::object tb(py::import("traceback"));
        py::object fmt_tb(tb.attr("format_tb"));
        py::object tb_list(fmt_tb(h_tb));
        py::object tb_str(py::str("\n").join(tb_list));
        py::extract<std::string> returned(tb_str);

        if (returned.check()) {
            ret += ": " + returned();
        } else {
            ret += std::string(": Unparseable Python traceback");
        }
    }
    return ret;
}

py::object import(const std::string& module, const std::string& path, py::object& globals) {
    py::dict locals;
    locals["module_name"] = module;
    locals["path"] = path;

    py::exec("import imp\n"
             "new_module = imp.load_module(module_name, open(path), path, ('py', 'U', imp.PY_SOURCE))\n",
             globals,
             locals);
    return locals["new_module"];
}

template<class T>
py::list stdvector2pylist(const std::vector<T>& v)
{
    py::list l;
    typename std::vector<T>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it)
    {
        l.append(*it);
    }
    return l;
}

    template<typename T>
    inline
    std::vector<T> pyiterable2stdvector(const py::object& iterable)
    {
        return std::vector<T>(py::stl_input_iterator<T>(iterable), py::stl_input_iterator<T>());
    }
}

#endif
