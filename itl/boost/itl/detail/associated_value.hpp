/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_DETAIL_ASSOCIATED_VALUE_HPP_JOFA_100829
#define BOOST_ITL_DETAIL_ASSOCIATED_VALUE_HPP_JOFA_100829

#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/type_traits/is_combinable.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>

namespace boost{namespace itl
{

template<class Type, class CoType> //JODO Only maps of equal iterability shall match!
typename enable_if< mpl::and_< is_domain_compare_equal<Type,CoType>
                             , mpl::and_<is_map<Type>, is_map<CoType> > >, 
                    bool>::type
co_equal(typename Type::const_iterator left_, typename CoType::const_iterator right_,
		 const Type&, const CoType&)
{
	return Type::co_value(left_) == CoType::co_value(right_);
}

template<class Type, class CoType> 
typename enable_if< mpl::and_< is_domain_compare_equal<Type,CoType>
                             , mpl::not_<mpl::and_<is_map<Type>, is_map<CoType> > > >,
                  bool>::type
co_equal(typename Type::const_iterator, typename CoType::const_iterator,
		 const Type&, const CoType&)
{
	return true;
}


}} // namespace itl boost

#endif

