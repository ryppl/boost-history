#ifndef BOOST_PYTHON_CONST_AWARE_HPP
#define BOOST_PYTHON_CONST_AWARE_HPP

#include <boost/python.hpp>
#include <boost/python/const_aware/exposer.hpp>
#include <boost/python/const_aware/as_const.hpp>
#include <boost/python/const_aware/const_shared_ptr_to_python.hpp>
#include <boost/python/const_aware/shared_ptr_from_proxy.hpp>

namespace boost { namespace python {

template <typename W, typename X1, typename X2, typename X3>
inline const_aware::exposer<W,X1,X2,X3> make_const_aware(
    class_<W,X1,X2,X3> const & class_ref
) {
    return const_aware::exposer<W,X1,X2,X3>(class_ref);
}

}} // namespace boost::python

#endif // !BOOST_PYTHON_CONST_AWARE_HPP
