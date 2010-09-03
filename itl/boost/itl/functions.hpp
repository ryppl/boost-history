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
#include <boost/itl/type_traits/segment_type_of.hpp>
#include <boost/itl/type_traits/is_interval_container.hpp>
#include <boost/itl/type_traits/is_interval_separator.hpp>
#include <boost/itl/type_traits/is_interval_splitter.hpp>
#include <boost/itl/type_traits/is_interval_joiner.hpp>
#include <boost/itl/type_traits/is_element_container.hpp>
#include <boost/itl/type_traits/is_combinable.hpp>
#include <boost/itl/detail/interval_map_functors.hpp>
#include <boost/itl/detail/interval_map_algo.hpp>

#include <boost/itl/functions/container.hpp>

namespace boost{namespace itl
{

//==========================================================================
//= Containedness
//==========================================================================


//------------------------------------------------------------------------------
//- contains
//------------------------------------------------------------------------------

template<class ObjectT, class OperandT>
typename enable_if<has_same_concept<is_interval_map, ObjectT, OperandT>, 
                   bool>::type 
contains(const ObjectT& super, const OperandT& sub)
{
    return interval_map_contains<ObjectT>::apply(super, sub);
    //CL?? return Interval_Set::contains(super, sub);
}

template<class ObjectT, class OperandT>
typename enable_if<has_same_concept<is_interval_set, ObjectT, OperandT>, 
                   bool>::type 
contains(const ObjectT& super, const OperandT& sub)
{
    return Interval_Set::contains(super, sub);
}

template<class ObjectT, class OperandT>
typename enable_if<mpl::and_<is_interval_map<ObjectT>,
                             is_intra_derivative<ObjectT, OperandT> >, 
                   bool>::type
contains(const ObjectT& super, const OperandT& sub)
{ 
    return interval_map_contains<ObjectT>::apply(super, sub);
    //CL?? return Interval_Map::contains(super, sub);
}

template<class ObjectT>
typename enable_if<is_interval_set<ObjectT>, bool>::type
contains(const ObjectT& super, const typename ObjectT::segment_type& inter_val)
{ 
    typedef typename ObjectT::const_iterator const_iterator;
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

template<class ObjectT>
typename enable_if<is_interval_set<ObjectT>, bool>::type
contains(const ObjectT& super, const typename ObjectT::element_type& element)
{
    return !(super.find(element) == super.end());
}

template<class ObjectT, class OperandT>
typename enable_if<is_cross_derivative<ObjectT, OperandT>, 
                   bool>::type
contains(const ObjectT& super, const OperandT& sub)
{ 
    return interval_map_contains_key<ObjectT, is_total<ObjectT>::value>::apply(super, sub);
    //CL?? return Interval_Map::contains(super, sub);
}

template<class ObjectT, class IntervalSetT>
typename enable_if<mpl::and_<is_interval_map<ObjectT>, 
                             combines_right_to_interval_set<ObjectT, IntervalSetT> >,
                   bool>::type
contains(const ObjectT& super, const IntervalSetT& sub)
{
    return interval_map_contains_key<ObjectT, is_total<ObjectT>::value>::apply(super, sub);
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

//- joining_add ----------------------------------------------------------------
template<class ObjectT>
typename enable_if<mpl::and_< is_interval_set<ObjectT>
                            , is_interval_joiner<ObjectT> >, ObjectT>::type&
add(ObjectT& object, const typename ObjectT::element_type& operand)
{
    detail::joining_add(object, typename ObjectT::interval_type(operand));
    return object; //JODO: May be it is better to return the iterator
}

template<class ObjectT>
typename enable_if<mpl::and_< is_interval_set<ObjectT>
                            , is_interval_joiner<ObjectT> >, ObjectT>::type&
add(ObjectT& object, const typename ObjectT::segment_type& operand)
{
    detail::joining_add(object, operand);
    return object; //JODO: May be it is better to return the iterator
}

template<class ObjectT>
typename enable_if<mpl::and_< is_interval_set<ObjectT>
                            , is_interval_joiner<ObjectT> >, 
                   typename ObjectT::iterator>::type
add(ObjectT& object, typename ObjectT::iterator      prior, 
               const typename ObjectT::segment_type& operand)
{
    return detail::joining_add(object, prior, operand);
}

//- separating_add -------------------------------------------------------------
template<class ObjectT>
typename enable_if<mpl::and_< is_interval_set<ObjectT>
                            , is_interval_separator<ObjectT> >, ObjectT>::type&
add(ObjectT& object, const typename ObjectT::element_type& operand)
{
    detail::separating_add(object, typename ObjectT::interval_type(operand));
    return object;
}

template<class ObjectT>
typename enable_if<mpl::and_< is_interval_set<ObjectT>
                            , is_interval_separator<ObjectT> >, ObjectT>::type&
add(ObjectT& object, const typename ObjectT::segment_type& operand)
{
    detail::separating_add(object, operand);
    return object; //JODO: May be it is better to return the iterator
}

template<class ObjectT>
typename enable_if<mpl::and_< is_interval_set<ObjectT>
                            , is_interval_separator<ObjectT> >,
                   typename ObjectT::iterator>::type
add(ObjectT& object, typename ObjectT::iterator      prior, 
               const typename ObjectT::segment_type& operand)
{
    return detail::separating_add(object, prior, operand);
}

//- splitting_add -------------------------------------------------------------
template<class ObjectT>
typename enable_if<mpl::and_< is_interval_set<ObjectT>
                            , is_interval_splitter<ObjectT> >, ObjectT>::type&
add(ObjectT& object, const typename ObjectT::element_type& operand)
{
    detail::splitting_add(object, typename ObjectT::interval_type(operand));
    return object;
}

template<class ObjectT>
typename enable_if<mpl::and_< is_interval_set<ObjectT>
                            , is_interval_splitter<ObjectT> >, ObjectT>::type&
add(ObjectT& object, const typename ObjectT::segment_type& operand)
{
    detail::splitting_add(object, operand);
    return object; //JODO: May be it is better to return the iterator
}

template<class ObjectT>
typename enable_if<mpl::and_< is_interval_set<ObjectT>
                            , is_interval_splitter<ObjectT> >,
                   typename ObjectT::iterator>::type
add(ObjectT& object, typename ObjectT::iterator      prior, 
               const typename ObjectT::segment_type& operand)
{
    return detail::splitting_add(object, prior, operand);
}
//------------------------------------------------------------------------------


/** \par \b Requires: \c OperandT is an interval container addable to \c ObjectT. 
    \b Effects: \c operand is added to \c object.
    \par \b Returns: A reference to \c object.
    \b Complexity: loglinear */
template<class ObjectT, class OperandT>
typename enable_if<is_intra_combinable<ObjectT, OperandT>, ObjectT>::type&
operator += (ObjectT& object, const OperandT& operand)
{
    typename ObjectT::iterator prior_ = object.end();
    ITL_const_FORALL(typename OperandT, elem_, operand) 
        prior_ = object.add(prior_, *elem_); //JODO

    return object; 
}

/* \par \b Requires: \c OperandT is an addable derivative type of \c ObjectT. 
    \b Effects: \c operand is added to \c object.
    \par \b Returns: A reference to \c object.
    \b Complexity:
\code
                  \ OperandT:                    
                   \         element     segment 
ObjectT:
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
template<class ObjectT, class OperandT>
typename enable_if<is_intra_derivative<ObjectT, OperandT>, ObjectT>::type&
operator += (ObjectT& object, const OperandT& operand)
{ 
    //JODO return itl::add(object, operand); 
    return object.add(operand); 
}

//CL
//template<class ObjectT, class OperandT>
//typename enable_if<is_intra_derivative<ObjectT, OperandT>, ObjectT>::type&
//add(ObjectT& object, const OperandT& operand)
//{ 
//    return object.add(operand); 
//}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c ObjectT \c object compared to inplace \c operator \c += */
template<class ObjectT, class OperandT>
typename enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator + (ObjectT object, const OperandT& operand)
{
    return object += operand; 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c ObjectT \c object compared to inplace \c operator \c += */
template<class ObjectT, class OperandT>
typename enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator + (const OperandT& operand, ObjectT object)
{
    return object += operand; 
}


/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c ObjectT \c object compared to inplace \c operator \c += */
template<class ObjectT>
ObjectT operator + (typename ObjectT::overloadable_type object, const ObjectT& operand)
{
    return object += operand; 
}


//------------------------------------------------------------------------------
//- Addition |=, | 
//------------------------------------------------------------------------------

/** \par \b Requires: Types \c ObjectT and \c OperandT are addable.
    \par \b Effects: \c operand is added to \c object.
    \par \b Returns: A reference to \c object.
    \b Complexity:
\code
                  \ OperandT:                      interval
                   \         element     segment   container
ObjectT:
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
template<class ObjectT, class OperandT>
typename enable_if<is_right_intra_combinable<ObjectT, OperandT>, ObjectT>::type&
operator |= (ObjectT& object, const OperandT& operand)
{ 
    return object += operand; 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c ObjectT \c object compared to inplace \c operator \c |= */
template<class ObjectT, class OperandT>
typename enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator | (ObjectT object, const OperandT& operand)
{
    return object += operand; 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c ObjectT \c object compared to inplace \c operator \c |= */
template<class ObjectT, class OperandT>
typename enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator | (const OperandT& operand, ObjectT object)
{
    return object += operand; 
}

/** \par \b Requires: \c object and \c operand are addable.
    \b Effects: \c operand is added to \c object.
    \par \b Efficieny: There is one additional copy of 
    \c ObjectT \c object compared to inplace \c operator \c |= */
template<class ObjectT>
ObjectT operator | (typename ObjectT::overloadable_type object, const ObjectT& operand)
{
    return object += operand; 
}


//==============================================================================
//= Subtraction
//==============================================================================

template<class ObjectT>
typename enable_if<is_interval_set<ObjectT>, ObjectT>::type&
subtract(ObjectT& object, const typename ObjectT::element_type& operand)
{
    detail::subtract(object, typename ObjectT::segment_type(operand));
    return object; //JODO: May be it is better to return the iterator
}

template<class ObjectT>
typename enable_if<is_interval_set<ObjectT>, ObjectT>::type&
subtract(ObjectT& object, const typename ObjectT::segment_type& operand)
{
    detail::subtract(object, operand);
    return object; //JODO: May be it is better to return the iterator
}

//------------------------------------------------------------------------------
//- Subtraction -=, -
//------------------------------------------------------------------------------

/** \par \b Requires: Types \c ObjectT and \c OperandT are subtractable.
    \par \b Effects: \c operand is subtracted from \c object.
    \par \b Returns: A reference to \c object.
    \b Complexity:
\code
                  \ OperandT:                      interval
                   \         element    segment    container
ObjectT:
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
template<class ObjectT, class OperandT>
typename enable_if<has_same_concept<is_interval_map, ObjectT, OperandT>, 
                   ObjectT>::type& 
operator -=(ObjectT& object, const OperandT& operand)
{
    ITL_const_FORALL(typename OperandT, elem_, operand) 
        object.subtract(*elem_); //JODO 

    return object; 
}

template<class ObjectT, class OperandT>
typename enable_if<is_intra_derivative<ObjectT, OperandT>, ObjectT>::type&
operator -= (ObjectT& object, const OperandT& operand)
{ 
    //JODO return itl::subtract(object, operand); 
    return object.subtract(operand); 
}

template<class ObjectT, class OperandT>
typename enable_if<is_cross_derivative<ObjectT, OperandT>, ObjectT>::type&
operator -= (ObjectT& object, const OperandT& operand)
{ 
    return object.erase(operand); 
}

template<class ObjectT, class IntervalSetT>
typename enable_if<combines_right_to_interval_set<ObjectT, IntervalSetT>,
                   ObjectT>::type&
operator -= (ObjectT& object, const IntervalSetT& operand)
{
    return erase(object, operand);
}


template<class ObjectT, class OperandT>
typename enable_if<is_right_inter_combinable<ObjectT, OperandT>, ObjectT>::type
operator - (ObjectT object, const OperandT& operand)
{
    return object -= operand; 
}

//==============================================================================
//= Insertion
//==============================================================================
template<class ObjectT, class OperandT>
typename enable_if<is_intra_combinable<ObjectT, OperandT>, ObjectT>::type&
insert(ObjectT& object, const OperandT& operand)
{
    typename ObjectT::iterator prior_ = object.end();
    ITL_const_FORALL(typename OperandT, elem_, operand) 
        object.insert(*elem_); 

    return object; 
}


//==============================================================================
//= Erasure
//==============================================================================
template<class ObjectT, class OperandT>
typename enable_if<combines_right_to_interval_container<ObjectT, OperandT>,
                   ObjectT>::type&
erase(ObjectT& object, const OperandT& operand)
{
    if(itl::is_empty(operand))
        return object;

    typename OperandT::const_iterator common_lwb;
    typename OperandT::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return object;

    typename OperandT::const_iterator it_ = common_lwb;
    while(it_ != common_upb)
        object.erase(*it_++);

    return object; 
}


//==============================================================================
//= Intersection
//==============================================================================
//------------------------------------------------------------------------------
//- Intersection add_intersection
//------------------------------------------------------------------------------
template<class ObjectT>
typename enable_if<is_interval_set<ObjectT>, ObjectT>::type&
add_intersection(ObjectT& section, const ObjectT& object, 
                          const typename ObjectT::domain_type& operand)
{
    typedef typename ObjectT::const_iterator const_iterator;
    const_iterator found = object.find(operand);
    if(found != object.end())
        itl::add(section, operand);

    return section;
}


template<class ObjectT>
typename enable_if<is_interval_set<ObjectT>, ObjectT>::type&
add_intersection(ObjectT& section, const ObjectT& object, 
                          const typename ObjectT::segment_type& segment)
{
    typedef typename ObjectT::const_iterator const_iterator;
    typedef typename ObjectT::iterator       iterator;
    typedef typename ObjectT::interval_type  interval_type;

    if(itl::is_empty(segment)) 
        return section;

    std::pair<const_iterator, const_iterator> exterior 
        = object.equal_range(segment);
    if(exterior.first == exterior.second)
        return section;

    iterator prior_ = section.end();
    for(const_iterator it_=exterior.first; it_ != exterior.second; it_++) 
    {
        interval_type common_interval = ObjectT::key_value(it_) & segment;
        if(!itl::is_empty(common_interval))
            prior_ = section._insert(prior_, common_interval);
    }
    return section;
}


template<class ObjectT, class OperandT>
typename enable_if<mpl::and_<is_interval_set<ObjectT>, 
                             combines_right_to_interval_set<ObjectT, OperandT> >,
                   ObjectT>::type&
add_intersection(ObjectT& section, const ObjectT& object, const OperandT& operand)
{
    typedef typename OperandT::const_iterator const_iterator;

    if(operand.empty())
        return section;

    const_iterator common_lwb, common_upb;
    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return section;

    const_iterator it_ = common_lwb;
    while(it_ != common_upb)
        itl::add_intersection(section, object, OperandT::key_value(it_++));

    return section;
}


//------------------------------------------------------------------------------
//- Intersection &=, &
//------------------------------------------------------------------------------
template<class ObjectT, class OperandT>
typename enable_if<is_right_inter_combinable<ObjectT, OperandT>, ObjectT>::type&
operator &= (ObjectT& object, const OperandT& operand)
{
    ObjectT intersection;
    object.add_intersection(intersection, operand);
    object.swap(intersection);
    return object;
}

template<class ObjectT, class OperandT>
typename enable_if<is_binary_inter_combinable<ObjectT, OperandT>, ObjectT>::type
operator & (ObjectT object, const OperandT& operand)
{
    return object &= operand; 
}

template<class ObjectT, class OperandT>
typename enable_if<is_binary_inter_combinable<ObjectT, OperandT>, ObjectT>::type
operator & (const OperandT& operand, ObjectT object)
{
    return object &= operand; 
}

template<class ObjectT>
ObjectT operator & (typename ObjectT::overloadable_type object, const ObjectT& operand)
{
    return object &= operand; 
}

//------------------------------------------------------------------------------
//- intersects
//------------------------------------------------------------------------------
template<class Type, class AssociateT>
typename enable_if<mpl::and_< is_interval_set<Type>
                            , is_same<AssociateT, domain_type_of<Type> > >, 
                   bool>::type
intersects(const Type& left, const AssociateT& right)
{
    return itl::contains(left, right); 
}

template<class Type, class AssociateT>
typename enable_if<mpl::and_< is_interval_set<Type>
                            , is_same<AssociateT, segment_type_of<Type> > >, 
                   bool>::type
intersects(const Type& left, const AssociateT& right)
{
    return left.find(right) != left.end();
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
        left.add_intersection(intersection, *it_++);
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
        left.add_intersection(intersection, RightT::key_value(it_++));
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
    return left.intersects(right);
}

/** \b Returns true, if \c left and \c right have no common elements.
    Intervals are interpreted as sequence of elements.
    \b Complexity: loglinear, if \c left and \c right are interval containers. */
template<class LeftT, class RightT>
typename enable_if<is_inter_combinable<LeftT, RightT>, 
                          bool>::type
is_disjoint(const LeftT& left, const RightT& right)
{
    return !intersects(left, right);
}

/** \b Returns true, if \c left and \c right have no common elements.
    Intervals are interpreted as sequence of elements.
    \b Complexity: logarithmic, if \c AssociateT is an element type \c Type::element_type. 
    linear, if \c AssociateT is a segment type \c Type::segment_type. */
template<class Type, class AssociateT>
typename enable_if<is_inter_derivative<Type, AssociateT>, 
                          bool>::type
is_disjoint(const Type& left, const AssociateT& right)
{
    return !left.intersects(right);
}


//==============================================================================
//= Symmetric difference
//==============================================================================
//------------------------------------------------------------------------------
//- Symmetric difference flip
//------------------------------------------------------------------------------

template<class ObjectT>
typename enable_if<is_interval_set<ObjectT>, ObjectT>::type&
flip(ObjectT& object, const typename ObjectT::domain_type& operand)
{
    if(itl::contains(object, operand))
        return object -= operand;
    else
        return object += operand;
}

//JODO MEMO: This did not compile, no idea why.
//template<class ObjectT, class OperandT>
//typename enable_if<mpl::and_< is_interval_set<ObjectT>
//                            , is_same<segment_type_of<ObjectT>, OperandT> >, 
//                          ObjectT>::type&
//flip(ObjectT& object, const OperandT& segment)

template<class ObjectT>
typename enable_if<is_interval_set<ObjectT>, ObjectT>::type&
flip(ObjectT& object, const typename ObjectT::segment_type& segment)
{
    typedef typename ObjectT::const_iterator const_iterator;
    typedef typename ObjectT::interval_type  interval_type;
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


template<class ObjectT, class OperandT>
typename enable_if<has_same_concept<is_interval_set, ObjectT, OperandT>, ObjectT>::type&
JODO_flip(ObjectT& object, const OperandT& operand)
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
//- Symmetric difference ^=, ^
//------------------------------------------------------------------------------
template<class ObjectT, class OperandT>
typename enable_if<is_intra_combinable<ObjectT, OperandT>, 
                          ObjectT>::type&
operator ^= (ObjectT& object, const OperandT& operand)
{ 
    return object.flip(operand); 
}

template<class ObjectT, class OperandT>
typename enable_if<is_intra_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator ^= (ObjectT& object, const OperandT& operand)
{ 
    return object.flip(operand); 
}

template<class ObjectT, class OperandT>
typename enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator ^ (ObjectT object, const OperandT& operand)
{
    return object ^= operand; 
}

template<class ObjectT, class OperandT>
typename enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator ^ (const OperandT& operand, ObjectT object)
{
    return object ^= operand; 
}


template<class ObjectT>
ObjectT operator ^ (typename ObjectT::overloadable_type object, const ObjectT& operand)
{
    return object ^= operand; 
}

//==============================================================================
//= Morphisms
//==============================================================================

template<class ObjectT>
typename enable_if<is_interval_set<ObjectT>, ObjectT>::type&
join(ObjectT& object)
{
    typedef typename ObjectT::interval_type interval_type;
    typedef typename ObjectT::iterator      iterator;

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


