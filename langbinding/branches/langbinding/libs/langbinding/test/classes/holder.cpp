// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/langbinding/classes/holder.hpp>
#include <boost/langbinding/classes/inheritance.hpp>

#include <boost/langbinding/backend/class_instance.hpp>
#include <boost/langbinding/backend/plugin.hpp>

#include <cassert>

using namespace boost::langbinding;

extern backend::class_ Z_class;

struct dummy_plugin : backend::plugin
{
    dummy_plugin() {}
    
    ~dummy_plugin()
    {
        assert(override_count == 0);
    }
    
 private:
    // plugin implementations
    void* call(
        void* function
      , backend::call_xxx_data& signature_constants
      , void* result_storage
      , void* const* arg_storage) const
    {
        assert(function == &override_count);
        assert(result_storage != 0);
        *(int*)result_storage = 42;
        return result_storage;
    }

    backend::override
    find_override(char const* function_name, backend::class_instance const& instance) const
    {
        // Assume nothing was found
        backend::override result = {0, 0};

        // If the XXX class is Z, we'll assume there's an override
        if (instance.class_ == &Z_class)
        {
            // Since we need _something_, we'll store a pointer to the
            // number of "live" overrides.
            result.data = &override_count;
            result.destroy = &dummy_plugin::destroy_override;

            // Bump the count
			++this->override_count;
        }
        return result;
    }

 private:
    // "destroy" the data associated with an override
    static void destroy_override(void* x)
    {
        assert(x == &override_count);
        unsigned z = --*(unsigned*)x;
        assert(z < 2);
    }

    // A counter for the number of override instances that need to be "destroyed."
    static unsigned override_count;
    
} dummy;

unsigned dummy_plugin::override_count;

struct X 
{
    virtual ~X() {}
    virtual int f(int a) { return a+1; }
};

struct x_wrap : X, classes::polymorphic<X>
{
    int f(int a)
    {
        if (classes::override f = this->find_override("f"))
            return f(a);
        else
            return this->X::f(a);
    }

    int default_f(int a) { return this->X::f(a); }
};

// This class helps to ensure that a Y object doesn't have the same
// address as its X base object
struct force_offset { int x; };

struct Y : force_offset, X
{
    int f(int a)
    {
        return a+2;
    }
};

struct y_wrap : Y, classes::polymorphic<Y>
{
    int f(int a)
    {
        if (classes::override f = this->find_override("f"))
            return f(a);
        else
            return this->Y::f(a);
    }

    int default_f(int a) { return this->Y::f(a); }
};

// One of these is created by the backend for each class that will be
// wrapped
backend::class_ X_class(dummy);
backend::class_ Y_class(dummy);
backend::class_ Z_class(dummy);

int main()
{
    classes::register_dynamic_id<X>();
    classes::register_dynamic_id<Y>();
    classes::register_dynamic_id<x_wrap>();
    classes::register_dynamic_id<y_wrap>();
    
    classes::register_conversion<Y, X>();
    classes::register_conversion<X, Y>();

    classes::register_conversion<Y, y_wrap>();
    classes::register_conversion<y_wrap, Y>();
    
    classes::register_conversion<X, x_wrap>();
    classes::register_conversion<x_wrap, X>();
    
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

    x_wrap xw;
    y_wrap yw;

    backend::class_instance Xw_instance(&X_class, &xw);
    backend::class_instance Yw_instance(&Y_class, &yw);
    backend::class_instance Zw_instance(&Z_class, &yw);
    
    classes::instance_holder<X*> xw_holder(Xw_instance, &xw, 0);
    classes::instance_holder<y_wrap*> zw_holder(Zw_instance, &yw, 0);
    assert(yw.f(33) == 42);
}
