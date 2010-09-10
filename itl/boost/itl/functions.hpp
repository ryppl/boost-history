/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_FUNCTIONS_HPP_JOFA_090803
#define BOOST_ITL_FUNCTIONS_HPP_JOFA_090803

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/itl/type_traits/domain_type_of.hpp>
#include <boost/itl/type_traits/interval_type_of.hpp>
#include <boost/itl/type_traits/element_type_of.hpp>
#include <boost/itl/type_traits/segment_type_of.hpp>
#include <boost/itl/type_traits/is_interval_container.hpp>
#include <boost/itl/type_traits/is_interval_separator.hpp>
#include <boost/itl/type_traits/is_interval_splitter.hpp>
#include <boost/itl/type_traits/is_interval_joiner.hpp>
#include <boost/itl/type_traits/is_element_container.hpp>
#include <boost/itl/type_traits/is_combinable.hpp>
#include <boost/itl/type_traits/is_fragment_type_of.hpp>
#include <boost/itl/detail/interval_map_functors.hpp>
#include <boost/itl/detail/interval_map_algo.hpp>

#include <boost/itl/functions/container.hpp>

namespace boost{namespace itl
{

//JODO Forward 4 gcc-3.4.4 -----------------------------------------------------
template<class Type>
inline typename enable_if<is_interval_map<Type>, typename Type::segment_type>::type
make_segment(const typename Type::element_type&);
//JODO Forward 4 gcc-3.4.4 -----------------------------------------------------

//==============================================================================
//= Containedness
//==============================================================================
//------------------------------------------------------------------------------
//- contains
//------------------------------------------------------------------------------

template<class Type, class OperandT>
typename enable_if<has_same_concept<is_interval_map, Type, OperandT>, //JODO parameter compatibility
                   bool>::type 
contains(const Type& super, const OperandT& sub)
{
    return interval_map_contains<Type>::apply(super, sub);
    //CL?? return Interval_Set::contains(super, sub);
}

template<class Type, class OperandT>
typename enable_if<has_same_concept<is_interval_set, Type, OperandT>, 
                   bool>::type 
contains(const Type& super, const OperandT& sub)
{
    return Interval_Set::contains(super, sub);
}

template<class Type, class OperandT>
typename enable_if<mpl::and_<is_interval_map<Type>,
                             is_intra_derivative<Type, OperandT> >, 
                   bool>::type
contains(const Type& super, const OperandT& sub)
{ 
    return interval_map_contains<Type>::apply(super, sub);
    //CL?? return Interval_Map::contains(super, sub);
}

template<class Type>
typename enable_if<is_interval_set<Type>, bool>::type
contains(const Type& super, const typename Type::segment_type& inter_val)
{ 
    typedef typename Type::const_iterator const_iterator;
    if(itl::is_empty(inter_val)) 
        return true;

    std::pair<const_iterator, const_iterator> exterior 
        = super.equal_range(inter_val);
    if(exterior.first == exterior.second)
        return false;

    const_iterator last_overlap = super.prior(exterior.second);

    return 
        itl::contains(hull(*(exterior.first), *last_overlap), inter_val)
    &&  Interval_Set::is_joinable(super, exterior.first, last_overlap);
}

template<class Type>
typename enable_if<is_interval_set<Type>, bool>::type
contains(const Type& super, const typename Type::element_type& element)
{
    return !(super.find(element) == super.end());
}

template<class Type, class OperandT>
typename enable_if<is_cross_derivative<Type, OperandT>, 
                   bool>::type
contains(const Type& super, const OperandT& sub)
{ 
    return interval_map_contains_key<Type, is_total<Type>::value>::apply(super, sub);
    //CL?? return Interval_Map::contains(super, sub);
}

template<class Type, class IntervalSetT>
typename enable_if<mpl::and_<is_interval_map<Type>, 
                             combines_right_to_interval_set<Type, IntervalSetT> >,
                   bool>::type
contains(const Type& super, const IntervalSetT& sub)
{
    return interval_map_contains_key<Type, is_total<Type>::value>::apply(super, sub);
    //CL?? return Interval_Map::contains(super, sub);
}


//------------------------------------------------------------------------------
//- within
//------------------------------------------------------------------------------
template<class SubT, class SuperT>
typename enable_if<is_interval_container<SuperT>, bool>::type 
within(const SubT& sub, const SuperT& super)
{
    return itl::contains(super, sub); 
}


//==============================================================================
//= Equivalences and Orderings
//==============================================================================
/** Returns true, if \c left and \c right contain the same elements. 
    Complexity: linear. */
template<class LeftT, class RightT>
typename enable_if<is_intra_combinable<LeftT, RightT>, bool>::type
is_element_equal(const LeftT& left, const RightT& right)
{
    return Interval_Set::is_element_equal(left, right);
}

/** Returns true, if \c left is lexicographically less than \c right. 
    Intervals are interpreted as sequence of elements.
    Complexity: linear. */
template<class LeftT, class RightT>
typename enable_if<is_intra_combinable<LeftT, RightT>, bool>::type
is_element_less(const LeftT& left, const RightT& right)
{
    return Interval_Set::is_element_less(left, right);
}

/** Returns true, if \c left is lexicographically greater than \c right. 
    Intervals are interpreted as sequence of elements.
    Complexity: linear. */
template<class LeftT, class RightT>
typename enable_if<is_intra_combinable<LeftT, RightT>, bool>::type
is_element_greater(const LeftT& left, const RightT& right)
{
    return Interval_Set::is_element_greater(left, right);
}

//------------------------------------------------------------------------------
template<class LeftT, class RightT>
typename enable_if<is_inter_combinable<LeftT, RightT>, int>::type
inclusion_compare(const LeftT& left, const RightT& right)
{
    return Interval_Set::subset_compare(left, right, 
                                        left.begin(), left.end(),
                                        right.begin(), right.end());
}

template<class LeftT, class RightT>
typename enable_if<is_concept_equivalent<is_element_container,LeftT, RightT>, 
                   int>::type
inclusion_compare(const LeftT& left, const RightT& right)
{
    return Set::subset_compare(left, right, 
                               left.begin(), left.end(),
                               right.begin(), right.end());
}
//------------------------------------------------------------------------------


//==============================================================================
//= Addition
//==============================================================================
//- add(interval_sets, [prior], {element, segment}) ----------------------------
template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
add(Type& object, const typename Type::element_type& operand)
{
    Interval_Set::add(object, typename Type::interval_type(operand));
    return object;
}

template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
add(Type& object, const typename Type::segment_type& operand)
{
    Interval_Set::add(object, operand);
    return object;
}

template<class Type>
typename enable_if<is_interval_set<Type>, typename Type::iterator>::type
add(Type& object, typename Type::iterator      prior, 
               const typename Type::segment_type& operand)
{
    return Interval_Set::add(object, prior, operand);
}

//------------------------------------------------------------------------------
//- add(interval_maps, [prior], {element, segment}) ----------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
add(Type& object, const typename Type::element_type& operand)
{
    typedef typename Type::codomain_combine codomain_combine;
    Interval_Map::add<Type,codomain_combine>(object, make_segment<Type>(operand));
    return object;
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
add(Type& object, const typename Type::segment_type& operand)
{
    typedef typename Type::codomain_combine codomain_combine;
    Interval_Map::add<Type,codomain_combine>(object, operand);
    return object;
}

template<class Type>
typename enable_if<is_interval_map<Type>, typename Type::iterator >::type
add(Type& object, typename Type::iterator      prior_,
            const typename Type::segment_type& operand)
{
    typedef typename Type::codomain_combine codomain_combine;
    return Interval_Map::add<Type,codomain_combine>(object, prior_, operand);
}
//------------------------------------------------------------------------------

/** \par \b Requires: \c OperandT is an interval container addable to \c Type. 
    \b Effects: \c operand is added to \c object.
    \par \b Returns: A reference to \c object.
    \b Complexity: loglinear */
template<class Type, class OperandT>
typename enable_if<is_intra_combinable<Type, OperandT>, Type>::type&
operator += (Type& object, const OperandT& operand)
{
    typename Type::iterator prior_ = object.end();
    ITL_const_FORALL(typename OperandT, elem_, operand) 
        prior_ = itl::add(object, prior_, *elem_); 

    return object; 
}

/* \par \b Requires: \c OperandT is an addable derivative type of \c Type. 
    \b Effects: \c operand is added to \c object.
    \par \b Returns: A reference to \c object.
    \b Complexity:
\code
                  \ OperandT:                    
                   \         element     segment 
Type:
       interval container    O(log n)     O(n)   

             interval_set               amortized
    spearate_interval_set                O(log n) 

n = object.interval_count()
\endcode

For the addition of \b elements or \b segments
complexity is \b logarithmic or \b linear respectively.
For \c interval_sets and \c separate_interval_sets addition of segments
is \b amortized \b logarithmic.
*/
template<class Type, class OperandT>
typename enable_if<is_intra_derivative<Type, OperandT>, Type>::type&
operator += (Type& object, const OperandT& operand)
{ 
    //JODO return itl::add(object, operand); 
	return itl::add(object, operand); 
}


/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c Type \c object compared to inplace \c operator \c += */
template<class Type, class OperandT>
typename enable_if<is_binary_intra_combinable<Type, OperandT>, Type>::type
operator + (Type object, const OperandT& operand)
{
    return object += operand; 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c Type \c object compared to inplace \c operator \c += */
template<class Type, class OperandT>
typename enable_if<is_binary_intra_combinable<Type, OperandT>, Type>::type
operator + (const OperandT& operand, Type object)
{
    return object += operand; 
}


/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c Type \c object compared to inplace \c operator \c += */
template<class Type>
Type operator + (typename Type::overloadable_type object, const Type& operand)
{
    return object += operand; 
}


//------------------------------------------------------------------------------
//- Addition |=, | 
//------------------------------------------------------------------------------

/** \par \b Requires: Types \c Type and \c OperandT are addable.
    \par \b Effects: \c operand is added to \c object.
    \par \b Returns: A reference to \c object.
    \b Complexity:
\code
                  \ OperandT:                      interval
                   \         element     segment   container
Type:
       interval container    O(log n)     O(n)     O(m log(n+m))

             interval_set               amortized
    spearate_interval_set                O(log n) 

n = object.interval_count()
m = operand.interval_count()
\endcode

For the addition of \b elements, \b segments and \b interval \b containers
complexity is \b logarithmic, \b linear and \b loglinear respectively.
For \c interval_sets and \c separate_interval_sets addition of segments
is \b amortized \b logarithmic.
*/
template<class Type, class OperandT>
typename enable_if<is_right_intra_combinable<Type, OperandT>, Type>::type&
operator |= (Type& object, const OperandT& operand)
{ 
    return object += operand; 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c Type \c object compared to inplace \c operator \c |= */
template<class Type, class OperandT>
typename enable_if<is_binary_intra_combinable<Type, OperandT>, Type>::type
operator | (Type object, const OperandT& operand)
{
    return object += operand; 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c Type \c object compared to inplace \c operator \c |= */
template<class Type, class OperandT>
typename enable_if<is_binary_intra_combinable<Type, OperandT>, Type>::type
operator | (const OperandT& operand, Type object)
{
    return object += operand; 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c Type \c object compared to inplace \c operator \c |= */
template<class Type>
Type operator | (typename Type::overloadable_type object, const Type& operand)
{
    return object += operand; 
}

//==============================================================================
//= Insertion
//==============================================================================
//------------------------------------------------------------------------------
//- Insertion<Interval_Set> fragment_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
insert(Type& object, const typename Type::element_type& operand)
{
    return itl::add(object, operand);
}

template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
insert(Type& object, const typename Type::segment_type& operand)
{
    return itl::add(object, operand);
}

//------------------------------------------------------------------------------
//- Insertion<Interval_Map> fragment_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
insert(Type& object, const typename Type::element_type& operand)
{
    Interval_Map::insert(object, make_segment<Type>(operand));
    return object;
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
insert(Type& object, const typename Type::segment_type& operand)
{
    Interval_Map::insert(object, operand);
    return object;
}

template<class Type>
typename enable_if<is_interval_map<Type>, typename Type::iterator>::type
insert(Type& object, typename Type::iterator      prior,
               const typename Type::segment_type& operand)
{
    return Interval_Map::insert(object, prior, operand);
}

//------------------------------------------------------------------------------
template<class Type, class OperandT>
typename enable_if<is_intra_combinable<Type, OperandT>, Type>::type&
insert(Type& object, const OperandT& operand)
{
    typename Type::iterator prior_ = object.end();
    ITL_const_FORALL(typename OperandT, elem_, operand) 
        insert(object, *elem_); 

    return object; 
}

//==============================================================================
//= Erasure
//==============================================================================
//------------------------------------------------------------------------------
//- Erasure<Interval_Set> fragment_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
erase(Type& object, const typename Type::segment_type& minuend)
{
    return itl::subtract(object, minuend);
}

template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
erase(Type& object, const typename Type::element_type& minuend)
{
    return itl::subtract(object, minuend);
}

//------------------------------------------------------------------------------
//- Erasure<Interval_Map> key_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
erase(Type& object, const typename Type::interval_type& minuend)
{
    typedef typename Type::interval_type  interval_type;
    typedef typename Type::value_type     value_type;
    typedef typename Type::iterator       iterator;

    if(itl::is_empty(minuend)) 
        return object;

    std::pair<iterator, iterator> exterior = object.equal_range(minuend);
    if(exterior.first == exterior.second)
        return object;

    iterator first_ = exterior.first,
             end_   = exterior.second,
             last_  = prior(end_);

    interval_type left_resid  = right_subtract(first_->first, minuend);
    interval_type right_resid =  left_subtract(last_ ->first, minuend);

    if(first_ == last_ )
        if(!itl::is_empty(left_resid))
        {
            const_cast<interval_type&>(first_->first) = left_resid;
            if(!itl::is_empty(right_resid))
				itl::insert(object, first_, value_type(right_resid, first_->second));
        }
        else if(!itl::is_empty(right_resid))
            const_cast<interval_type&>(first_->first) = left_subtract(first_->first, minuend);
        else
            object.erase(first_);
    else
    {   //            [-------- minuend ---------)
        // [left_resid   fst)   . . . .    [lst  right_resid)
        iterator second_= first_; ++second_;

        iterator start_ = itl::is_empty(left_resid)? first_: second_;
        iterator stop_  = itl::is_empty(right_resid)? end_  : last_ ;
        object.erase(start_, stop_); //erase [start_, stop_)

        if(!itl::is_empty(left_resid))
            const_cast<interval_type&>(first_->first) = left_resid;

        if(!itl::is_empty(right_resid))
            const_cast<interval_type&>(last_ ->first) = right_resid;
    }
    return object;
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
erase(Type& object, const typename Type::domain_type& minuend)
{
    typedef typename Type::interval_type interval_type;
    return itl::erase(object, interval_type(minuend));
}

//------------------------------------------------------------------------------
//- Erasure<Interval_Map> fragment_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
erase(Type& object, const typename Type::segment_type& minuend)
{
    Interval_Map::erase(object, minuend);
    return object;
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
erase(Type& object, const typename Type::element_type& minuend)
{
    return itl::erase(object, make_segment<Type>(minuend));
}

//------------------------------------------------------------------------------
template<class Type, class OperandT>
typename enable_if<combines_right_to_interval_container<Type, OperandT>,
                   Type>::type&
erase(Type& object, const OperandT& operand)
{
    typedef typename OperandT::const_iterator const_iterator;

    if(itl::is_empty(operand))
        return object;

    const_iterator common_lwb, common_upb;
    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return object;

    const_iterator it_ = common_lwb;
    while(it_ != common_upb)
        itl::erase(object, *it_++);

    return object; 
}

//==============================================================================
//= Subtraction
//==============================================================================
//------------------------------------------------------------------------------
//- Subtraction<Interval_Set> fragment_type
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
subtract(Type& object, const typename Type::element_type& operand)
{
    Interval_Set::subtract(object, typename Type::segment_type(operand));
    return object;
}

template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
subtract(Type& object, const typename Type::segment_type& operand)
{
    Interval_Set::subtract(object, operand);
    return object;
}

//------------------------------------------------------------------------------
//- Subtraction<Interval_Map> fragment_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if
    <mpl::and_< is_interval_map<Type>
              , mpl::and_< is_total<Type> 
                         , has_inverse<typename Type::codomain_type> >
              >, 
    Type>::type&
subtract(Type& object, const typename Type::segment_type& operand)
{
    typedef typename Type::inverse_codomain_combine inverse_codomain_combine;
    Interval_Map::add<Type,inverse_codomain_combine>(object, operand);
    return object;
}

template<class Type>
typename enable_if
    <mpl::and_< is_interval_map<Type>
              , mpl::not_<mpl::and_< is_total<Type> 
                                   , has_inverse<typename Type::codomain_type> > >
              >, 
    Type>::type&
subtract(Type& object, const typename Type::segment_type& operand)
{
    typedef typename Type::inverse_codomain_combine inverse_codomain_combine;
    Interval_Map::subtract<Type,inverse_codomain_combine>(object, operand);
    return object;
}


template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
subtract(Type& object, const typename Type::element_type& operand)
{
    itl::subtract(object, make_segment<Type>(operand));
    return object;
}

//------------------------------------------------------------------------------
//- Subtraction<Interval_Map> key_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
subtract(Type& object, const typename Type::domain_type& operand)
{
    typedef typename Type::interval_type interval_type;
    Interval_Map::erase(object, interval_type(operand));
    return object;
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
subtract(Type& object, const typename Type::interval_type& operand)
{
    Interval_Map::erase(object, operand);
    return object;
}


//------------------------------------------------------------------------------
//- Subtraction -=, -
//------------------------------------------------------------------------------

/** \par \b Requires: Types \c Type and \c OperandT are subtractable.
    \par \b Effects: \c operand is subtracted from \c object.
    \par \b Returns: A reference to \c object.
    \b Complexity:
\code
                  \ OperandT:                      interval
                   \         element    segment    container
Type:
       interval container    O(log n)     O(n)     O(m log(n+m))

                                       amortized
            interval_sets               O(log n) 

n = object.interval_count()
m = operand.interval_count()
\endcode

For the subtraction of \em elements, \b segments and \b interval \b containers
complexity is \b logarithmic, \b linear and \b loglinear respectively.
For interval sets subtraction of segments
is \b amortized \b logarithmic.
*/
template<class Type, class OperandT>
typename enable_if<has_same_concept<is_interval_map, Type, OperandT>, 
                   Type>::type& 
operator -=(Type& object, const OperandT& operand)
{
    ITL_const_FORALL(typename OperandT, elem_, operand) 
		itl::subtract(object, *elem_);

    return object; 
}

template<class Type, class OperandT>
typename enable_if<is_intra_derivative<Type, OperandT>, Type>::type&
operator -= (Type& object, const OperandT& operand)
{ 
	return itl::subtract(object, operand); 
}

template<class Type, class OperandT>
typename enable_if<is_cross_derivative<Type, OperandT>, Type>::type&
operator -= (Type& object, const OperandT& operand)
{ 
	return itl::erase(object, operand); 
}

template<class Type, class IntervalSetT>
typename enable_if<combines_right_to_interval_set<Type, IntervalSetT>,
                   Type>::type&
operator -= (Type& object, const IntervalSetT& operand)
{
    return erase(object, operand);
}


template<class Type, class OperandT>
typename enable_if<is_right_inter_combinable<Type, OperandT>, Type>::type
operator - (Type object, const OperandT& operand)
{
    return object -= operand; 
}


//------------------------------------------------------------------------------
//- set (selective update)
//------------------------------------------------------------------------------

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
set_at(Type& object, const typename Type::segment_type& operand)
{
    itl::erase(object, operand.first); 
    return itl::insert(object, operand); 
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
set_at(Type& object, const typename Type::element_type& operand)
{
    return itl::set_at(object, make_segment<Type>(operand));
}


//==============================================================================
//= Intersection
//==============================================================================
//------------------------------------------------------------------------------
//- Intersection add_intersection <interval_set>
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_set<Type>, void>::type
add_intersection(Type& section, const Type& object, 
                 const typename Type::domain_type& operand)
{
    typedef typename Type::const_iterator const_iterator;
    const_iterator found = object.find(operand);
    if(found != object.end())
        itl::add(section, operand);
}


template<class Type>
typename enable_if<is_interval_set<Type>, void>::type
add_intersection(Type& section, const Type& object, 
                 const typename Type::segment_type& segment)
{
    typedef typename Type::const_iterator const_iterator;
    typedef typename Type::iterator       iterator;
    typedef typename Type::interval_type  interval_type;

    if(itl::is_empty(segment)) 
        return;

    std::pair<const_iterator, const_iterator> exterior 
        = object.equal_range(segment);
    if(exterior.first == exterior.second)
        return;

    iterator prior_ = section.end();
    for(const_iterator it_=exterior.first; it_ != exterior.second; it_++) 
    {
        interval_type common_interval = Type::key_value(it_) & segment;
        if(!itl::is_empty(common_interval))
            prior_ = section._insert(prior_, common_interval);
    }
}


template<class Type, class OperandT>
typename enable_if<mpl::and_<is_interval_set<Type>, 
                             combines_right_to_interval_set<Type, OperandT> >,
                   void>::type
add_intersection(Type& section, const Type& object, const OperandT& operand)
{
    typedef typename OperandT::const_iterator const_iterator;

    if(operand.empty())
        return;

    const_iterator common_lwb, common_upb;
    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return;

    const_iterator it_ = common_lwb;
    while(it_ != common_upb)
        itl::add_intersection(section, object, OperandT::key_value(it_++));
}


//------------------------------------------------------------------------------
//- Intersection add_intersection<IntervalMaps> for key_types
//------------------------------------------------------------------------------
template<class Type, class OperandT>
typename enable_if<mpl::and_< is_total<Type>
                            , is_interval_map_fragment_type_of<OperandT, Type> >, void>::type
add_intersection(Type& section, const Type& object, const OperandT& operand)
{
    section += object;
    section += operand;
}

template<class Type>
typename enable_if<mpl::and_< mpl::not_<is_total<Type> >, is_interval_map<Type> >, void>::type
add_intersection(Type& section, const Type& object, const typename Type::element_type& operand)
{
    add_intersection(section, object, make_segment<Type>(operand));
}

template<class Type>
typename enable_if<mpl::and_< mpl::not_<is_total<Type> >, is_interval_map<Type> >, void>::type
add_intersection(Type& section, const Type& object, const typename Type::segment_type& operand)
{
    typedef typename Type::segment_type       segment_type;
    typedef typename Type::interval_type      interval_type;
    typedef typename Type::value_type         value_type;
    typedef typename Type::const_iterator     const_iterator;
    typedef typename Type::codomain_combine   codomain_combine;
    typedef typename Type::codomain_intersect codomain_intersect;

    interval_type inter_val = operand.first;
    if(itl::is_empty(inter_val)) 
        return;

    std::pair<const_iterator, const_iterator> exterior 
        = object.equal_range(inter_val);
    if(exterior.first == exterior.second)
        return;

    for(const_iterator it_=exterior.first; it_ != exterior.second; it_++) 
    {
        interval_type common_interval = it_->first & inter_val; 
        if(!itl::is_empty(common_interval))
        {
            Interval_Map::add<Type,codomain_combine>  (section, value_type(common_interval, it_->second) );
            Interval_Map::add<Type,codomain_intersect>(section, value_type(common_interval, operand.second));
        }
    }
}

template<class Type, class MapT>
typename enable_if<mpl::and_< mpl::not_<is_total<Type> >
                            , is_concept_compatible<is_interval_map, Type, MapT> >, void>::type
add_intersection(Type& section, const Type& object, const MapT& operand)
{
    typedef typename Type::segment_type   segment_type;
    typedef typename Type::interval_type  interval_type;
    typedef typename MapT::const_iterator const_iterator;

    if(operand.empty()) 
        return;
    const_iterator common_lwb, common_upb;
    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return;
    const_iterator it_ = common_lwb;
    while(it_ != common_upb)
        add_intersection(section, object, *it_++);
}

//------------------------------------------------------------------------------
//- Intersection add_intersection<IntervalMaps> for fragment_types
//------------------------------------------------------------------------------

template<class Type>
typename enable_if<is_interval_map<Type>, void>::type
add_intersection(Type& section, const Type& object, 
                 const typename Type::domain_type& key_value)
{
    typedef typename Type::interval_type  interval_type;
    typedef typename Type::segment_type   segment_type;
    typedef typename Type::const_iterator const_iterator;

    const_iterator it_ = object.find(key_value);
    if(it_ != object.end())
        add(section, segment_type(interval_type(key_value),it_->second));
}

template<class Type>
typename enable_if<is_interval_map<Type>, void>::type
add_intersection(Type& section, const Type& object, 
                 const typename Type::interval_type& inter_val)
{
    typedef typename Type::interval_type  interval_type;
    typedef typename Type::value_type     value_type;
    typedef typename Type::const_iterator const_iterator;
    typedef typename Type::iterator       iterator;

    if(itl::is_empty(inter_val)) 
        return;

    std::pair<const_iterator, const_iterator> exterior 
        = object.equal_range(inter_val);
    if(exterior.first == exterior.second)
        return;

    iterator prior_ = section.end();
    for(const_iterator it_=exterior.first; it_ != exterior.second; it_++) 
    {
        interval_type common_interval = it_->first & inter_val; 
        if(!itl::is_empty(common_interval))
            prior_ = add(section, prior_, 
                         value_type(common_interval, it_->second) );
    }
}

template<class Type, class KeySetT>
typename enable_if<is_concept_combinable<is_interval_map, is_interval_set, Type, KeySetT>, void>::type
add_intersection(Type& section, const Type& object, const KeySetT& key_set)
{
    typedef typename KeySetT::const_iterator const_iterator;

    if(itl::is_empty(key_set)) 
        return;

    const_iterator common_lwb, common_upb;
    if(!Set::common_range(common_lwb, common_upb, key_set, object))
        return;

    const_iterator it_ = common_lwb;
    while(it_ != common_upb)
        add_intersection(section, object, *it_++);
}



//------------------------------------------------------------------------------
//- Intersection &=, &
//------------------------------------------------------------------------------
template<class Type, class OperandT>
typename enable_if<is_right_inter_combinable<Type, OperandT>, Type>::type&
operator &= (Type& object, const OperandT& operand)
{
    Type intersection;
    add_intersection(intersection, object, operand);
    object.swap(intersection);
    return object;
}

template<class Type, class OperandT>
typename enable_if<is_binary_inter_combinable<Type, OperandT>, Type>::type
operator & (Type object, const OperandT& operand)
{
    return object &= operand; 
}

template<class Type, class OperandT>
typename enable_if<is_binary_inter_combinable<Type, OperandT>, Type>::type
operator & (const OperandT& operand, Type object)
{
    return object &= operand; 
}

template<class Type>
Type operator & (typename Type::overloadable_type object, const Type& operand)
{
    return object &= operand; 
}

//------------------------------------------------------------------------------
//- intersects
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//- intersects<Interval_Container> key_types
//------------------------------------------------------------------------------
template<class Type, class AssociateT>
typename enable_if<mpl::and_< is_interval_container<Type>
                            , is_same<AssociateT, domain_type_of<Type> > >, 
                   bool>::type
intersects(const Type& left, const AssociateT& right)
{
    return itl::contains(left, right); 
}

template<class Type, class AssociateT>
typename enable_if<mpl::and_< is_interval_container<Type>
                            , is_same<AssociateT, interval_type_of<Type> > >, 
                   bool>::type
intersects(const Type& left, const AssociateT& right)
{
    return left.find(right) != left.end();
}

//------------------------------------------------------------------------------
//- intersects<Interval_Maps> fragment_types
//------------------------------------------------------------------------------

template<class Type, class OperandT>
typename enable_if<mpl::and_< is_interval_map<Type>
                            , is_total<Type>
                            , is_same<OperandT, segment_type_of<Type> > >, 
                   bool>::type
intersects(const Type&, const OperandT&)
{
    return true;
}

template<class Type, class OperandT>
typename enable_if<mpl::and_< is_interval_map<Type>
                            , mpl::not_<is_total<Type> >
                            , is_same<OperandT, segment_type_of<Type> > >, 
                   bool>::type
intersects(const Type& object, const OperandT& operand)
{
    Type intersection; //JODO
    itl::add_intersection(intersection, left, operand);
    return !itl::is_empty(intersection); 
}

template<class Type, class OperandT>
typename enable_if<mpl::and_< is_interval_map<Type>
                            , is_same<OperandT, element_type_of<Type> > >, 
                   bool>::type
intersects(const Type& object, const OperandT& operand)
{
    return itl::intersects(object, make_segment<Type>(operand)); 
}

//------------------------------------------------------------------------------

#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif                        

template<class LeftT, class RightT>
typename enable_if<is_intra_combinable<LeftT, RightT>, 
                          bool>::type
intersects(const LeftT& left, const RightT& right)
{
    if(mpl::or_<is_total<LeftT>, is_total<RightT> >::value)
        return true;

    LeftT intersection;

    typename RightT::const_iterator right_common_lower_;
    typename RightT::const_iterator right_common_upper_;

    if(!Set::common_range(right_common_lower_, right_common_upper_, right, left))
        return false;

    typename RightT::const_iterator it_ = right_common_lower_;
    while(it_ != right_common_upper_)
    {
        itl::add_intersection(intersection, left, *it_++);
        if(!itl::is_empty(intersection))
            return true;
    }

    return false; 
}

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif


template<class LeftT, class RightT>
typename enable_if<is_cross_combinable<LeftT, RightT>, 
                          bool>::type
intersects(const LeftT& left, const RightT& right)
{
    LeftT intersection;

    if(itl::is_empty(left) || itl::is_empty(right))
        return false;

    typename RightT::const_iterator right_common_lower_;
    typename RightT::const_iterator right_common_upper_;

    if(!Set::common_range(right_common_lower_, right_common_upper_, right, left))
        return false;

    typename RightT::const_iterator it_ = right_common_lower_;
    while(it_ != right_common_upper_)
    {
        itl::add_intersection(intersection, left, RightT::key_value(it_++));
        if(!itl::is_empty(intersection))
            return true;
    }

    return false; 
}

template<class Type, class AssociateT>
typename enable_if<mpl::and_< is_interval_map<Type>
                            , is_inter_derivative<Type, AssociateT> >, 
                   bool>::type
intersects(const Type& left, const AssociateT& right)
{
	return itl::intersects(left, right);
}

/** \b Returns true, if \c left and \c right have no common elements.
    Intervals are interpreted as sequence of elements.
    \b Complexity: loglinear, if \c left and \c right are interval containers. */
template<class LeftT, class RightT>
typename enable_if<is_inter_combinable<LeftT, RightT>, bool>::type
disjoint(const LeftT& left, const RightT& right)
{
    return !intersects(left, right);
}

/** \b Returns true, if \c left and \c right have no common elements.
    Intervals are interpreted as sequence of elements.
    \b Complexity: logarithmic, if \c AssociateT is an element type \c Type::element_type. 
    linear, if \c AssociateT is a segment type \c Type::segment_type. */
template<class Type, class AssociateT>
typename enable_if<is_inter_derivative<Type, AssociateT>, bool>::type
disjoint(const Type& left, const AssociateT& right)
{
    return !left.intersects(right);
}


//==============================================================================
//= Symmetric difference
//==============================================================================
//------------------------------------------------------------------------------
//- Symmetric difference flip<Interval_Set> fragment_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
flip(Type& object, const typename Type::element_type& operand)
{
    if(itl::contains(object, operand))
        return object -= operand;
    else
        return object += operand;
}

template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
flip(Type& object, const typename Type::segment_type& segment)
{
    typedef typename Type::const_iterator const_iterator;
    typedef typename Type::interval_type  interval_type;
    // That which is common shall be subtracted
    // That which is not shall be added
    // So x has to be 'complementary added' or flipped
    interval_type span = segment;
    std::pair<const_iterator, const_iterator> exterior 
        = object.equal_range(span);

    const_iterator fst_ = exterior.first;
    const_iterator end_ = exterior.second;

    interval_type covered, left_over;
    const_iterator it_ = fst_;
    while(it_ != end_) 
    {
        covered = *it_++; 
        //[a      ...  : span
        //     [b ...  : covered
        //[a  b)       : left_over
        left_over = right_subtract(span, covered);
        itl::subtract(object, span & covered); //That which is common shall be subtracted
        itl::add(object, left_over);              //That which is not shall be added

        //...      d) : span
        //... c)      : covered
        //     [c  d) : span'
        span = left_subtract(span, covered);
    }

    //If span is not empty here, it_ is not in the set so it_ shall be added
    itl::add(object, span);
    return object;
}


template<class Type, class OperandT>
typename enable_if<is_concept_compatible<is_interval_set, Type, OperandT>, Type>::type&
flip(Type& object, const OperandT& operand)
{
    typedef typename OperandT::const_iterator const_iterator;

    if(operand.empty())
        return object;

    const_iterator common_lwb, common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return object += operand;

    const_iterator it_ = operand.begin();

    // All elements of operand left of the common range are added
    while(it_ != common_lwb)
        itl::add(object, *it_++);
    // All elements of operand in the common range are symmertrically subtracted
    while(it_ != common_upb)
        itl::flip(object, *it_++);
    // All elements of operand right of the common range are added
    while(it_ != operand.end())
        itl::add(object, *it_++);

    return object;
}

//------------------------------------------------------------------------------
//- Symmetric difference flip<Interval_Map> fragment_types
//------------------------------------------------------------------------------
template<class Type, class OperandT>
typename enable_if< mpl::and_< is_interval_map_right_intra_combinable<Type, OperandT> //JODO =^= fragment_type_of
                             , is_total<Type>
                             , absorbs_neutrons<Type> >
                  , Type >::type&
flip(Type& object, const OperandT&)
{
    itl::clear(object);
    return object;
}


template<class Type, class OperandT>
typename enable_if< mpl::and_< is_interval_map_right_intra_combinable<Type, OperandT>
                             , is_total<Type>
                             , mpl::not_<absorbs_neutrons<Type> > >
                  , Type >::type&
flip(Type& object, const OperandT& operand)
{
    typedef typename Type::codomain_type codomain_type;
    object += operand;
    ITL_FORALL(typename Type, it_, object) //JODO: neutralisierendes add.
        it_->second = neutron<codomain_type>::value();

    if(mpl::not_<is_interval_splitter<Type> >::value) //JODO
        object.join();

    return object;
}


template<class Type>
typename enable_if<mpl::and_< is_interval_map<Type>
                            , mpl::not_<is_total<Type> > >, Type>::type&
flip(Type& object, const typename Type::segment_type& interval_value_pair)
{
    typedef typename Type::set_type       set_type;
    typedef typename Type::interval_type  interval_type;
    typedef typename Type::value_type     value_type;
    typedef typename Type::const_iterator const_iterator;
    typedef typename Type::codomain_type  codomain_type;
    typedef typename Type::inverse_codomain_intersect inverse_codomain_intersect;
    // That which is common shall be subtracted
    // That which is not shall be added
    // So x has to be 'complementary added' or flipped

    interval_type span = interval_value_pair.first;
    std::pair<const_iterator, const_iterator> exterior 
        = object.equal_range(span);

    const_iterator first_ = exterior.first;
    const_iterator end_   = exterior.second;

    interval_type covered, left_over, common_interval;
    const codomain_type& x_value = interval_value_pair.second;
    const_iterator it_ = first_;

    set_type eraser;
    Type     intersection;

    while(it_ != end_  ) 
    {
        const codomain_type& co_value = it_->second;
        covered = (*it_++).first;
        //[a      ...  : span
        //     [b ...  : covered
        //[a  b)       : left_over
        left_over = right_subtract(span, covered);

        //That which is common ...
        common_interval = span & covered;
        if(!itl::is_empty(common_interval))
        {
            // ... shall be subtracted
            itl::add(eraser, common_interval);

            if(has_set_semantics<codomain_type>::value) //JODO
            {
                codomain_type common_value = x_value;
                inverse_codomain_intersect()(common_value, co_value);
                itl::add(intersection, value_type(common_interval, common_value));
            }
            else
                itl::add(intersection, value_type(common_interval, neutron<codomain_type>::value()));
        }

        itl::add(object, value_type(left_over, x_value)); //That which is not shall be added
        // Because this is a collision free addition I don't have to distinguish codomain_types.

        //...      d) : span
        //... c)      : covered
        //     [c  d) : span'
        span = left_subtract(span, covered);
    }

    //If span is not empty here, it is not in the set so it shall be added
    itl::add(object, value_type(span, x_value));

    //finally rewrite the common segments
    itl::erase(object, eraser);
    object += intersection;

    return object;
}

template<class Type>
typename enable_if<mpl::and_< is_interval_map<Type>
                            , mpl::not_<is_total<Type> > >, Type>::type&
flip(Type& object, const typename Type::element_type& key_value_pair)
{
	return itl::flip(object, make_segment<Type>(key_value_pair));
}



template<class Type, class OperandT>
typename enable_if< mpl::and_< mpl::not_<is_total<Type> > 
                             , is_concept_compatible<is_interval_map, 
                                                     Type, OperandT >
                             >
                  , Type>::type&
flip(Type& object, const OperandT& operand)
{
    typedef typename OperandT::const_iterator const_iterator;
    typedef typename Type::codomain_type  codomain_type;

    const_iterator common_lwb, common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return object += operand;

    const_iterator it_ = operand.begin();

    // All elements of operand left of the common range are added
    while(it_ != common_lwb)
        itl::add(object, *it_++);
    // All elements of operand in the common range are symmetrically subtracted
    while(it_ != common_upb)
        itl::flip(object, *it_++);
    // All elements of operand right of the common range are added
    while(it_ != operand.end())
        itl::add(object, *it_++);

    return object;
}



//------------------------------------------------------------------------------
//- Symmetric difference ^=, ^
//------------------------------------------------------------------------------
template<class Type, class OperandT>
typename enable_if<is_intra_combinable<Type, OperandT>, 
                          Type>::type&
operator ^= (Type& object, const OperandT& operand)
{ 
	return itl::flip(object, operand); 
}

template<class Type, class OperandT>
typename enable_if<is_intra_derivative<Type, OperandT>, 
                          Type>::type&
operator ^= (Type& object, const OperandT& operand)
{ 
	return itl::flip(object, operand); 
}

template<class Type, class OperandT>
typename enable_if<is_binary_intra_combinable<Type, OperandT>, Type>::type
operator ^ (Type object, const OperandT& operand)
{
    return object ^= operand; 
}

template<class Type, class OperandT>
typename enable_if<is_binary_intra_combinable<Type, OperandT>, Type>::type
operator ^ (const OperandT& operand, Type object)
{
    return object ^= operand; 
}


template<class Type>
Type operator ^ (typename Type::overloadable_type object, const Type& operand)
{
    return object ^= operand; 
}

//==============================================================================
//= Morphisms
//==============================================================================

template<class Type>
typename enable_if<is_interval_set<Type>, Type>::type&
join(Type& object)
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::iterator      iterator;

    iterator it_ = object.begin();
    if(it_ == object.end()) 
        return object;

    iterator next_ = it_; next_++;

    while(next_ != object.end())
    {
        if( touches(*it_, *next_) )
        {
            iterator fst_mem = it_;  // hold the first member
            
            // Go on while touching members are found
            it_++; next_++;
            while(     next_ != object.end()
                    && touches(*it_, *next_) )
            { it_++; next_++; }

            // finally we arrive at the end of a sequence of joinable intervals
            // and it points to the last member of that sequence
            const_cast<interval_type&>(*it_) = hull(*it_, *fst_mem);
            object.erase(fst_mem, it_);

            it_++; next_=it_; 
            if(next_!=object.end())
                next_++;
        }
        else { it_++; next_++; }
    }
    return object;
}



}} // namespace itl boost

#endif


