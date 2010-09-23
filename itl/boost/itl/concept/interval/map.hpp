/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_CONCEPT_INTERVAL_MAP_HPP_JOFA_100920
#define BOOST_ITL_CONCEPT_INTERVAL_MAP_HPP_JOFA_100920

#include <boost/itl/type_traits/element_type_of.hpp>
#include <boost/itl/type_traits/segment_type_of.hpp>
#include <boost/itl/type_traits/absorbs_neutrons.hpp>
#include <boost/itl/type_traits/is_combinable.hpp>

namespace boost{ namespace itl
{

//==============================================================================
//= Containedness<IntervalMap>
//==============================================================================
//------------------------------------------------------------------------------
//- bool contains(c T&, c P&) T:{M} P:{b p M} fragment_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, bool>::type
contains(const Type& super, const typename Type::element_type& key_value_pair)
{
    typedef typename Type::const_iterator const_iterator;
    const_iterator it_ = super.find(key_value_pair.key);
    return it_ != super.end() && it_->second == key_value_pair.data;
}

template<class Type>
typename enable_if<is_interval_map<Type>, bool>::type
contains(const Type& super, const typename Type::segment_type& sub_segment)
{
    typedef typename Type::interval_type  interval_type;
    typedef typename Type::const_iterator const_iterator;

    interval_type sub_interval = sub_segment.first;
    if(itl::is_empty(sub_interval)) 
        return true;

    std::pair<const_iterator, const_iterator> exterior = super.equal_range(sub_interval);
    if(exterior.first == exterior.second)
        return false;

    const_iterator last_overlap = prior(exterior.second);

    if(!(sub_segment.second == exterior.first->second) )
        return false;

    return
          itl::contains(hull(exterior.first->first, last_overlap->first), sub_interval)
      &&  Interval_Map::is_joinable(super, exterior.first, last_overlap);
}

template<class Type, class CoType>
typename enable_if<is_concept_compatible<is_interval_map, Type, CoType>, bool>::type
contains(const Type& super, const CoType& sub)
{
    return Interval_Set::within(sub, super);
}


//------------------------------------------------------------------------------
//- bool contains(c T&, c P&) T:{M} P:{e i S} key_types : total
//------------------------------------------------------------------------------
template<class Type, class CoType>
typename enable_if< mpl::and_< is_interval_map<Type>
                             , is_total<Type> 
                             , is_cross_derivative<Type, CoType> >
            , bool>::type
contains(const Type& super, const CoType& sub)
{
	return true;
}

//------------------------------------------------------------------------------
//- bool contains(c T&, c P&) T:{M} P:{e i S} key_types : partial
//------------------------------------------------------------------------------
template<class Type>
typename enable_if< mpl::and_< is_interval_map<Type>
                             , mpl::not_<is_total<Type> > >
            , bool>::type
contains(const Type& super, const typename Type::domain_type& key)    
{
    return super.find(key) != super.end();
}

template<class Type>
typename enable_if< mpl::and_< is_interval_map<Type>
                             , mpl::not_<is_total<Type> > >
            , bool>::type
contains(const Type& super, const typename Type::interval_type& sub_interval)
{
    typedef typename Type::const_iterator const_iterator;

    if(itl::is_empty(sub_interval)) 
        return true;

    std::pair<const_iterator, const_iterator> exterior = super.equal_range(sub_interval);
    if(exterior.first == exterior.second)
        return false;

    const_iterator last_overlap = prior(exterior.second);

    return
          itl::contains(hull(exterior.first->first, last_overlap->first), sub_interval)
      &&  Interval_Set::is_joinable(super, exterior.first, last_overlap);
}

template<class Type, class KeyT>
typename enable_if< mpl::and_< is_concept_combinable<is_interval_map, is_interval_set, Type, KeyT>
                             , mpl::not_<is_total<Type> > >
            , bool>::type
contains(const Type& super, const KeyT& sub)
{
    return Interval_Set::within(sub, super);
}


//==============================================================================
//= Addition<IntervalMap>
//==============================================================================
//------------------------------------------------------------------------------
//- T& add(T&, c P&) T:{M} P:{b p} fragment_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
add(Type& object, const typename Type::element_type& operand)
{
    return object.add(operand);
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
add(Type& object, const typename Type::segment_type& operand)
{
    return object.add(operand);
}

//------------------------------------------------------------------------------
//- T& add(T&, J, c P&) T:{M} P:{p} segment_type
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, typename Type::iterator >::type
add(Type& object, typename Type::iterator      prior_,
            const typename Type::segment_type& operand)
{
    return object.add(prior_, operand);
}

//==============================================================================
//= Insertion<IntervalMap>
//==============================================================================
//------------------------------------------------------------------------------
//- T& insert(T&, c P&) T:{M} P:{b p} fragment_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
insert(Type& object, const typename Type::element_type& operand)
{
    return object.insert(operand);
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
insert(Type& object, const typename Type::segment_type& operand)
{
    return object.insert(operand);
}

//------------------------------------------------------------------------------
//- T& insert(T&, J, c P&) T:{M} P:{p} with hint
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, typename Type::iterator>::type
insert(Type& object, typename Type::iterator      prior,
               const typename Type::segment_type& operand)
{
    return object.insert(prior, operand);
}


//==============================================================================
//= Erasure<IntervalMap>
//==============================================================================
//------------------------------------------------------------------------------
//- T& erase(T&, c P&) T:{M} P:{e i} key_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
erase(Type& object, const typename Type::domain_type& minuend)
{
    return object.erase(minuend);
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
erase(Type& object, const typename Type::interval_type& minuend)
{
    return object.erase(minuend);
}

//------------------------------------------------------------------------------
//- T& erase(T&, c P&) T:{M} P:{b p} fragment_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
erase(Type& object, const typename Type::element_type& minuend)
{
    return object.erase(minuend);
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
erase(Type& object, const typename Type::segment_type& minuend)
{
    return object.erase(minuend);
}

//==============================================================================
//= Subtraction<IntervalMap>
//==============================================================================
//------------------------------------------------------------------------------
//- T& subtract(T&, c P&) T:{M} P:{b p} fragment_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
subtract(Type& object, const typename Type::element_type& operand)
{
    return object.subtract(operand);
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type& 
subtract(Type& object, const typename Type::segment_type& operand)
{
    return object.subtract(operand);
}

//------------------------------------------------------------------------------
//- T& subtract(T&, c P&) T:{M} P:{e i} key_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
subtract(Type& object, const typename Type::domain_type& operand)
{
    return object.erase(operand);
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
subtract(Type& object, const typename Type::interval_type& operand)
{
    return object.erase(operand);
}

//==============================================================================
//= Selective Update<IntervalMap>
//==============================================================================
//------------------------------------------------------------------------------
//- T& set_at(T&, c P&) T:{M} P:{e i}
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
//= Intersection<IntervalMap>
//==============================================================================
//------------------------------------------------------------------------------
//- T& subtract(T&, c P&) T:{M} P:{b p} fragment_type
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, void>::type
add_intersection(Type& section, const Type& object, 
                 const typename Type::element_type& operand)
{
    object.add_intersection(section, operand);
}

template<class Type>
typename enable_if<is_interval_map<Type>, void>::type
add_intersection(Type& section, const Type& object, 
                 const typename Type::segment_type& operand)
{
    object.add_intersection(section, operand);
}

//------------------------------------------------------------------------------
//- T& subtract(T&, c P&) T:{M} P:{M'} map fragment_type total
//------------------------------------------------------------------------------
template<class Type, class MapT>
typename enable_if
<
    mpl::and_< is_total<Type>
             , is_concept_compatible<is_interval_map, Type, MapT> >
  , void
>::type
add_intersection(Type& section, const Type& object, const MapT& operand)
{
    section += object;
    section += operand;
}

//------------------------------------------------------------------------------
//- T& subtract(T&, c P&) T:{M} P:{M'} map fragment_type partial
//------------------------------------------------------------------------------
template<class Type, class MapT>
typename enable_if
<
    mpl::and_< mpl::not_<is_total<Type> >
             , is_concept_compatible<is_interval_map, Type, MapT> >
  , void
>::type
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
//- T& subtract(T&, c P&) T:{M} P:{e i S} key_type
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
//- intersects<IntervalMaps> fragment_types
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
//- Symmetric difference flip<Interval_Map> fragment_types
//------------------------------------------------------------------------------
//JODO NOTE: typename enable_if< mpl::and_< is_interval_map_right_intra_combinable<Type, OperandT> //JODO =^= fragment_type_of
//------------------------------------------------------------------------------
//- T& flip(T&, c P&) T:{M} P:{b p} fragment_types
//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
flip(Type& object, const typename Type::element_type& operand)
{
    return object.flip(operand);
}

template<class Type>
typename enable_if<is_interval_map<Type>, Type>::type&
flip(Type& object, const typename Type::segment_type& operand)
{
    return object.flip(operand);
}

//------------------------------------------------------------------------------
//- T& flip(T&, c P&) T:{M} P:{M'} total absorber 
//------------------------------------------------------------------------------
template<class Type, class OperandT>
typename enable_if< mpl::and_< is_total<Type>
                             , absorbs_neutrons<Type>
                             , is_concept_compatible<is_interval_map, 
                                                     Type, OperandT >
                             >
                  , Type>::type&
flip(Type& object, const OperandT&)
{
    object.clear();
    return object;
}

//------------------------------------------------------------------------------
//- T& flip(T&, c P&) T:{M} P:{M'} total enricher 
//------------------------------------------------------------------------------
template<class Type, class OperandT>
typename enable_if< mpl::and_< is_total<Type>
                             , mpl::not_<absorbs_neutrons<Type> >
                             , is_concept_compatible<is_interval_map, 
                                                     Type, OperandT >
                             >
                  , Type>::type&
flip(Type& object, const OperandT& operand)
{
    typedef typename Type::codomain_type  codomain_type;

    object += operand;
    ITL_FORALL(typename Type, it_, object) //JODO: neutralisierendes add.
        it_->second = neutron<codomain_type>::value();

    if(mpl::not_<is_interval_splitter<Type> >::value) //JODO
        itl::join(object);

    return object;
}

//------------------------------------------------------------------------------
//- T& flip(T&, c P&) T:{M} P:{M'} partial 
//------------------------------------------------------------------------------
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

//==============================================================================
//= Domain
//==============================================================================
template<class Type, class SetT>
typename enable_if<is_concept_combinable<is_interval_set, is_interval_map, 
                                         SetT, Type>, SetT>::type&
domain(SetT& result, const Type& object)
{
    result.clear(); 
    ITL_const_FORALL(typename Type, it_, object) 
        result += it_->first; 
    
    return result;
}

//==============================================================================
//= Manipulation by predicates
//==============================================================================
template<class MapT, class Predicate> //JODO unify with element_map . . .
typename enable_if<is_interval_map<MapT>, MapT>::type&
erase_if(const Predicate& pred, MapT& object)
{
    typename MapT::iterator it_ = object.begin();
    while(it_ != object.end())
        if(pred(*it_))
            object.erase(it_++); 
        else ++it_;
    return object;
}

template<class MapT, class Predicate>
inline typename enable_if<is_interval_map<MapT>, MapT>::type&
add_if(const Predicate& pred, MapT& object, const MapT& src)
{
    typename MapT::const_iterator it_ = src.begin();
    while(it_ != src.end())
        if(pred(*it_)) 
            itl::add(object, *it_++); 
    
    return object;
}

template<class MapT, class Predicate>
inline typename enable_if<is_interval_map<MapT>, MapT>::type&
assign_if(const Predicate& pred, MapT& object, const MapT& src)
{
    itl::clear(object);
    return add_if(object, src, pred);
}


//==============================================================================
//= Morphisms
//==============================================================================
template<class Type>
typename enable_if<mpl::and_< is_interval_map<Type>
                            , absorbs_neutrons<Type> >, Type>::type&
absorb_neutrons(Type& object)
{
    return object;
}

template<class Type>
typename enable_if<mpl::and_< is_interval_map<Type>
                            , mpl::not_<absorbs_neutrons<Type> > >, Type>::type&
absorb_neutrons(Type& object)
{
    typedef typename Type::segment_type segment_type;
    return itl::erase_if(content_is_neutron<segment_type>(), object);
}

//==============================================================================
//= Streaming
//==============================================================================
template<class CharType, class CharTraits, class Type>
typename enable_if<is_interval_map<Type>, 
                   std::basic_ostream<CharType, CharTraits> >::type& 
operator << (std::basic_ostream<CharType, CharTraits>& stream, const Type& object)
{
    stream << "{";
    ITL_const_FORALL(typename Type, it_, object)
        stream << "(" << it_->first << "->" << it_->second << ")";

    return stream << "}";
}


}} // namespace boost itl

#endif


