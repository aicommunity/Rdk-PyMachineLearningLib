#ifndef RDK_TPythonIntegrationUtilH
#define RDK_TPythonIntegrationUtilH

#include "../../../Rdk/Deploy/Include/rdk.h"
//#ifndef BOOST_PYTHON_STATIC_LIB
//#define BOOST_PYTHON_STATIC_LIB
//#endif
//#include <boost/python.hpp>

//namespace py = boost::python;
#include "TPythonIntegrationInclude.h"

#include <vector>

namespace RDK {
    std::string parse_python_exception();

py::object import(const std::string& module, const std::string& path, py::object& globals);

template<class T> py::list stdvector2pylist(const std::vector<T>& v);

    template<typename T> inline std::vector<T> pyiterable2stdvector(const py::object& iterable);
}

#endif
