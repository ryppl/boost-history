/**
 * \file boost/mirror/factory.hpp
 *
 *  Customizable instance construction facility
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_FACTORY_HPP
#define BOOST_MIRROR_FACTORY_HPP

#include <boost/mirror/meta_constructors.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mirror/detail/argument_type_list.hpp>


namespace boost {
namespace mirror {
namespace detail {

/** Forward declaration of the constructor template
 *  class.
 */
template <
        template <class> class Manufacturer,
        class Product,
        class TypeList,
        class ParamCount,
        class Index
> struct constructor;

/** Default constructor (with no argument)
 */
template <
        template <class> class Manufacturer,
        class Product,
        class TypeList,
        class Index
> struct constructor<
        Manufacturer,
        Product,
        TypeList,
        mpl::int_<0>,
        Index
>
{
        template <typename Param>
        constructor(Param param)
        { }

        inline Product operator()(void)
        {
                return Product();
        }
};


#define BOOST_MIRROR_DECLARE_CONSTRUCTOR_PARAM_MANUFACTURER(Z, INDEX, TYPELIST)\
Manufacturer<typename mpl::at<TYPELIST, mpl::int_<INDEX> >::type > _##INDEX;

#define BOOST_MIRROR_INITIALIZE_CONSTRUCTOR_MANUFACTURER(Z, INDEX, PARAM)\
        _##INDEX (PARAM, (Product*)0, ConstructorIndex(), mpl::int_<INDEX>())

#define BOOST_MIRROR_CALL_CONSTRUCTOR_MANUFACTURER(Z, INDEX, X)\
        _##INDEX ()


/** Constructor with arguments
 */
#define BOOST_MIRROR_IMPLEMENT_CONSTRUCTOR(PARAM_COUNT) \
template < \
        template <class> class Manufacturer,  \
        class Product,  \
        class TypeList,  \
        class ConstructorIndex \
> struct constructor< \
        Manufacturer, \
        Product,  \
        TypeList,  \
        mpl::int_< PARAM_COUNT >,  \
        ConstructorIndex \
> \
{ \
        BOOST_PP_REPEAT( \
                PARAM_COUNT,  \
                BOOST_MIRROR_DECLARE_CONSTRUCTOR_PARAM_MANUFACTURER,  \
                TypeList \
        ) \
         \
        template <typename Param> \
        inline constructor(Param param) : \
                BOOST_PP_ENUM( \
                        PARAM_COUNT,  \
                        BOOST_MIRROR_INITIALIZE_CONSTRUCTOR_MANUFACTURER,  \
                        param \
                ) \
        { } \
 \
        inline Product operator()(void) \
        { \
                return Product( \
                        BOOST_PP_ENUM( \
                                PARAM_COUNT,  \
                                BOOST_MIRROR_CALL_CONSTRUCTOR_MANUFACTURER,  \
                                0 \
                        ) \
                ); \
        } \
 \
        inline Product* new_(void) \
        { \
                return new Product( \
                        BOOST_PP_ENUM( \
                                PARAM_COUNT,  \
                                BOOST_MIRROR_CALL_CONSTRUCTOR_MANUFACTURER,  \
                                0 \
                        ) \
                ); \
        } \
};

#define BOOST_MIRROR_DO_IMPLEMENT_CONSTRUCTOR(Z, PARAM_COUNT, X) \
        BOOST_MIRROR_IMPLEMENT_CONSTRUCTOR(PARAM_COUNT)

BOOST_PP_REPEAT_FROM_TO(1, 12, BOOST_MIRROR_DO_IMPLEMENT_CONSTRUCTOR, 0)


#undef BOOST_MIRROR_DO_IMPLEMENT_CONSTRUCTOR
#undef BOOST_MIRROR_IMPLEMENT_CONSTRUCTOR
#undef BOOST_MIRROR_CALL_CONSTRUCTOR_MANUFACTURER
#undef BOOST_MIRROR_INITIALIZE_CONSTRUCTOR_MANUFACTURER
#undef BOOST_MIRROR_DECLARE_CONSTRUCTOR_PARAM_MANUFACTURER


template <
        template <class> class Manufacturer,
        class Product,
        class ConstructorParams,
        class ConstructorCount
> struct base_factory;

/** Declares the I-th factory member constructor
 */
#define BOOST_MIRROR_DECLARE_FACTORY_CONSTRUCTOR(Z, INDEX, CONSTR_LIST) \
constructor< \
        Manufacturer, \
        Product, \
        typename mpl::at< CONSTR_LIST, mpl::int_<INDEX> >::type, \
        mpl::int_< \
                mpl::size< \
                        typename mpl::at< CONSTR_LIST, mpl::int_<INDEX> >::type \
                >::value \
        >, \
        mpl::int_< INDEX > \
> _ ## INDEX;

/** Factory constructor initalizer
 */
