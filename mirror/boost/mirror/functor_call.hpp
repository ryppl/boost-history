/**
 * \file boost/mirror/functor_call.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_FUNCTOR_CALL_HPP
#define BOOST_MIRROR_FUNCTOR_CALL_HPP

#include <boost/mirror/factory.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <
        template <class> class Manufacturer,
        class ParamTypeList,
        class ParamCount
> struct base_functor_caller;

#define BOOST_MIRROR_BASE_FUNCTION_CALLER_DECLARE_FACTORY(Z, INDEX, X)\
        factory<Manufacturer, typename mpl::at<ParamTypeList, mpl::int_< INDEX > >::type> \
                BOOST_PP_CAT(_, INDEX);


#define BOOST_MIRROR_BASE_FUNCTION_CALLER_INITIALIZE_FACTORY(Z, INDEX, PARAM) \
        , BOOST_PP_CAT(_, INDEX) (PARAM, INDEX)

#define BOOST_MIRROR_BASE_FUNCTION_CALLER_CALL_FACTORY(Z, INDEX, X) \
        BOOST_PP_CAT(_, INDEX)()


#define BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER(PARAM_COUNT) \
template < \
        template <class> class Manufacturer,  \
        class ParamTypeList \
> struct base_functor_caller<Manufacturer, ParamTypeList, mpl::int_<PARAM_COUNT> > \
{ \
        struct { } _dummy; \
        BOOST_PP_REPEAT( \
                PARAM_COUNT, \
                BOOST_MIRROR_BASE_FUNCTION_CALLER_DECLARE_FACTORY, \
                0 \
        ) \
 \
        template <class Param> \
        base_functor_caller(Param param) \
         : _dummy() \
        BOOST_PP_REPEAT( \
                PARAM_COUNT, \
                BOOST_MIRROR_BASE_FUNCTION_CALLER_INITIALIZE_FACTORY, \
                param \
        ) { } \
 \
        template <class Functor> \
        void operator()(Functor func)  \
        { \
                func( \
                        BOOST_PP_ENUM( \
                                PARAM_COUNT, \
                                BOOST_MIRROR_BASE_FUNCTION_CALLER_CALL_FACTORY, \
                                0 \
                        ) \
                ); \
        } \
        template <class Functor, class Consumer> \
        Consumer& operator()(Functor func, Consumer& consumer)  \
        { \
                consumer(func( \
                        BOOST_PP_ENUM( \
                                PARAM_COUNT, \
                                BOOST_MIRROR_BASE_FUNCTION_CALLER_CALL_FACTORY, \
                                0 \
                        ) \
                )); \
                return consumer; \
        } \
};

#define BOOST_MIRROR_DO_IMPLEMENT_BASE_FUNCTION_CALLER(Z, PARAM_COUNT, X) \
        BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER(PARAM_COUNT)

BOOST_PP_REPEAT(12, BOOST_MIRROR_DO_IMPLEMENT_BASE_FUNCTION_CALLER, 0)

#undef BOOST_MIRROR_DO_IMPLEMENT_BASE_FUNCTION_CALLER
#undef BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER
#undef BOOST_MIRROR_BASE_FUNCTION_CALLER_CALL_FACTORY
#undef BOOST_MIRROR_BASE_FUNCTION_CALLER_INITIALIZE_FACTORY
#undef BOOST_MIRROR_BASE_FUNCTION_CALLER_DECLARE_FACTORY


} // namespace detail

template <
	template <class> class Manufacturer,
	class ParamTypeList
> struct functor_caller
 : detail::base_functor_caller<
	Manufacturer,
	ParamTypeList,
	mpl::int_<
		mpl::size<ParamTypeList>::value
	>
>
{
	typedef  detail::base_functor_caller<
	        Manufacturer,
	        ParamTypeList,
	        mpl::int_<
	                mpl::size<ParamTypeList>::value
	        >
	> base_class;

	template <class Param>
	functor_caller(Param param)
	 : base_class(param)
	{ }
};

} // namespace mirror
} // namespace boost

#endif //include guard

