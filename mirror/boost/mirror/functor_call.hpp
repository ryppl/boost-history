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
	class MetaFunctions,
	class FuncIndex,
	class ParamCount
> struct base_functor_caller;


#define BOOST_MIRROR_BASE_FUNCTION_CALLER_DECLARE_FACTORY(Z, INDEX, X)\
Manufacturer< \
typename constructor_utils::template adjust_product< \
	typename meta_function::params:: \
	template param< mpl::int_< INDEX > >:: \
	type::reflected_type \
>::type \
> BOOST_PP_CAT(_, INDEX);

#define BOOST_MIRROR_BASE_FUNCTION_CALLER_INITIALIZE_FACTORY(Z, INDEX, PARAM) \
	, BOOST_PP_CAT(_, INDEX) ( \
		PARAM, \
		_meta_functions, \
		FuncIndex(), \
		mpl::int_< INDEX >() \
	)

#define BOOST_MIRROR_BASE_FUNCTION_CALLER_CALL_FACTORY(Z, INDEX, X) \
	BOOST_PP_CAT(_, INDEX)()


#define BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER(PARAM_COUNT) \
template < \
	template <class> class Manufacturer,  \
	class MetaFunctions, \
	class FuncIndex \
> struct base_functor_caller< \
	Manufacturer, \
	MetaFunctions, \
	FuncIndex, \
	mpl::int_<PARAM_COUNT> \
> : constructor_utils \
{ \
protected: \
	typedef typename MetaFunctions:: \
	template function< FuncIndex > meta_function; \
 \
	MetaFunctions _meta_functions; \
	BOOST_PP_REPEAT( \
		PARAM_COUNT, \
		BOOST_MIRROR_BASE_FUNCTION_CALLER_DECLARE_FACTORY, \
		0 \
	) \
 \
	typedef typename meta_function::result_type::reflected_type result;\
 \
	template <class Param> \
	inline base_functor_caller(Param param) \
	 : _meta_functions() \
	BOOST_PP_REPEAT( \
		PARAM_COUNT, \
		BOOST_MIRROR_BASE_FUNCTION_CALLER_INITIALIZE_FACTORY, \
		param \
	) { } \
 \
	template <class Functor> \
	inline result call(Functor func) \
	{ \
		return func( \
			BOOST_PP_ENUM( \
				PARAM_COUNT, \
				BOOST_MIRROR_BASE_FUNCTION_CALLER_CALL_FACTORY, \
				0 \
			) \
		); \
	} \
};

#define BOOST_MIRROR_DO_IMPLEMENT_BASE_FUNCTION_CALLER(Z, PARAM_COUNT, X) \
	BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER(PARAM_COUNT)

BOOST_PP_REPEAT(
	BOOST_MIRROR_MAX_FUNC_PARAMS(), 
	BOOST_MIRROR_DO_IMPLEMENT_BASE_FUNCTION_CALLER, 
	0
)

#undef BOOST_MIRROR_DO_IMPLEMENT_BASE_FUNCTION_CALLER
#undef BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER
#undef BOOST_MIRROR_BASE_FUNCTION_CALLER_CALL_FACTORY
#undef BOOST_MIRROR_BASE_FUNCTION_CALLER_INITIALIZE_FACTORY
#undef BOOST_MIRROR_BASE_FUNCTION_CALLER_DECLARE_FACTORY


} // namespace detail

template <
	template <class> class Manufacturer,
	class MetaFunctions,
	class FunctionIndex
> struct functor_caller : detail::base_functor_caller<
	Manufacturer,
	MetaFunctions,
	FunctionIndex,
	typename MetaFunctions::
	template function<FunctionIndex>::params::size
>
{
private:
	typedef detail::base_functor_caller<
		Manufacturer,
		MetaFunctions,
		FunctionIndex,
		typename MetaFunctions::
		template function<FunctionIndex>::params::size
	> base_class;

	typedef typename base_class::result result;
	typedef typename base_class::meta_function meta_function;

public:
	template <class Param>
	functor_caller(Param param)
	 : base_class(param)
	{ }

	template <class Class>
	inline result operator()(Class& instance)
	{
		return base_class::call(meta_function::wrap(instance));
	}
};

} // namespace mirror
} // namespace boost

#endif //include guard