#define BOOST_MIRROR_INITIALIZE_FACTORY_CONSTRUCTOR(Z, INDEX, X)\
        , _##INDEX (manager.param())

#define BOOST_MIRROR_CALL_FACTORY_FUNCTOR(Z, INDEX, X)\
        case INDEX: return _##INDEX ();

#define BOOST_MIRROR_CALL_FACTORY_OP_NEW(Z, INDEX, X)\
        case INDEX: return _##INDEX .new_();

#define BOOST_MIRROR_IMPLEMENT_CONSTRUCTION_FUNCTION(MACRO, PARAM_COUNT) \
	assert(manager.index() >= 0); \
        assert(manager.index() < PARAM_COUNT); \
        switch(manager.index()) \
        { \
        	BOOST_PP_REPEAT( \
                	PARAM_COUNT,  \
	                MACRO,  \
                	0 \
                ) \
        } \
        ::std::abort(); \


/** Implements a base factory
 */
#define BOOST_MIRROR_IMPLEMENT_BASE_FACTORY(PARAM_COUNT) \
template < \
        template <class> class Manufacturer, \
        class Product, \
        class ConstructorParams \
> struct base_factory<Manufacturer, Product, ConstructorParams, mpl::int_< PARAM_COUNT> > \
{ \
        Manufacturer<void> manager; \
 \
        BOOST_PP_REPEAT( \
                PARAM_COUNT, \
                BOOST_MIRROR_DECLARE_FACTORY_CONSTRUCTOR, \
                ConstructorParams \
        ) \
 \
        template <typename Param> \
        base_factory(Param param, int factory_index) \
         : manager(param, factory_index) \
                BOOST_PP_REPEAT( \
                        PARAM_COUNT,  \
                        BOOST_MIRROR_INITIALIZE_FACTORY_CONSTRUCTOR,  \
                        0 \
                ) \
        { } \
 \
        inline Product operator()(void) \
        { \
		BOOST_MIRROR_IMPLEMENT_CONSTRUCTION_FUNCTION(\
			BOOST_MIRROR_CALL_FACTORY_FUNCTOR, \
			PARAM_COUNT \
		)\
        	return Product(*((Product*)0)); \
        } \
 \
        inline Product* new_(void) \
        { \
		BOOST_MIRROR_IMPLEMENT_CONSTRUCTION_FUNCTION(\
			BOOST_MIRROR_CALL_FACTORY_OP_NEW, \
			PARAM_COUNT \
		)\
        	return ((Product*)0); \
        } \
};

#define BOOST_MIRROR_DO_IMPLEMENT_BASE_FACTORY(Z, PARAM_COUNT, X) \
        BOOST_MIRROR_IMPLEMENT_BASE_FACTORY(PARAM_COUNT)

BOOST_PP_REPEAT(12, BOOST_MIRROR_DO_IMPLEMENT_BASE_FACTORY, 0)

#undef BOOST_MIRROR_CALL_FACTORY_OP_NEW
#undef BOOST_MIRROR_DO_IMPLEMENT_BASE_FACTORY
#undef BOOST_MIRROR_IMPLEMENT_BASE_FACTORY
#undef BOOST_MIRROR_INITIALIZE_FACTORY_CONSTRUCTOR
#undef BOOST_MIRROR_DECLARE_FACTORY_CONSTRUCTOR
#undef BOOST_MIRROR_CALL_FACTORY_FUNCTOR

template <class TypeList>
struct base_factory_remove_type_list_null_types
{
	typedef typename mpl::remove_if<
		TypeList,
		detail::is_typelist_null_type<
			mpl::_
		>
	>::type type;
};

template <
        template <class> class Manufacturer,
        class Product
> struct make_base_factory
{
	typedef typename meta_constructors<Product>::param_type_lists
		raw_param_type_lists;

	typedef typename mpl::accumulate<
		raw_param_type_lists,
		mpl::vector0<>,
		mpl::push_back<
			mpl::_1,
			base_factory_remove_type_list_null_types<
				mpl::_2
			>
		>
	>::type param_type_lists;

	typedef base_factory<
	        Manufacturer,
	        Product,
	        param_type_lists,
	        mpl::int_< mpl::size< param_type_lists >::value >
	> type;
};

} // namespace detail

/** the factory template
 */
template <
        template <class> class Manufacturer,
        class Product
> struct factory : detail::make_base_factory<Manufacturer, Product>::type
{
        typedef typename detail::make_base_factory<Manufacturer, Product>::type
        	base_class;

	factory(void)
	 : base_class(0, 0)
	{ }

        template <typename Param>
        factory(Param param, int factory_index = 0)
         : base_class(param, factory_index)
        { }
};

template <
        class ConcreteManufacturer,
        class Product
> struct make_factory;

template <
        template <class> class Manufacturer,
        class Product
> struct make_factory<Manufacturer<Product>, Product>
{
        typedef factory<Manufacturer, Product> type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

