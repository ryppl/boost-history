/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_SET_FUNCTIONS_HPP_JOFA_100827
#define BOOST_ITL_SET_FUNCTIONS_HPP_JOFA_100827

#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/type_traits/is_element_container.hpp>
#include <boost/itl/detail/map_algo.hpp>
#include <boost/itl/detail/map_functors.hpp>

#include <boost/itl/functions/container.hpp>
#include <boost/itl/functions/associative_element_container.hpp>



namespace boost{namespace itl
{

//------------------------------------------------------------------------------


//==============================================================================
//= Addition
//==============================================================================
/** \c add inserts \c operand into the map if it's key does 
    not exist in the map.    
    If \c operands's key value exists in the map, it's data
    value is added to the data value already found in the map. */
template <class SetT>
typename enable_if<is_element_set<SetT>, SetT>::type&
add(SetT& object, const typename SetT::element_type& operand)
{
    object.insert(operand); 
    return object;
}

/** \c add add \c operand into the map using \c prior as a hint to
    insert \c operand after the position \c prior is pointing to. */
template <class SetT>
typename enable_if<is_element_set<SetT>, typename SetT::iterator>::type
add(SetT& object, typename SetT::iterator prior, 
    const typename SetT::element_type& operand) 
{ 
    return object.insert(prior, operand); 
}


//==============================================================================
//= Subtraction
//==============================================================================
/** If the \c operand's key value is in the map, it's data value is
    subtraced from the data value stored in the map. */
template <class SetT>
typename enable_if<is_element_set<SetT>, SetT>::type&
subtract(SetT& object, const typename SetT::element_type& operand)
{
    typedef typename SetT::iterator iterator;
    iterator it_ = object.find(operand);
    if(it_ != object.end())
        object.erase(it_);

    return object;
}

template <class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
operator -= (SetT& object, const typename SetT::element_type& operand) 
{ 
    return itl::subtract(object, operand); 
}

template <class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type
operator - (SetT object, const typename SetT::element_type& operand) 
{ 
    return object -= operand; 
}

template <class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
operator -= (SetT& object, const SetT& subtrahend) 
{ 
    ITL_const_FORALL(typename SetT, it_, subtrahend)
        itl::subtract(object, *it_);

    return object; 
}

template <class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type
operator - (SetT object, const SetT& subtrahend) 
{ 
    return object -= subtrahend; 
}

template <class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
operator -= (SetT& object, const typename SetT::set_type& subtrahend) 
{ 
    return Set::erase(object, subtrahend); 
}

template <class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type
operator - (SetT object, const typename SetT::set_type& subtrahend) 
{ 
    return object -= subtrahend; 
}



//==============================================================================
//= Insertion
//==============================================================================

template<class SetT>
typename enable_if<is_element_set<SetT>, 
                   std::pair<typename SetT::iterator,bool> >::type
insert(SetT& object, const typename SetT::element_type& operand)
{
    return object.insert(operand);
}

template<class SetT>
typename enable_if<is_element_set<SetT>, 
                   typename SetT::iterator>::type
insert(SetT& object, typename SetT::iterator prior, const typename SetT::element_type& operand)
{
    return object.insert(prior, operand);
}

template<class SetT>
typename enable_if<is_element_set<SetT>, SetT>::type&
insert(SetT& object, const SetT& addend)
{
    typedef typename SetT::iterator iterator;

    iterator prior_ = object.end();
    ITL_const_FORALL(typename SetT, elem_, addend) 
        itl::insert(object, prior_, *elem_);

    return object; 
}

//==============================================================================
//= Erasure
//==============================================================================

template<class SetT>
typename enable_if<is_element_set<SetT>, typename SetT::size_type>::type
erase(SetT& object, const typename SetT::domain_type& operand)
{
    typedef typename SetT::iterator iterator;
    iterator it_ = object.find(operand);
    if(it_ != object.end())
    {
        object.erase(it_);
        return 1;
    }

    return 0;
}

template<class SetT>
typename enable_if<is_element_set<SetT>, SetT>::type&
erase(SetT& object, const typename SetT::set_type& erasure)
{
    typedef typename SetT::set_type set_type;
    ITL_const_FORALL(typename set_type, elem_, erasure) 
        itl::erase(object, *elem_); 

    return object; 
}

template<class SetT>
typename enable_if<is_element_set<SetT>, SetT>::type&
erase(SetT& object, const SetT& erasure)
{
    ITL_const_FORALL(typename SetT, elem_, erasure) 
        itl::erase(object, *elem_); 

    return object; 
}



//==============================================================================
//= Intersection
//==============================================================================


template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
add_intersection(SetT& section, const SetT& object, const typename SetT::domain_type& operand)
{
    typedef typename SetT::const_iterator const_iterator;
    const_iterator it_ = object.find(operand);
    if(it_ != object.end()) 
        itl::add(section, *it_);

    return section;
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
add_intersection(SetT& section, const SetT& object, const SetT& operand)
{
    typedef typename SetT::const_iterator const_iterator;
    const_iterator common_lwb_;
    const_iterator common_upb_;
    if(!Set::common_range(common_lwb_, common_upb_, operand, object))
        return section;

    const_iterator sec_ = common_lwb_;
    while(sec_ != common_upb_)
        add_intersection(section, object, *sec_++);

    return section;
}


/** Intersect map \c x2 and \c *this.
    So \c *this becomes the intersection of \c *this and \c x2 */
template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
operator &= (SetT& object, const SetT& operand)
{
    SetT section;
    itl::add_intersection(section, object, operand);
    object.swap(section);
    return object; 
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type
operator & (SetT object, const SetT& operand)
{
    return object &= operand;
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
operator &= (SetT& object, const typename SetT::element_type& operand) //JODO codereplica.
{
    SetT section;
    itl::add_intersection(section, object, operand);
    object.swap(section);
    return object; 
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type
operator & (SetT object, const typename SetT::element_type& operand)
{
    return object &= operand;
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type
operator & (const typename SetT::element_type& operand, SetT object)
{
    return object &= operand;
}


/** Intersect set \c x2 and \c *this.
    So \c *this becomes the intersection of \c *this and \c x2 */
template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
operator &= (SetT& object, const typename SetT::set_type& operand)
{ 
    SetT section;
    add_intersection(section, object, operand);
    object.swap(section);
    return object;
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type
operator & (SetT object, const typename SetT::set_type& key_set)
{
    return object &= key_set; //JODO test  a - (a & s) == a - s
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type
operator & (const typename SetT::set_type& key_set, SetT object)
{
    return object &= key_set; //JODO test  a - (s & a) == a - s
}


//------------------------------------------------------------------------------
//- intersects
//------------------------------------------------------------------------------

template<class SetT>
inline typename enable_if<is_element_set<SetT>, bool>::type
intersects(const SetT& object, const typename SetT::domain_type& operand)
{
    return itl::contains(object, operand);
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, bool>::type
intersects(const typename SetT::domain_type& operand, const SetT& object)
{
    return itl::contains(object, operand);
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, bool>::type
intersects(const SetT& object, const SetT& operand)
{
    if(iterative_size(object) < iterative_size(operand))
        return Set::intersects(object, operand);
    else
        return Set::intersects(operand, object);
}


template<class SetT>
inline typename enable_if<is_element_set<SetT>, bool>::type
disjoint(const SetT& left, const SetT& right)//JODO All variants via meta predicate.
{
    return !intersects(left, right);
}



//==============================================================================
//= Symmetric difference
//==============================================================================

template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
flip(SetT& object, const typename SetT::element_type& operand)
{
    typedef typename SetT::iterator iterator;
    std::pair<iterator,bool> insertion = object.insert(operand);
    if(!insertion.second)
        object.erase(insertion.first);

    return object;
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
operator ^= (SetT& object, const typename SetT::element_tpye& operand)
{
    return itl::flip(object, operand); 
}

/** Symmetric subtract map \c x2 and \c *this.
    So \c *this becomes the symmetric difference of \c *this and \c x2 */
template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
operator ^= (SetT& object, const SetT& operand)
{
    typedef typename SetT::const_iterator const_iterator;
    const_iterator it_ = operand.begin();
    while(it_ != operand.end())
        itl::flip(object, *it_++);

    return object;
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type
operator ^ (SetT object, const SetT& operand)
{
    return object ^= operand; 
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type
operator ^ (SetT object, const typename SetT::element_type& operand)
{
    return itl::flip(object, operand); 
}

template<class SetT>
inline typename enable_if<is_element_set<SetT>, SetT>::type
operator ^ (const typename SetT::element_type& operand, SetT object)
{
    return itl::flip(object, operand); 
}

//==============================================================================
//= Manipulation by predicates
//==============================================================================

template<class SetT, class Predicate>
typename enable_if<is_element_set<SetT>, SetT>::type&
erase_if(const Predicate& pred, SetT& object)
{
    typename SetT::iterator it_ = object.begin();
    while(it_ != object.end())
        if(pred(*it_))
            itl::erase(object, it_++); 
        else ++it_;
    return object;
}


template<class SetT, class Predicate>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
add_if(const Predicate& pred, SetT& object, const SetT& src)
{
    typename SetT::const_iterator it_ = src.begin();
    while(it_ != src.end())
        if(pred(*it_)) 
            itl::add(object, *it_++); 
    
    return object;
}

template<class SetT, class Predicate>
inline typename enable_if<is_element_set<SetT>, SetT>::type&
assign_if(const Predicate& pred, SetT& object, const SetT& src)
{
    itl::clear(object);
    return add_if(object, src, pred);
}


//---------------------------------------------------------------------------------
template<class CharType, class CharTraits, class SetT>
inline typename enable_if<is_element_set<SetT>, std::basic_ostream<CharType, CharTraits> >::type&
operator << (std::basic_ostream<CharType, CharTraits>& stream, const SetT& object)
{
    stream << "{";
    ITL_const_FORALL(typename SetT, it, object)
        stream << *it << " ";

    return stream << "}";
}


}} // namespace itl boost

#endif // BOOST_ITL_SET_FUNCTIONS_HPP_JOFA_100827

