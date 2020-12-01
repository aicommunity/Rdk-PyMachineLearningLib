#ifndef RDK_TPythonIntegrationUtilH
#define RDK_TPythonIntegrationUtilH

//#include "../../../Rdk/Deploy/Include/rdk.h"
#include "TPythonIntegrationInclude.h"
#include "pyboostcvconverter.hpp"

#include <vector>

#define Py_BLOCK_GIL      if(!PyGILState_Check()){ gil_state=PyGILState_Ensure(); }
#define Py_UNBLOCK_GIL    if(PyGILState_Check()) { PyGILState_Release(gil_state); }

namespace RDK {
    std::string parse_python_exception();

py::object import(const std::string& module, const std::string& path, py::object& globals);

template<class T> py::list stdvector2pylist(const std::vector<T>& v);

    template<typename T> inline std::vector<T> pyiterable2stdvector(const py::object& iterable);
}

#endif
