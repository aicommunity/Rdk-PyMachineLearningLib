#ifndef TPYTHONINTEGRATION_H
#define TPYTHONINTEGRATION_H

//#define NO_IMPORT_ARRAY
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
#include <boost/numpy.hpp>
#include <boost/numpy/dtype.hpp>
#include <boost/numpy/ndarray.hpp>
#endif
#include <boost/python.hpp>
#include <boost/python/detail/wrap_python.hpp>

namespace py = boost::python;

#ifdef WIN32
namespace np = boost::python::numpy;
#else
namespace np = boost::numpy;
#endif

#endif // TPYTHONINTEGRATION_H
