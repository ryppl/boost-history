//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_STATIC_VALUE_HPP)
#define BOOST_SQL_CLI_DETAIL_STATIC_VALUE_HPP

#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>

#include <boost/sql_cli/detail/value_types.hpp>

namespace boost
{
namespace sql_cli
{
namespace detail
{

typedef boost::make_variant_over<value_types>::type value_variant;
typedef boost::make_variant_over<optional_types>::type optional_variant;
typedef boost::make_variant_over<null_types>::type null_variant;

typedef boost::variant<value_variant, optional_variant, null_variant> combined_value_variant;

class static_value
{
public:
	enum type_style { plain_v, optional_v, null_v };

	static_value(int ti, type_style s) : type_idx(ti), style(s) {}

	template <typename FieldT>
	void set_value(FieldT & f)
	{
		value_setter<0, boost::mpl::begin<value_types>::type>()(f, *this, type_idx, style);
	}

	template <typename ValueT>
	operator ValueT() const
	{
		return boost::apply_visitor(value_visitor<ValueT>(), var);
	}

private:
	template <typename ValueT>
	struct value_visitor : public boost::static_visitor<ValueT>
	{
		template <typename VariantT>
		ValueT operator() (VariantT const & v) const
		{
			return boost::get<ValueT>(v);
		}
	};

	template <int IndexV, typename IterT>
	struct value_setter
	{
		template <typename FieldT>
		void operator() (FieldT & f, static_value & sv, int ti, type_style s)
		{
			using namespace boost::mpl;
			if ( ti == IndexV )
			{
				switch(s)
				{
				case plain_v:
					sv.set(f.template get<typename deref<IterT>::type>());
					break;
				case optional_v:
					sv.set(f.template get<boost::optional<typename deref<IterT>::type> >());
					break;
				case null_v:
					sv.set(f.template get<null_value<typename deref<IterT>::type> >());
				}
			}
			else
				value_setter<IndexV+1, typename next<IterT>::type>()(f, sv, ti, s);
		}
	};

	template <int IndexV>
	struct value_setter<IndexV, boost::mpl::end<value_types>::type>
	{
		template <typename FieldT>
		void operator() (FieldT &, static_value &, int, type_style)
		{
			assert(!"Invalid type");
		}
	};

	template <typename ValueT>
	void set(ValueT const & v) 
	{ 
		var = value_variant(v); 
	}

	template <typename ValueT>
	void set(boost::optional<ValueT> const & v)
	{
		var = optional_variant(v);
	}

	template <typename ValueT>
	void set(null_value<ValueT> const & v)
	{
		var = null_variant(v);
	}

	combined_value_variant var;
	int type_idx;
	type_style style;
};

template <typename ValueT>
struct static_value_style
{
	typedef ValueT type;

	static const static_value::type_style value = static_value::plain_v;
};

template <typename ValueT>
struct static_value_style<boost::optional<ValueT> >
{
	typedef ValueT type;

	static const static_value::type_style value = static_value::optional_v;
};

template <typename ValueT>
struct static_value_style<null_value<ValueT> >
{
	typedef ValueT type;

	static const static_value::type_style value = static_value::null_v;
};

template <typename ValueT> 
static_value make_static_value()
{
	using namespace boost::mpl;
	return static_value(distance<begin<value_types>::type, 
		typename find<value_types, typename static_value_style<ValueT>::type>::type>::value,
		static_value_style<ValueT>::value);
}
} // namespace detail
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_DETAIL_STATIC_VALUE_HPP)
