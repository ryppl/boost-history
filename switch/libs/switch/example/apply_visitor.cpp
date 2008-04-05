// apply_visitor.cpp
//
// Copyright (c) 2008
// Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

//[apply_visitor_implementation

#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>
#include <boost/assert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/control/switch.hpp>
#include <boost/control/case.hpp>

#include <iostream>

// bring switch_ and case_ into scope.
using namespace boost::control;

// index based function object which wraps the type based
// static visitor, to make it usable by switch_.
template<class Variant, class Visitor>
struct apply_visitor_function {
public:
    apply_visitor_function(Variant& variant, Visitor& visitor)
        : variant_(variant), visitor_(visitor) {}
    template<class N>
    typename Visitor::result_type operator()(N) {
        // N is the index of the type that the variant currently holds.
        // Extract the object of that type and call the user's visitor.
        typedef typename Variant::types types;
        typedef typename boost::mpl::at<types, N>::type contained_type;
        return(visitor_(boost::get<contained_type>(variant_)));
    }
private:
    Variant& variant_;
    Visitor& visitor_;
};

// One of the cases should always be selected.  If the
// the default is executed it must be an error.
template<class R>
struct never_called {
    template<class Int>
    R operator()(Int) {
        BOOST_ASSERT(!"this function should never be called.");
    }
};

// implementation of boost::apply_visitor using switch_.
template<class Visitor, class Variant>
typename Visitor::result_type apply_visitor(Visitor visitor, Variant& variant) {
    typedef typename Visitor::result_type result_type;
    typedef boost::mpl::size<Variant::types> size;
    typedef boost::mpl::range_c<int, 0, size::value> cases;
    apply_visitor_function<Variant, Visitor> f(variant, visitor);
    never_called<typename Visitor::result_type> default_;
    return(switch_<result_type>(variant.which(), case_<cases>(f), default_));
}

//]

//[apply_visitor_usage

struct my_visitor : boost::static_visitor<> {
    void operator()(int) const {
        std::cout << "Got an int!" << std::endl;
    }
    void operator()(double) const {
        std::cout << "Got a double!" << std::endl;
    }
    void operator()(char) const {
        std::cout << "Got a char!" << std::endl;
    }
};

int main() {
    boost::variant<int, char, double> my_variant(1.0);
    ::apply_visitor(my_visitor(), my_variant);
}

//]
