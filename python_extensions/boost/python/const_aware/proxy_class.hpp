// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_CONST_AWARE_PROXY_CLASS_HPP
#define BOOST_PYTHON_CONST_AWARE_PROXY_CLASS_HPP

#include <boost/python.hpp>
#include <boost/python/const_aware/proxy_method.hpp>
#include <boost/python/const_aware/rvalue_from_proxy.hpp>

namespace boost { namespace python { namespace const_aware {

// Mostly copied from objects::class_base in main Boost.Python library.
struct proxy_class_base : python::api::object {

    // constructor
    proxy_class_base(
        std::size_t num_types,         // A list of class_ids. The first is the type
        type_info const * const types  // this is wrapping. The rest are the types of
                                       // any bases.
    );


protected:

    // Retrieve the underlying object
    void setattr(char const* name, object const&);

};

class proxy_class : proxy_class_base {
    
    template <typename W, typename X1, typename X2, typename X3>
    struct target_metadata {
        typedef typename objects::class_metadata<W,X1,X2,X2> metadata;
        typedef typename metadata::bases bases;
        typedef typename metadata::wrapped wrapped;

        target_metadata() {
            // Stick the derived class id into the first element of the array
            ids[0] = detail::unwrap_type_id((W*)0, (W*)0);
            
            // Write the rest of the elements into succeeding positions.
            type_info* p = ids + 1;
            mpl::for_each(detail::write_type_id(&p), (bases*)0, (add_pointer<mpl::_>*)0);
        }
        
        BOOST_STATIC_CONSTANT(
            std::size_t, size = mpl::size<bases>::value + 1);
        type_info ids[size];

    };

public:

    template <typename W, typename X1, typename X2, typename X3>
    proxy_class(class_<W,X1,X2,X3> & target)
        : proxy_class_base(target_metadata<W,X1,X2,X3>::size, target_metadata<W,X1,X2,X3>().ids)
    {
        rvalue_from_proxy<typename target_metadata<W,X1,X2,X3>::wrapped>();
    }

public:

    proxy_class & setattr(char const * name, object const & obj) {
        proxy_class_base::setattr(name, obj);
        return *this;
    }

    proxy_class & use_method(char const * name, object const & func) {
        proxy_class_base::setattr(name, make_proxy_method(func));
        return *this;
    }

    proxy_class & use_property(char const * name, object const & prop) {
        proxy_class_base::setattr(name, make_proxy_property(prop));
        return *this;
    }

    proxy_class & add_property(char const * name, object const & fget, char const * doc=0) {
        proxy_class_base::setattr(name, make_new_proxy_property(fget));
        return *this;
    }

};

}}} // namespace boost::python::const_aware

#endif // !BOOST_PYTHON_CONST_AWARE_PROXY_CLASS_HPP
