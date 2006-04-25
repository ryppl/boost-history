// Copyright (C) 2004 Arkadiy Vertleyb
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TYPEOF_TYPE_ENCODING_HPP_INCLUDED
#define BOOST_TYPEOF_TYPE_ENCODING_HPP_INCLUDED

#define BOOST_TYPEOF_REGISTER_TYPE_IMPL(T, Id)                          \
                                                                        \
    template<class V> struct encode_type_impl<V, T >                    \
		: boost::type_of::push_back<V, boost::mpl::size_t<Id> >                   \
    {};                                                                 \
	template<class Iter> struct decode_type_impl<boost::mpl::size_t<Id>, Iter> \
    {                                                                   \
        typedef T type;                                                 \
        typedef Iter iter;                                              \
    };

#define BOOST_TYPEOF_REGISTER_TYPE_EXPLICIT_ID(Type, Id)                \
	namespace { namespace boost_typeof {					\
        BOOST_TYPEOF_REGISTER_TYPE_IMPL(Type, Id)                       \
    }}

#define BOOST_TYPEOF_REGISTER_TYPE(Type)                                \
    BOOST_TYPEOF_REGISTER_TYPE_EXPLICIT_ID(Type, BOOST_TYPEOF_UNIQUE_ID())

#endif//BOOST_TYPEOF_TYPE_ENCODING_HPP_INCLUDED
