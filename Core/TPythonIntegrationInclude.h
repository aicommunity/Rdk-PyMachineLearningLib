#ifndef TPYTHONINTEGRATION_H
#define TPYTHONINTEGRATION_H

//#ifndef PY_ARRAY_UNIQUE_SYMBOL
//#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API
//#endif

#include "../../../Rdk/Deploy/Include/rdk.h"

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#ifndef BOOST_PYTHON_STATIC_LIB
#define BOOST_PYTHON_STATIC_LIB
#endif
#include <boost/python.hpp>
#ifdef WIN32
#include <boost/python/numpy.hpp>
#include <boost/python/numpy/dtype.hpp>
#include <boost/python/numpy/ndarray.hpp>
#else
#include <boost/python/numpy.hpp>
#include <boost/python/numpy/dtype.hpp>
#include <boost/python/numpy/ndarray.hpp>
#endif
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>


namespace py = boost::python;

#ifdef WIN32
namespace np = boost::python::numpy;
#else
namespace np = boost::numpy;
#endif

#if (PY_VERSION_HEX >= 0x03000000)
    extern void *init_py();
#else
    extern void init_py();
#endif



#endif // TPYTHONINTEGRATION_H
