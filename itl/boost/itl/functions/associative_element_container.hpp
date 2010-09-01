/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_FUNCTIONS_ASSOCIATIVE_ELEMENT_CONTAINER_HPP_JOFA_100831
#define BOOST_ITL_FUNCTIONS_ASSOCIATIVE_ELEMENT_CONTAINER_HPP_JOFA_100831

#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/type_traits/is_key_container_of.hpp>
#include <boost/itl/type_traits/is_concept_equivalent.hpp>
#include <boost/itl/type_traits/is_container.hpp>
#include <boost/itl/type_traits/is_associative_element_container.hpp>

namespace boost{namespace itl
{

//JODO Declaration forwarding for gcc-3.4.4
template <class SetT>
typename enable_if<is_element_set<SetT>, SetT>::type&
add(SetT& object, const typename SetT::element_type& operand);


//==============================================================================
//= Containedness
//==============================================================================

//------------------------------------------------------------------------------
// within
//------------------------------------------------------------------------------
/** Checks if a key is in the associative container */
template<class Type>
typename enable_if<is_associative_element_container<Type>, bool>::type
within(const typename Type::domain_type& key, const Type& super)
{ 
    return !(super.find(key) == super.end()); 
}

template<class SubT, class SuperT>
typename enable_if<mpl::and_< is_associative_element_container<SuperT> 
                            , is_key_container_of<SubT, SuperT> >,
                   bool>::type
within(const SubT& sub, const SuperT& super)
{
    if(itl::is_empty(sub))                return true;
    if(itl::is_empty(super))              return false;
    if(itl::size(super) < itl::size(sub)) return false;

    typename SubT::const_iterator common_lwb_;
    typename SubT::const_iterator common_upb_;
    if(!Set::common_range(common_lwb_, common_upb_, sub, super))
        return false;

    typename SubT::const_iterator sub_ = sub.begin();
    typename SuperT::const_iterator super_;
    while(sub_ != sub.end())
    {
        super_ = super.find(SubT::key_value(sub_));
        if(super_ == super.end()) 
            return false;
        else if(!co_equal(sub_, super_, sub, super))
            return false;

        ++sub_;
    }
    return true;
}


//------------------------------------------------------------------------------
// contains
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_associative_element_container<Type>, bool>::type
contains(const Type& super, const typename Type::domain_type& key)
{ 
    return itl::within(key, super); 
}

template<class SubT, class SuperT>
typename enable_if<mpl::and_< is_associative_element_container<SuperT>
                            , is_key_container_of<SubT, SuperT> >,
                   bool>::type
contains(const SuperT& super, const SubT& sub)
{ 
    return itl::within(sub, super); 
}


//==============================================================================
//= Equivalences and Orderings
//==============================================================================

#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4996) //'std::equal': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
#endif                        // I do guarantee here that I am using the parameters correctly :)

/** Standard equality, which is lexicographical equality of the sets
    as sequences, that are given by their Compare order. */
template<class Type>
inline typename enable_if<is_associative_element_container<Type>, bool>::type
operator == (const Type& left, const Type& right)
{
    return left.size() == right.size() 
        && std::equal(left.begin(), left.end(), right.begin());
}

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

template<class Type>
inline typename enable_if<is_associative_element_container<Type>, bool>::type
is_element_equal(const Type& left, const Type& right)
{ return left == right; }


/* Strict weak less ordering which is given by the Compare order */
template<class Type>
inline typename enable_if<is_associative_element_container<Type>, bool>::type
operator < (const Type& left, const Type& right)
{
    return std::lexicographical_compare(
        left.begin(), left.end(), right.begin(), right.end(), 
        typename Type::element_compare()
        );
}


//==============================================================================
//= Addition
//==============================================================================
template <class Type>
inline typename enable_if<is_associative_element_container<Type>, Type>::type&
operator += (Type& object, const typename Type::element_type& operand) 
{ 
    return itl::add(object, operand); 
}

template <class Type>
inline typename enable_if<is_associative_element_container<Type>, Type>::type
operator + (Type object, const typename Type::element_type& operand) 
{ 
    return object += operand; 
}

template <class Type>
inline typename enable_if<is_associative_element_container<Type>, Type>::type
operator + (const typename Type::element_type& operand, Type object) 
{ 
    return object += operand; 
}

template <class Type>
inline typename enable_if<is_associative_element_container<Type>, Type>::type
operator += (Type& object, const Type& operand) 
{ 
    return Set::add(object, operand); 
}

template <class Type>
inline typename enable_if<is_associative_element_container<Type>, Type>::type
operator + (Type object, const Type& operand) 
{ 
    return object += operand; 
}

//==============================================================================
template <class Type>
inline typename enable_if<is_associative_element_container<Type>, Type>::type&
operator |= (Type& object, const typename Type::element_type& operand) 
{ 
    return itl::add(object, operand); 
}

template <class Type>
inline typename enable_if<is_associative_element_container<Type>, Type>::type
operator | (Type object, const typename Type::element_type& operand) 
{ 
    return object += operand; 
}

template <class Type>
inline typename enable_if<is_associative_element_container<Type>, Type>::type
operator | (const typename Type::element_type& operand, Type object) 
{ 
    return object += operand; 
}

template <class Type>
inline typename enable_if<is_associative_element_container<Type>, Type>::type&
operator |= (Type& object, const Type& operand) 
{ 
    return Set::add(object, operand); 
}

template <class Type>
inline typename enable_if<is_associative_element_container<Type>, Type>::type
operator | (Type object, const Type& operand) 
{ 
    return object += operand; 
}



}} // namespace itl boost


#endif


