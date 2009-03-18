////////////////////////////////////////////////////////////////////////////
// shared_features
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
// Disclaimer :
//
// This code is a minor modification to Boost.Accumulators and serves a
// particular purpose not readily provided by the original library.
// Contact the Boost library for copyright implications.
////////////////////////////////////////////////////////////////////////////

///////////
/ Contact /
///////////

Please send questions or suggestions to erwann.rogard@gmail.com

///////////////
/ Requirements/
///////////////

////////////////
/ Requirements /
////////////////

Compiles fine under

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

The compiler search path must include
boost_1_37_0
sandbox/conditionally_specified_distribution/shared_features
sandbox/conditionally_specified_distribution/utility
////////////
/ Overview /
////////////

This collection of C++ classes may be used to manage objects that depend
on one another through a common class (set). The classes of these
objects are referred to as features.

/////////
/ Usage /
/////////

Assume B depends on A.

* Feature definition
    namespace impl{
        struct B_base{
            // associated types
        };
        struct B : B_base{
            template<typename Args>
            void foo(const Args& args){
                //typically calls
                // args[kwd_set].template extract<feature::A>();
            }
        };
    }
    namespace feature{
        struct B depends_on1<feature::A>{
            typedef impl::B_base traits;
            typedef impl::B impl;
        };
    }

Implemented features include:
./feature/scalar.hpp
./feature/container.hpp
./feature/container_element.hpp
./feature/example.hpp

* Keyword ( See Boost.Parameter )
Above we have extracted the set that A and B are "a part of" using
    args[kwd_set]
where kwd_set is defined in ./parameter.hpp.
Another keyword associated with set is kwd_do_initialize, which we explain
further below.

If B takes other arguments as part of its interface
(constructor, accessors etc.), a specific keyword may be needed :
    class kwd_of_B{
        public:
        static ::boost::parameter::keyword<tag_0>&    arg_0;
        static ::boost::parameter::keyword<tag_1>&    arg_1;
        // etc.
    };

* Construction of a set:
    typedef mpl::vector<feature::B> features;
    typedef set<features> set_t; // includes A due to the dependence
For a technical reason, default construct first, and initialize next:
    set_t set;
    set.initialize(args);
The first command internally passes
    (kwd_set=set)
to the constructor of each feature. The second command overrides the
existing feature with one constructed with
    (kwd_set=set,kwd_do_initialize = true, args)
You may want to take that into account in the definition of the feature.

* Visitation of a set:
Let {e[i] : i=1,...,n} denote elements in the set,
sorted in such a way that each dependees precedes its dependants
for each i=1,...n  do binary_f(e[i],args) is executed with :
    set.visit(binary_f(),args)

Implemented binary functions include:
./functor/initialize.hpp (used by set.initialize)
./functor/sample.hpp
./functor/update.hpp

The last two have no particular use here. They are provided as a
convenience to clients.

* Argument passing
If there are multiple features with an identical type we need to make the
feature and the keyword which it uses depend on an additional type:
namespace impl{
    template<typename Id>
    struct B;
}
    template<typename Id>
    class kwd_of_B;

//////////////
/ Discussion /
//////////////

These are standard recommendations, but it won't hurt to remind them
(it has spared us compiler errors) :
Rather than
    template<typename Args>
    get_x(const Args& args){
    return args[kwd_set].template extract<X>();
    }
Define
    template<typename Args>
    get_x(const Args& args){ this->get_x_impl(args[kwd_set]) }
    template<typename Set>
    get_x_impl(const Set& set){
        return set.template extract<X>();
    }


