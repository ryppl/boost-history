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

#include <boost/call_traits.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/mirror/factory.hpp>
#include <boost/mirror/meta_mem_functions.hpp>


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
typedef typename constructor_utils::template adjust_product< \
	typename meta_function::params:: \
	template param< mpl::int_< INDEX > >:: \
	type::reflected_type \
>::type BOOST_PP_CAT(T, INDEX); \
Manufacturer< BOOST_PP_CAT(T, INDEX) > BOOST_PP_CAT(_, INDEX);

#define BOOST_MIRROR_BASE_FUNCTION_CALLER_DECL_DEFAULT_PARAM(Z, INDEX, X) \
	, typename ::boost::call_traits< BOOST_PP_CAT(T, INDEX) >::param_type \
	BOOST_PP_CAT(opt_default_, INDEX)

#define BOOST_MIRROR_BASE_FUNCTION_CALLER_INITIALIZE_FACTORY(Z, INDEX, PARAM) \
	, BOOST_PP_CAT(_, INDEX) ( \
		PARAM, \
		_meta_functions, \
		FuncIndex(), \
		mpl::int_< INDEX >() \
	)

#define BOOST_MIRROR_BASE_FUNCTION_CALLER_INITIALIZE_FACTORY_W_DEF(Z, INDEX, PARAM) \
	, BOOST_PP_CAT(_, INDEX) ( \
		PARAM, \
		_meta_functions, \
		FuncIndex(), \
		mpl::int_< INDEX >(), \
		& BOOST_PP_CAT(opt_default_, INDEX) \
	)

#define BOOST_MIRROR_BASE_FUNCTION_CALLER_DECLARE_CONSTR_W_DEF(PARAM_COUNT) \
	template <class FactoryParam> \
	inline base_functor_caller( \
		FactoryParam factory_param \
		BOOST_PP_REPEAT( \
			PARAM_COUNT, \
			BOOST_MIRROR_BASE_FUNCTION_CALLER_DECL_DEFAULT_PARAM, \
			_ \
		) \
	): _meta_functions() \
	BOOST_PP_REPEAT( \
		PARAM_COUNT, \
		BOOST_MIRROR_BASE_FUNCTION_CALLER_INITIALIZE_FACTORY_W_DEF, \
		factory_param \
	) { } 

#define BOOST_MIRROR_BASE_FUNCTION_CALLER_CALL_FACTORY(Z, INDEX, X) \
	BOOST_PP_CAT(_, INDEX)()


#define BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER_BEGIN(PARAM_COUNT)\
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
	typedef typename meta_function::result_type::reflected_type result_type;\
 \
	template <class FactoryParam> \
	inline base_functor_caller(FactoryParam factory_param) \
	 : _meta_functions() \
	BOOST_PP_REPEAT( \
		PARAM_COUNT, \
		BOOST_MIRROR_BASE_FUNCTION_CALLER_INITIALIZE_FACTORY, \
		factory_param \
	) { } \

#define BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER_END(PARAM_COUNT)\
	template <class Functor> \
	inline result_type call(Functor func) \
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

#define BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER(PARAM_COUNT) \
BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER_BEGIN(PARAM_COUNT) \
BOOST_MIRROR_BASE_FUNCTION_CALLER_DECLARE_CONSTR_W_DEF(PARAM_COUNT) \
BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER_END(PARAM_COUNT)

#define BOOST_MIRROR_DO_IMPLEMENT_BASE_FUNCTION_CALLER(Z, PARAM_COUNT, X) \
	BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER(PARAM_COUNT)

BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER_BEGIN(0)
BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER_END(0)
BOOST_PP_REPEAT_FROM_TO(
	1, BOOST_MIRROR_MAX_FUNC_PARAMS(), 
	BOOST_MIRROR_DO_IMPLEMENT_BASE_FUNCTION_CALLER, 
	0
)

#undef BOOST_MIRROR_BASE_FUNCTION_CALLER_DECLARE_CONSTR_W_DEF
#undef BOOST_MIRROR_BASE_FUNCTION_CALLER_DECL_DEFAULT_PARAM
#undef BOOST_MIRROR_BASE_FUNCTION_CALLER_INITIALIZE_FACTORY_W_DEF
#undef BOOST_MIRROR_DO_IMPLEMENT_BASE_FUNCTION_CALLER
#undef BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER
#undef BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER_BEGIN
#undef BOOST_MIRROR_IMPLEMENT_BASE_FUNCTION_CALLER_END
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

	typedef typename base_class::result_type result_type;
	typedef typename base_class::meta_function meta_function;

public:
	template <class FactoryParam>
	functor_caller(FactoryParam factory_param)
	 : base_class(factory_param)
	{ }

#ifndef BOOST_NO_VARIADIC_TEMPLATES
	template <class FactoryParam, class ...Defaults>
	functor_caller(FactoryParam factory_param, Defaults ...defs)
	 : base_class(factory_param, defs...)
	{ }
#endif

	inline result_type operator()(void)
	{
		return base_class::call(meta_function::wrap());
	}	

	template <class Class>
	inline result_type operator()(Class& instance)
	{
		return base_class::call(meta_function::wrap(instance));
	}
};

template <
	template <class> class Manufacturer,
	class Class,
	int FuncIndex
> struct member_function_caller : public functor_caller<
	Manufacturer,
	meta_member_functions<Class>,
	mpl::int_<FuncIndex>
> 
{
private:
	typedef functor_caller<
	        Manufacturer,
	        meta_member_functions<Class>,
	        mpl::int_<FuncIndex>
	> base_class;
public:	
	member_function_caller()
	 : base_class(0)
	{ }

	template <class FactoryParam>
	member_function_caller(FactoryParam factory_param)
	 : base_class(factory_param)
	{ }

#ifndef BOOST_NO_VARIADIC_TEMPLATES
	template <class FactoryParam, class ...Defaults>
	member_function_caller(FactoryParam factory_param, Defaults ...defs)
	 : base_class(factory_param, defs...)
	{ }
#endif

};

} // namespace mirror
} // namespace boost

#endif //include guard

