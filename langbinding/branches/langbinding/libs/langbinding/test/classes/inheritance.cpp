// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/langbinding/classes/inheritance.hpp>
#include <boost/langbinding/classes/aux_/inheritance_query.hpp>
#include <boost/langbinding/util/type_id.hpp>

using namespace boost::langbinding;

struct X 
{
    virtual ~X() {}
};

// This class helps to ensure that a Y object doesn't have the same
// address as its X base object
struct force_offset { int x; };

struct Y : force_offset, X {};

int main()
{
    classes::register_dynamic_id<X>();
    classes::register_dynamic_id<Y>();

    classes::register_conversion<Y, X>();
    classes::register_conversion<X, Y>();

    Y y;
    X* x = &y;

    void* p = classes::aux::find_static_type(
        &y, util::type_id<Y>(), util::type_id<X>());
    assert(p == x);

    p = classes::aux::find_dynamic_type(
        x, util::type_id<X>(), util::type_id<Y>());
    assert(p == &y);
}

