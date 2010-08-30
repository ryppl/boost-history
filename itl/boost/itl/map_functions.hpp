/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_MAP_FUNCTIONS_HPP_JOFA_100824
#define BOOST_ITL_MAP_FUNCTIONS_HPP_JOFA_100824

#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/detail/map_algo.hpp>
#include <boost/itl/detail/map_functors.hpp>
#include <boost/itl/type_traits/is_element_container.hpp>
#include <boost/itl/type_traits/is_key_container_of.hpp>

namespace boost{namespace itl
{

//==============================================================================
//= Containedness
//==============================================================================

/** All content of the container is dropped. 
    Complexity: linear. */
template<class MapT>
typename enable_if<is_element_map<MapT>, void>::type
clear(MapT& object)
{
    object.clear();
}

/** Tests if the container is empty. 
    Complexity: constant. */
template<class MapT>
typename enable_if<is_element_map<MapT>, bool>::type
is_empty(const MapT& object)
{
    return object.empty();
}

/** Checks if a key is in the map */
template<class MapT>
typename enable_if<is_element_map<MapT>, bool>::type
contains(const MapT& object, const typename MapT::domain_type& key)
{ 
    return !(object.find(key) == object.end()); 
}

/** Checks if a key-value pair is in the map */
template<class MapT>
typename enable_if<is_element_map<MapT>, bool>::type
contains(const MapT& object, const typename MapT::element_type& key_value_pair)
{ 
    typedef typename MapT::const_iterator const_iterator;
    const_iterator found_ = object.find(key_value_pair.first);
    return found_ != object.end() && found_->second == key_value_pair.second;
}

/** Does <tt>super</tt> contain <tt>sub</tt>? */
template<class MapT>
typename enable_if<is_element_map<MapT>, bool>::type
contains(const MapT& super, const MapT& sub)
{ return itl::within(sub, super); }

/** Does <tt>super</tt> contain <tt>sub</tt>? */
template<class MapT>
typename enable_if<is_element_map<MapT>, bool>::type
contains(const MapT& super, const typename MapT::set_type& sub)
{ 
    return itl::within(sub, super); 
}

//- within ---------------------------------------------------------------------


template<class SubT, class SuperT>
typename enable_if<mpl::and_< is_element_map<SuperT> 
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



/** Is <tt>sub</tt> contained within <tt>super</tt>? */
template<class MapT>
typename enable_if<is_element_map<MapT>, bool>::type
within(const typename MapT::domain_type& sub, const MapT& super)
{ return contains(super, sub); }

/** Is <tt>sub</tt> contained within <tt>super</tt>? */
template<class MapT>
typename enable_if<is_element_map<MapT>, bool>::type
within(const typename MapT::element_type& sub, const MapT& super)
{ return contains(super, sub); }

//==============================================================================
//= Size
//==============================================================================
template<class MapT>
typename enable_if<is_element_map<MapT>, typename MapT::size_type>::type
size(const MapT& object)
{ 
    return object.size(); 
}

template<class MapT>
typename enable_if<is_element_map<MapT>, typename MapT::size_type>::type
cardinality(const MapT& object)
{ 
    return object.size(); 
}

template<class MapT>
typename enable_if<is_element_map<MapT>, typename MapT::size_type>::type
iterative_size(const MapT& object)
{ 
    return object.size(); 
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
template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
operator == (const MapT& lhs, const MapT& rhs)
{
    return lhs.size() == rhs.size()
        && equal(lhs.begin(), lhs.end(), rhs.begin());
}

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
operator != (const MapT& lhs, const MapT& rhs)
{ return !(lhs == rhs); }

template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
is_element_equal(const MapT& lhs, const MapT& rhs)
{ return lhs == rhs; }

/** Protonic equality is equality on all elements that do not carry a neutron as content. */
template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
is_protonic_equal(const MapT& lhs, const MapT& rhs)
{
    return Map::lexicographical_protonic_equal(lhs, rhs);
}

/** Strict weak less ordering which is given by the Compare order */
template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
operator < (const MapT& lhs, const MapT& rhs)
{
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), 
        typename MapT::element_compare()
        //lhs.value_comp() //JODO why does this implementation violate antisymmetry?
        );
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
operator > (const MapT& lhs, const MapT& rhs)
{ return rhs < lhs; }

/** Partial ordering which is induced by Compare */
template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
operator <= (const MapT& lhs, const MapT& rhs)
{ return !(lhs > rhs); }

template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
operator >= (const MapT& lhs, const MapT& rhs)
{ return !(lhs < rhs); }

//------------------------------------------------------------------------------


//==============================================================================
//= Addition
//==============================================================================
/** \c add inserts \c value_pair into the map if it's key does 
    not exist in the map.    
    If \c value_pairs's key value exists in the map, it's data
    value is added to the data value already found in the map. */
template <class MapT>
typename enable_if<is_element_map<MapT>, MapT>::type&
add(MapT& object, const typename MapT::value_type& value_pair)
{
    return MapT::add_::apply(object, value_pair); 
}

/** \c add add \c value_pair into the map using \c prior as a hint to
    insert \c value_pair after the position \c prior is pointing to. */
template <class MapT>
typename enable_if<is_element_map<MapT>, typename MapT::iterator>::type
add(MapT& object, typename MapT::iterator prior, 
    const typename MapT::value_type& value_pair) 
{ 
    return MapT::add_::apply(object, prior, value_pair); 
}


template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
operator += (MapT& object, const typename MapT::element_type& value_pair) 
{ 
    return itl::add(object, value_pair); 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator + (MapT object, const typename MapT::element_type& value_pair) 
{ 
    return object += value_pair; 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator + (const typename MapT::element_type& value_pair, MapT object) 
{ 
    return object += value_pair; 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
operator += (MapT& object, const MapT& addend) 
{ 
    return Set::add(object, addend); 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator + (MapT object, const MapT& addend) 
{ 
    return object += addend; 
}

//==============================================================================

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
operator |= (MapT& object, const typename MapT::element_type& value_pair) 
{ 
    return itl::add(object, value_pair); 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator | (MapT object, const typename MapT::element_type& value_pair) 
{ 
    return object += value_pair; 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator | (const typename MapT::element_type& value_pair, MapT object) 
{ 
    return object += value_pair; 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
operator |= (MapT& object, const MapT& addend) 
{ 
    return Set::add(object, addend); 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator | (MapT object, const MapT& addend) 
{ 
    return object += addend; 
}




//==============================================================================
//= Subtraction
//==============================================================================
/** If the \c value_pair's key value is in the map, it's data value is
    subtraced from the data value stored in the map. */
template <class MapT>
typename enable_if<is_element_map<MapT>, MapT>::type&
subtract(MapT& object, const typename MapT::element_type& value_pair)
{
    return MapT::subtract_::apply(object, value_pair);
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
operator -= (MapT& object, const typename MapT::element_type& value_pair) 
{ 
    return itl::subtract(object, value_pair); 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator - (MapT object, const typename MapT::element_type& value_pair) 
{ 
    return object -= value_pair; 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
operator -= (MapT& object, const MapT& subtrahend) 
{ 
    ITL_const_FORALL(typename MapT, it_, subtrahend)
        itl::subtract(object, *it_);

    return object; 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator - (MapT object, const MapT& subtrahend) 
{ 
    return object -= subtrahend; 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
operator -= (MapT& object, const typename MapT::set_type& subtrahend) 
{ 
    return Set::erase(object, subtrahend); 
}

template <class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator - (MapT object, const typename MapT::set_type& subtrahend) 
{ 
    return object -= subtrahend; 
}



//==============================================================================
//= Insertion
//==============================================================================

template<class MapT>
typename enable_if<is_element_map<MapT>, 
                   std::pair<typename MapT::iterator,bool> >::type
insert(MapT& object, const typename MapT::element_type& value_pair)
{
    typedef typename MapT::insert_ map_insert;
    return map_insert::apply(object, value_pair);
}

template<class MapT>
typename enable_if<is_element_map<MapT>, 
                   typename MapT::iterator>::type
insert(MapT& object, typename MapT::iterator prior, const typename MapT::element_type& value_pair)
{
    return MapT::insert_::apply(object, prior, value_pair);
}

template<class MapT>
typename enable_if<is_element_map<MapT>, MapT>::type&
insert(MapT& object, const MapT& addend)
{
    typedef typename MapT::iterator iterator;

    iterator prior_ = object.end();
    ITL_const_FORALL(typename MapT, elem_, addend) 
        itl::insert(object, prior_, *elem_);

    return object; 
}

//==============================================================================
//= Erasure
//==============================================================================

template<class MapT>
typename enable_if<is_element_map<MapT>, typename MapT::size_type>::type
erase(MapT& object, const typename MapT::domain_type& key_value)
{
    return MapT::erase_::apply(object, key_value);
}

template<class MapT>
typename enable_if<is_element_map<MapT>, typename MapT::size_type>::type
erase(MapT& object, const typename MapT::element_type& value_pair)
{
    return MapT::erase_::apply(object, value_pair);
}

template<class MapT>
typename enable_if<is_element_map<MapT>, MapT>::type&
erase(MapT& object, const typename MapT::set_type& erasure)
{
    typedef typename MapT::set_type set_type;
    ITL_const_FORALL(typename set_type, elem_, erasure) 
        itl::erase(object, *elem_); 

    return object; 
}

template<class MapT>
typename enable_if<is_element_map<MapT>, MapT>::type&
erase(MapT& object, const MapT& erasure)
{
    ITL_const_FORALL(typename MapT, elem_, erasure) 
        itl::erase(object, *elem_); 

    return object; 
}



//==============================================================================
//= Intersection
//==============================================================================


template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
add_intersection(MapT& section, const MapT& object, const typename MapT::domain_type& key_value)
{
    return MapT::add_intersection_::apply(section, object, key_value);
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
add_intersection(MapT& section, const MapT& object, const typename MapT::element_type& value_pair)
{
    return MapT::add_intersection_::apply(section, object, value_pair);
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
add_intersection(MapT& section, const MapT& object, const typename MapT::set_type& key_set)
{
    typedef typename MapT::set_type set_type;
    typedef typename set_type::const_iterator const_iterator;

    const_iterator common_lwb_;
    const_iterator common_upb_;
    if(!Set::common_range(common_lwb_, common_upb_, key_set, object))
        return section;

    const_iterator sec_ = common_lwb_;
    while(sec_ != common_upb_)
        itl::add_intersection(section, object, *sec_++);

    return section;
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
add_intersection(MapT& section, const MapT& object, const MapT& operand)
{
    return MapT::add_intersection_::apply(section, object, operand);
}


/** Intersect map \c x2 and \c *this.
    So \c *this becomes the intersection of \c *this and \c x2 */
template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
operator &= (MapT& object, const MapT& operand)
{
    return MapT::inplace_intersect_::apply(object, operand);
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator & (MapT object, const MapT& operand)
{
    return object &= operand;
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
operator &= (MapT& object, const typename MapT::element_type& operand) //JODO codereplica.
{
    return MapT::inplace_intersect_::apply(object, operand);
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator & (MapT object, const typename MapT::element_type& value_pair)
{
    return object &= value_pair;
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator & (const typename MapT::element_type& value_pair, MapT object)
{
    return object &= value_pair;
}


/** Intersect set \c x2 and \c *this.
    So \c *this becomes the intersection of \c *this and \c x2 */
template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
operator &= (MapT& object, const typename MapT::set_type& operand)
{ 
    MapT section;
    add_intersection(section, object, operand);
    object.swap(section);
    return object;
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator & (MapT object, const typename MapT::set_type& key_set)
{
    return object &= key_set; //JODO test  a - (a & s) == a - s
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator & (const typename MapT::set_type& key_set, MapT object)
{
    return object &= key_set; //JODO test  a - (s & a) == a - s
}


//------------------------------------------------------------------------------
//- intersects
//------------------------------------------------------------------------------

template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
intersects(const MapT& object, const typename MapT::domain_type& operand)
{
    return MapT::intersects_::apply(object, operand);
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
intersects(const typename MapT::domain_type& operand, const MapT& object)
{
    return MapT::intersects_::apply(object, operand);
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
intersects(const MapT& object, const typename MapT::element_type& operand)
{
    return MapT::intersects_::apply(object, operand);
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
intersects(const typename MapT::element_type& operand, const MapT& object)
{
    return MapT::intersects_::apply(object, operand);
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
intersects(const MapT& object, const typename MapT::set_type& operand)
{
    return MapT::intersects_::apply(object, operand);
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
intersects(const typename MapT::set_type& operand, const MapT& object)
{
    return MapT::intersects_::apply(object, operand);
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
intersects(const MapT& object, const MapT& operand)
{
    return MapT::intersects_::apply(object, operand);
}


template<class MapT>
inline typename enable_if<is_element_map<MapT>, bool>::type
disjoint(const MapT& left, const MapT& right)//JODO All variants via meta predicate.
{
    return !intersects(left, right);
}



//==============================================================================
//= Symmetric difference
//==============================================================================

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
operator ^= (MapT& object, const typename MapT::element_tpye& operand)
{
    return MapT::flip_::apply(object, operand); 
}


/** Symmetric subtract map \c x2 and \c *this.
    So \c *this becomes the symmetric difference of \c *this and \c x2 */
template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
operator ^= (MapT& object, const MapT& operand)
{
    return MapT::flip_::apply(object, operand); 
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator ^ (MapT object, const MapT& operand)
{
    return object ^= operand; 
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator ^ (MapT object, const typename MapT::element_type& value_pair)
{
    MapT operand;//JODO
    operand.insert(value_pair);
    return object ^= operand; 
}

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type
operator ^ (const typename MapT::element_type& value_pair, MapT object)
{
    MapT operand;//JODO
    operand.insert(value_pair);
    return object ^= operand; 
}

//==============================================================================
//= Set selection
//==============================================================================
template<class MapT>
inline typename enable_if<is_element_map<MapT>, 
                          typename MapT::set_type>::type&
domain(typename MapT::set_type& domain_set, const MapT& object)
{
    typename MapT::set_type::iterator prior_ = domain_set.end();
    typename MapT::const_iterator it_ = object.begin();
    while(it_ != object.end())
        prior_ = domain_set.insert(prior_, (*it_++).first);
        //JODO prior_ = itl::insert(domain_set, prior_, it_->first);

    return domain_set;
}


//==============================================================================
//= Manipulation by predicates
//==============================================================================

template<class MapT, class Predicate>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
erase_if(const Predicate& pred, MapT& object)
{
    typename MapT::iterator it_ = object.begin();
    while(it_ != object.end())
        if(pred(*it_))
            itl::erase(object, it_++); 
        else ++it_;
    return object;
}


template<class MapT, class Predicate>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
add_if(const Predicate& pred, MapT& object, const MapT& src)
{
    typename MapT::const_iterator it_ = src.begin();
    while(it_ != src.end())
        if(pred(*it_)) 
            itl::add(object, *it_++); 
    
    return object;
}

template<class MapT, class Predicate>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
assign_if(const Predicate& pred, MapT& object, const MapT& src)
{
    itl::clear(object);
    return add_if(object, src, pred);
}

//==============================================================================
//= Neutron absorbtion
//==============================================================================

template<class MapT>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
absorb_neutrons(MapT& object)
{
    return map_absorb_neutrons<MapT,absorbs_neutrons<MapT>::value>::apply(object);
}


//---------------------------------------------------------------------------------
template<class CharType, class CharTraits, class MapT>
inline typename enable_if<is_element_map<MapT>, std::basic_ostream<CharType, CharTraits> >::type&
operator << (std::basic_ostream<CharType, CharTraits>& stream, const MapT& object)
{
    stream << "{";
    ITL_const_FORALL(typename MapT, it, object)
        stream << "(" << it->first << "->" << it->second << ")";

    return stream << "}";
}


}} // namespace itl boost

#endif // BOOST_ITL_MAP_FUNCTIONS_HPP_JOFA_100824

