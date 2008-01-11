//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_STATIC_PARAM_HPP)
#define BOOST_SQL_CLI_DETAIL_STATIC_PARAM_HPP

#include <boost/type_traits/add_pointer.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/variant.hpp>

#include <boost/sql_cli/detail/value_types.hpp>

namespace boost
{
namespace sql_cli
{
namespace detail
{

typedef boost::mpl::transform<value_types, boost::add_pointer<boost::mpl::_1> >::type pointer_value_types;    
typedef boost::mpl::transform<optional_types, boost::add_pointer<boost::mpl::_1> >::type pointer_optional_types;    
typedef boost::mpl::transform<null_types, boost::add_pointer<boost::mpl::_1> >::type pointer_null_types;    

typedef boost::make_variant_over<pointer_value_types>::type pointer_value_variant;
typedef boost::make_variant_over<pointer_optional_types>::type pointer_optional_variant;
typedef boost::make_variant_over<pointer_null_types>::type pointer_null_variant;

typedef boost::variant<pointer_value_variant, pointer_optional_variant, pointer_null_variant> combined_pointer_variant;

class static_param
{
public:
	template <typename ParamT>
	static_param(ParamT & p) : v(pointer_value_variant(&p)) {}

	template <typename ParamT>
	static_param(boost::optional<ParamT> & p) : v(pointer_optional_variant(&p)) {}

	template <typename ParamT>
	static_param(null_value<ParamT> & p) : v(pointer_null_variant(&p)) {}

	template <typename ParamBinderT>
	void bind_param(int n, ParamBinderT & pb)
	{
		variant_visitor<ParamBinderT> vv(n, pb);
		boost::apply_visitor(vv, v);
	}

private:
	template <typename ParamBinderT>
	struct variant_visitor : public boost::static_visitor<>
	{
		template <typename VariantT>
		struct binder_visitor : public boost::static_visitor<>
		{
			binder_visitor(int n, ParamBinderT * pb) : num(n), binder(pb) {}

			template <typename ElementT>
			void operator() (ElementT e)
			{
				binder->bind_param(num, *e);
			}

			int num;
			ParamBinderT * binder;
		};

		variant_visitor(int n, ParamBinderT  & pb) : num(n), binder(&pb) {}

		template <typename VariantT>
		void operator() (VariantT & v)
		{
			binder_visitor<VariantT> bv(num, binder);
			boost::apply_visitor(bv, v);
		}

		int num;
		ParamBinderT * binder;
	};

	combined_pointer_variant v;
};

} // namespace detail
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_DETAIL_STATIC_PARAM_HPP)
