// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/langbinding/classes/inheritance.hpp>
#include <boost/langbinding/util/type_id.hpp>

using namespace boost::langbinding;

struct X 
{
    virtual ~X() {}
};

struct Y : X {};

int main()
{
    classes::register_conversion<Y, X>();

    Y y;

    void* p = classes::find_dynamic_type(
        (X*)&y, util::type_id<X>(), util::type_id<Y>());

    assert(p == &y);
}

