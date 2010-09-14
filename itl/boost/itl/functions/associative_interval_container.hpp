/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_FUNCTIONS_ASSOCIATIVE_INTERVAL_CONTAINER_HPP_JOFA_100901
#define BOOST_ITL_FUNCTIONS_ASSOCIATIVE_INTERVAL_CONTAINER_HPP_JOFA_100901

#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/type_traits/is_interval_container.hpp>
#include <boost/itl/type_traits/is_continuous.hpp>
#include <boost/itl/detail/interval_set_algo.hpp>

namespace boost{namespace itl
{

//==============================================================================
//= Size
//==============================================================================

template<class Type>
typename enable_if<is_interval_container<Type>, typename Type::size_type>::type
cardinality(const Type& object)
{
    using namespace boost::mpl;
	typedef typename Type::domain_type domain_type;

    return if_<
                bool_<is_continuous<domain_type>::value>,
                continuous_interval_container,
                discrete_interval_container
              >
              ::type::cardinality(object);
}

template<class Type>
typename enable_if<is_interval_container<Type>, typename Type::difference_type>::type
length(const Type& object)
{
	typedef typename Type::difference_type difference_type;
	typedef typename Type::const_iterator  const_iterator;
    difference_type length = neutron<difference_type>::value();
	const_iterator it_ = object.begin();

	while(it_ != object.end())
		length += itl::length(Type::key_value(it_++));
    return length;
}

template<class Type>
typename enable_if<is_interval_container<Type>, std::size_t>::type
interval_count(const Type& object)
{
	return itl::iterative_size(object);
}

//==============================================================================
//= Range
//==============================================================================
template<class ObjectT>
typename enable_if<is_interval_container<ObjectT>, 
                   typename ObjectT::interval_type>::type
hull(const ObjectT& object)
{
    return 
        itl::is_empty(object) ? neutron<typename ObjectT::interval_type>::value()
        : hull((ObjectT::key_value(object.begin())), ObjectT::key_value(object.rbegin()));
}


//==========================================================================
//= Element iterator related
//==========================================================================
//--------------------------------------------------------------------------
//- Forward
//--------------------------------------------------------------------------
template<class Type>
typename enable_if
<mpl::and_< is_interval_container<Type> 
          , mpl::not_<is_continuous_interval<typename Type::interval_type> > >,
typename Type::element_iterator>::type
elements_begin(Type& object)
{
	return typename Type::element_iterator(object.begin());
}

template<class Type>
typename enable_if
<mpl::and_< is_interval_container<Type> 
          , mpl::not_<is_continuous_interval<typename Type::interval_type> > >,
typename Type::element_iterator>::type
elements_end(Type& object)
{ 
    return typename Type::element_iterator(object.end()); 
}

template<class Type>
typename enable_if
<mpl::and_< is_interval_container<Type> 
          , mpl::not_<is_continuous_interval<typename Type::interval_type> > >,
typename Type::element_const_iterator>::type
elements_begin(const Type& object)
{ 
	return typename Type::element_const_iterator(object.begin());
}

template<class Type>
typename enable_if
<mpl::and_< is_interval_container<Type> 
          , mpl::not_<is_continuous_interval<typename Type::interval_type> > >,
typename Type::element_const_iterator>::type
elements_end(const Type& object)
{ 
	return typename Type::element_const_iterator(object.end());
}

//--------------------------------------------------------------------------
//- Reverse
//--------------------------------------------------------------------------
template<class Type>
typename enable_if
<mpl::and_< is_interval_container<Type> 
          , mpl::not_<is_continuous_interval<typename Type::interval_type> > >,
typename Type::element_reverse_iterator>::type
elements_rbegin(Type& object)
{
	return typename Type::element_reverse_iterator(object.rbegin());
}

template<class Type>
typename enable_if
<mpl::and_< is_interval_container<Type> 
          , mpl::not_<is_continuous_interval<typename Type::interval_type> > >,
typename Type::element_reverse_iterator>::type
elements_rend(Type& object)
{ 
	return typename Type::element_reverse_iterator(object.rend());
}

template<class Type>
typename enable_if
<mpl::and_< is_interval_container<Type> 
          , mpl::not_<is_continuous_interval<typename Type::interval_type> > >,
typename Type::element_const_reverse_iterator>::type
elements_rbegin(const Type& object)
{ 
	return typename Type::element_const_reverse_iterator(object.rbegin());
}

template<class Type>
typename enable_if
<mpl::and_< is_interval_container<Type> 
          , mpl::not_<is_continuous_interval<typename Type::interval_type> > >,
typename Type::element_const_reverse_iterator>::type
elements_rend(const Type& object)
{ 
	return typename Type::element_const_reverse_iterator(object.rend());
}

//==============================================================================
//= Equivalences and Orderings
//==============================================================================

template<class Type>
inline typename enable_if<is_interval_container<Type>, bool>::type
operator == (const Type& left, const Type& right)
{
    return Set::lexicographical_equal(left, right);
}


template<class Type>
inline typename enable_if<is_interval_container<Type>, bool>::type
operator < (const Type& left, const Type& right)
{
	typedef typename Type::segment_compare segment_compare;
    return std::lexicographical_compare(
        left.begin(), left.end(), right.begin(), right.end(), 
        segment_compare()
        );
}


}} // namespace itl boost



#endif


