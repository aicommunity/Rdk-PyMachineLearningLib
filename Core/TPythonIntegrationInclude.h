#ifndef TPYTHONINTEGRATION_H
#define TPYTHONINTEGRATION_H

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#ifndef BOOST_PYTHON_STATIC_LIB
#define BOOST_PYTHON_STATIC_LIB
#endif
#include <boost/python.hpp>
#include <ndarrayobject.h>
#ifdef __WIN32__
#include <boost/python/numpy.hpp>
#include <boost/python/numpy/dtype.hpp>
#include <boost/python/numpy/ndarray.hpp>
#else
#include <boost/numpy.hpp>
#include <boost/numpy/dtype.hpp>
#include <boost/numpy/ndarray.hpp>
#endif
#include <boost/python.hpp>

namespace py = boost::python;

#ifdef __WIN32__
namespace np = boost::python::numpy;
#else
namespace np = boost::numpy;
#endif

#endif // TPYTHONINTEGRATION_H
