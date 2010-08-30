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

template<class ObjectT, class CoObjectT> //JODO Only maps of equal iterability shall match!
typename enable_if< mpl::and_< is_domain_compare_equal<ObjectT,CoObjectT>
                             , mpl::and_<is_map<ObjectT>, is_map<CoObjectT> > >, 
                    bool>::type
co_equal(typename ObjectT::const_iterator left_, typename CoObjectT::const_iterator right_,
		 const ObjectT& left, const CoObjectT& right)
{
	return ObjectT::co_value(left_) == CoObjectT::co_value(right_);
}

template<class ObjectT, class CoObjectT> 
typename enable_if< mpl::and_< is_domain_compare_equal<ObjectT,CoObjectT>
                             , mpl::not_<mpl::and_<is_map<ObjectT>, is_map<CoObjectT> > > >,
                  bool>::type
co_equal(typename ObjectT::const_iterator, typename CoObjectT::const_iterator,
		 const ObjectT&, const CoObjectT&)
{
	return true;
}


}} // namespace itl boost

#endif

