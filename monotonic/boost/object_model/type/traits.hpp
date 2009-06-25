// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/object_model/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/object_model/

#ifndef BOOST_OBJECT_MODEL_TYPE_TRAITS_HPP
#define BOOST_OBJECT_MODEL_TYPE_TRAITS_HPP

#include <boost/object_model/detail/prefix.hpp>
#include <boost/object_model/type/number.hpp>
#include <boost/preprocessor/stringize.hpp>

BOOST_OM_BEGIN

namespace type
{
	template <class T, size_t N, class S = T>
	struct traits_base
	{
		static number type_number;
		typedef S Storage;
		typedef Storage type;
		typedef Storage *pointer;
		typedef Storage &reference;
		typedef Storage const &const_reference;
	};

}

#define BOOST_OBJECT_MODEL_TRAITS_NUM_STORE_NAME(T,N,S,NAME) \
	BOOST_OM_BEGIN \
	namespace type \
	{ \
		template <> \
		struct traits<T> : traits_base<T,N,S> \
		{ \
			using traits_base<T,N,S>::type_number; \
			static const char *name; \
		}; \
		template <> \
		number traits_base<T,N,S>::type_number; \
		const char * traits<T>::name = NAME; \
	} \
	BOOST_OM_END

#define BOOST_OBJECT_MODEL_TRAITS_NUM_STORE(T,N,S) \
	BOOST_OBJECT_MODEL_TRAITS_NUM_STORE_NAME(T,N,S,BOOST_PP_STRINGIZE(T))

#define BOOST_OBJECT_MODEL_TRAITS_NUM(T,N) \
	BOOST_OBJECT_MODEL_TRAITS_NUM_STORE(T,N,T)

BOOST_OM_END

#include <boost/object_model/detail/prefix.hpp>

#endif // BOOST_OBJECT_MODEL_TYPE_TRAITS_HPP

//EOF
