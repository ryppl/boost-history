/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_DETAIL_INTERVAL_MAP_FUNCTORS_HPP_JOFA_100826
#define BOOST_ITL_DETAIL_INTERVAL_MAP_FUNCTORS_HPP_JOFA_100826

#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/functors.hpp>
#include <boost/itl/interval_functions.hpp>
#include <boost/itl/detail/interval_map_algo.hpp>

namespace boost{namespace itl
{

//==============================================================================
//=P Containment
//==============================================================================


template<class MapT>
struct interval_map_contains
{
    typedef typename MapT::element_type  element_type;
    typedef typename MapT::segment_type  segment_type;
    typedef typename MapT::interval_type interval_type;
    typedef typename MapT::const_iterator const_iterator;

    static bool apply(const MapT& super, const element_type& key_value_pair)
    {
        const_iterator it_ = super.find(key_value_pair.key);
        return it_ != super.end() && it_->second == key_value_pair.data;
    }

    static bool apply(const MapT& super, const segment_type& sub_segment)
    {
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

    template<class CoMapT>
    static typename enable_if<is_interval_map<CoMapT>, bool>::type
    apply(const MapT& super, const CoMapT& sub)
    {
        return Interval_Set::within(sub, super);
    }
};

//------------------------------------------------------------------------------

template<class MapT, bool is_total>
struct interval_map_contains_key
{
    typedef typename MapT::domain_type   domain_type;
    typedef typename MapT::interval_type interval_type;
    typedef typename MapT::const_iterator const_iterator;

    static bool apply(const MapT&, const domain_type&);
    static bool apply(const MapT&, const interval_type&);

    template<class SetT>
    static typename enable_if<is_interval_set<SetT>, bool>::type
    apply(const MapT&, const SetT&);
};

template<class MapT>
struct interval_map_contains_key<MapT, true>
{                               // is_total
    typedef typename MapT::domain_type   domain_type;
    typedef typename MapT::interval_type interval_type;
    typedef typename MapT::set_type      set_type;

    //JODO enable_if< is_key_type_of >
    // Key objects are always contained in a total map
    static bool apply(const MapT&, const domain_type&  ){ return true; }
    static bool apply(const MapT&, const interval_type&){ return true; }

    template<class SetT>
    static typename enable_if<is_interval_set<SetT>, bool>::type
    apply(const MapT&, const SetT&){ return true; }
};

template<class MapT>
struct interval_map_contains_key<MapT, false>
{                               // !is_total
    typedef typename MapT::domain_type   domain_type;
    typedef typename MapT::interval_type interval_type;
    typedef typename MapT::set_type      set_type;

    typedef typename MapT::const_iterator const_iterator;

    static bool apply(const MapT& super, const domain_type& key)    
    {
        return super.find(key) != super.end();
    }

    static bool apply(const MapT& super, const interval_type& sub_interval)
    {
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

    template<class SetT>
    static typename enable_if<is_interval_set<SetT>, bool>::type
    apply(const MapT& super, const SetT& sub)
    {
        return Interval_Set::within(sub, super);
    }
};

}} // namespace itl boost

#endif

