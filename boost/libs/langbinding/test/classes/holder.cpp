// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/langbinding/classes/holder.hpp>
#include <boost/langbinding/classes/inheritance.hpp>

#include <boost/langbinding/backend/class_instance.hpp>
#include <boost/langbinding/backend/plugin.hpp>

#include <cassert>

using namespace boost::langbinding;

struct dummy_plugin : backend::plugin
{
 private:
    // plugin implementations
    void* call(
        void* function
      , backend::call_xxx_data& signature_constants
      , void* result_storage
      , void* const* arg_storage) const
    {
        return 0;
    }

    backend::override
    find_override(char const* function_name, backend::class_instance const&) const
    {
        backend::override result = {0, 0};
        return result;
    }
    
} dummy;

struct X 
{
    virtual ~X() {}
};

// This class helps to ensure that a Y object doesn't have the same
// address as its X base object
struct force_offset { int x; };

struct Y : force_offset, X {};

// One of these is created by the backend for each class that will be
// wrapped
backend::class_ X_class(dummy);
backend::class_ Y_class(dummy);

int main()
{
    classes::register_dynamic_id<X>();
    classes::register_dynamic_id<Y>();
    
    classes::register_conversion<Y, X>();
    classes::register_conversion<X, Y>();

    Y y;
    X& x = y;

    backend::class_instance X_instance(&X_class, &x);
    backend::class_instance Y_instance(&Y_class, &y);
    
    classes::instance_holder<X*> x_holder(X_instance, &x, 0);
    classes::instance_holder<Y*> y_holder(Y_instance, &y, 0);

    assert(x_holder.find(util::type_id<X>()) == &x);
    assert(x_holder.find(util::type_id<Y>()) == &y);
    assert(y_holder.find(util::type_id<X>()) == &x);
    assert(y_holder.find(util::type_id<Y>()) == &y);
}
