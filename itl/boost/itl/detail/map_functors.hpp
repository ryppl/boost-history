/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_DETAIL_MAP_FUNCTORS_HPP_JOFA_100823
#define BOOST_ITL_DETAIL_MAP_FUNCTORS_HPP_JOFA_100823

#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/functors.hpp>
#include <boost/itl/map_functions.hpp>

namespace boost{namespace itl
{

template<class MapT, class Predicate>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
erase_if(const Predicate& pred, MapT& object);//JODO gcc-3.4.4 does not recognize some functions from map_functions.hpp
                                              //JODO e.g. erase_if and subtract. Forward decl should not be necessary
                                              //JODO but may be it is better to use forward decls only instead of inlclues here.


template<class MapT, class Combiner> //JODO locate
struct adds_inversely
{
    typedef adds_inversely type;
    BOOST_STATIC_CONSTANT(bool, 
        value = (mpl::and_< has_inverse<typename MapT::codomain_type>
                          , is_negative<Combiner> 
                          >::value)); 
};


template<class MapT, class Combiner, bool creates_inverse>
struct element_version
{
    static typename MapT::value_type apply(const typename MapT::value_type& val);
};

template<class MapT, class Combiner>
struct element_version<MapT, Combiner, false>
{
    static typename MapT::value_type apply(const typename MapT::value_type& value_pair)
    { return value_pair; }
};

template<class MapT, class Combiner>
struct element_version<MapT, Combiner, true>
{
    static typename MapT::value_type apply(const typename MapT::value_type& value_pair)
    {
        return typename MapT::value_type(value_pair.first, version<Combiner>()(value_pair.second));
    }
};

//==============================================================================
//=P Containment
//==============================================================================

template<class MapT>
struct map_within
{
	typedef typename MapT::set_type set_type;

	static bool apply(const typename MapT::set_type& sub, const MapT& super)
	{
		if(itl::is_empty(sub))                return true;
		if(itl::is_empty(super))              return false;
		if(itl::size(super) < itl::size(sub)) return false;

		typename set_type::const_iterator common_lwb_;
		typename set_type::const_iterator common_upb_;
		if(!Set::common_range(common_lwb_, common_upb_, sub, super))
			return false;

		typename set_type::const_iterator sub_ = sub.begin();
		typename MapT::const_iterator super_;
		while(sub_ != seqs<MapT>::end(sub))
		{
			super_ = seqs<MapT>::find(super, *sub_++);
			if(super_ == seqs<MapT>::end(super)) 
				return false;
		}
		return true;
	}

	static bool apply(const MapT& sub, const MapT& super)
	{
		if(&super == &sub)                    return true;
		if(itl::is_empty(sub))                return true;
		if(itl::is_empty(super))              return false;
		if(itl::size(super) < itl::size(sub)) return false;

		typename MapT::const_iterator common_lwb_;
		typename MapT::const_iterator common_upb_;
		if(!Set::common_range(common_lwb_, common_upb_, sub, super))
			return false;

		typename MapT::const_iterator sub_ = sub.begin(), super_;
		while(sub_ != seqs<MapT>::end(sub))
		{
			super_ = seqs<MapT>::find(super, seqs<MapT>::key_value(sub_));
			if(super_ == super.end()) 
				return false;
			else if(!(seqs<MapT>::co_value(sub_) == seqs<MapT>::co_value(super_)))
				return false;
			sub_++;
		}
		return true;
	}
};

//==============================================================================
//=P Addition
//==============================================================================

template<class MapT, class Combiner, bool absorbs_neutrons>
struct map_add
{
    typedef MapT map_type;
    typedef typename map_type::value_type    value_type;
    typedef typename map_type::element_type  element_type;
    typedef typename map_type::iterator      iterator;

    static MapT&    apply(map_type& object, const value_type& value_pair);
    static iterator apply(map_type& object, iterator prior, const element_type& value_pair);
};

template <class MapT, class Combiner>
struct map_add<MapT, Combiner, true> 
{               // absorbs_neutrons
    typedef MapT map_type;
    typedef typename map_type::value_type    value_type;
    typedef typename map_type::element_type  element_type;
    typedef typename map_type::codomain_type codomain_type;
    typedef typename map_type::iterator      iterator;

    BOOST_STATIC_CONSTANT(bool, co_version = (adds_inversely<MapT,Combiner>::value));

    static map_type& apply(map_type& object, const value_type& val)
    {
        if(val.second == Combiner::neutron())
            return object;
                                              // if Combiner is negative e.g. subtract and
        std::pair<iterator, bool> insertion = // the codomain has an inverse, we have to 
            seqs<map_type>::insert(object,    // insert the inverse element of val.second
                                   element_version<MapT,Combiner,co_version>::apply(val));

        if( insertion.second )
            return object;
        else
        {
            iterator it = insertion.first;
            Combiner()((*it).second, val.second);   // If this combination results in a
                                                    // neutral element, 
            if((*it).second == Combiner::neutron()) // it has to be removed.
                seqs<map_type>::erase(object, it);

            return object;
        }
    }


    static iterator apply(map_type& object, iterator prior, const element_type& value_pair)
    {
        if(value_pair.second == Combiner::neutron())
            return seqs<map_type>::end(object);

        iterator inserted_ 
            = seqs<map_type>::insert(object, prior, 
                                     value_type(value_pair.first, Combiner::neutron()));
        Combiner()(inserted_->second, value_pair.second); // May generate an inverse via inv(x)= 0-x

        if(inserted_->second == Combiner::neutron())
        {
            seqs<map_type>::erase(object, inserted_);
            return seqs<map_type>::end(object);
        }
        else
            return inserted_;
    }
};


template <class MapT, class Combiner>
struct map_add<MapT, Combiner, false> 
{               // !absorbs_neutrons
    typedef MapT map_type;
    typedef typename map_type::value_type    value_type;
    typedef typename map_type::element_type  element_type;
    typedef typename map_type::codomain_type codomain_type;
    typedef typename map_type::iterator      iterator;

    BOOST_STATIC_CONSTANT(bool, co_version = (adds_inversely<MapT,Combiner>::value));

    static map_type& apply(map_type& object, const value_type& value_pair)
    {
        std::pair<iterator, bool> insertion = 
            seqs<map_type>::insert(object, 
                                   element_version<MapT,Combiner,co_version>::apply(value_pair));

        if( insertion.second )
            return object;
        else
        {
            iterator it = insertion.first;
            Combiner()((*it).second, value_pair.second);
            return object;
        }
    }

    static iterator apply(map_type& object, iterator prior, const element_type& value_pair)
    {
        iterator inserted_ 
            = seqs<MapT>::insert(object, prior, 
                                 value_type(value_pair.first, Combiner::neutron()));
        Combiner()(inserted_->second, value_pair.second); // May generate an inverse via inv(x)= 0-x
        return inserted_;
    }

};


//==============================================================================
//=P Subtraction
//==============================================================================

//------------------------------------------------------------------------------
//-P Subtraction for partial maps
//------------------------------------------------------------------------------

template<class MapT, class Combiner, bool absorbs_neutrons>
struct map_partial_subtract
{
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;

    static map_type& apply(map_type&, const element_type&);
};

//------------------------------------------------------------------------------

template<class MapT, bool is_total_invertible>
struct map_subtract
{
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;

    static map_type& apply(map_type&, const element_type& value_pair);
};

template<class MapT>
struct map_subtract<MapT, true>
{
    typedef          MapT                   map_type;
    typedef typename map_type::element_type element_type;
    typedef typename map_type::inverse_codomain_combine inverse_codomain_combine;

    static map_type& apply(map_type& object, const element_type& value_pair)
    {
        return map_add<map_type
                      ,inverse_codomain_combine
                      ,absorbs_neutrons<MapT>::value
                      >::apply(object, value_pair);        
    }
};

template<class MapT>
struct map_subtract<MapT, false>
{
    typedef          MapT                   map_type;
    typedef typename map_type::element_type element_type;
    typedef typename map_type::inverse_codomain_combine inverse_codomain_combine;

    static map_type& apply(map_type& object, const element_type& value_pair)
    {
        return map_partial_subtract<map_type
                                   ,inverse_codomain_combine
                                   ,absorbs_neutrons<MapT>::value >
               ::apply(object, value_pair);        
    }
};

template<class MapT, class Combiner>
struct map_partial_subtract<MapT, Combiner, true>
{
    typedef          MapT                       map_type;
    typedef typename map_type::element_type     element_type;
    typedef typename map_type::codomain_combine codomain_combine;
    typedef typename map_type::iterator         iterator;

    static map_type& apply(map_type& object, const element_type& value_pair)
    {
        iterator it_ = seqs<map_type>::find(object, value_pair.first);
        if(it_ != seqs<map_type>::end(object))
        {
            Combiner()((*it_).second, value_pair.second);
            if((*it_).second == codomain_combine::neutron())
                seqs<map_type>::erase(object, it_);
        }
        return object;
    }
};

template<class MapT, class Combiner>
struct map_partial_subtract<MapT, Combiner, false>
{
    typedef          MapT                   map_type;
    typedef typename map_type::element_type element_type;
    typedef typename map_type::iterator     iterator;

    static map_type& apply(map_type& object, const element_type& value_pair)
    {
        iterator it_ = seqs<MapT>::find(object, value_pair.first);
        if(it_ != seqs<MapT>::end(object))
            Combiner()((*it_).second, value_pair.second);
        
        return object;
    }
};


//==============================================================================
//=P Insertion
//==============================================================================

template<class MapT, class Combiner, bool absorbs_neutrons>
struct map_insert
{
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;
    typedef typename map_type::iterator     iterator;

    static map_type& apply(map_type&, const element_type&);
    static map_type& apply(map_type&, iterator, const element_type&);
};

template<class MapT, class Combiner>
struct map_insert<MapT, Combiner, true>
{
    typedef          MapT                       map_type;
    typedef typename map_type::element_type     element_type;
    typedef typename map_type::codomain_combine codomain_combine;
    typedef typename map_type::iterator         iterator;

    static std::pair<iterator,bool> apply(map_type& object, const element_type& value_pair)
    {
        if(value_pair.second == codomain_combine::neutron()) 
            return std::pair<iterator,bool>(object.end(),true);
        else
            return seqs<map_type>::insert(object, value_pair);
    }

    static iterator apply(map_type& object, iterator prior, const element_type& value_pair)
    {
        if(value_pair.second == codomain_combine::neutron()) 
            return seqs<map_type>::end(object);
        else
            return seqs<map_type>::insert(object, prior, value_pair);
    }

};

template<class MapT, class Combiner>
struct map_insert<MapT, Combiner, false>
{
    typedef          MapT                   map_type;
    typedef typename map_type::element_type element_type;
    typedef typename map_type::iterator     iterator;

    static std::pair<iterator,bool> apply(map_type& object, const element_type& value_pair)
    {
        return seqs<MapT>::insert(object, value_pair);
    }

    static iterator apply(map_type& object, iterator prior, const element_type& value_pair)
    {
        return seqs<MapT>::insert(object, prior, value_pair);
    }

};

//==============================================================================
//=P Erasure
//==============================================================================

template<class MapT, class Combiner, bool absorbs_neutrons>
struct map_erase
{
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;
    typedef typename map_type::size_type    size_type;
    typedef typename map_type::iterator     iterator;

    static size_type apply(map_type&, const element_type&);

    static size_type apply(map_type& object, const domain_type& key)
    {
        iterator it_ = seqs<map_type>::find(object, key);
        if(it_ != seqs<map_type>::end(object))
        {
            seqs<map_type>::erase(object, it_);
            return 1;
        }

        return 0;
    }

};

template<class MapT, class Combiner>
struct map_erase<MapT, Combiner, true>
{
    typedef          MapT                       map_type;
    typedef typename map_type::domain_type      domain_type;
    typedef typename map_type::element_type     element_type;
    typedef typename map_type::size_type        size_type;
    typedef typename map_type::codomain_combine codomain_combine;
    typedef typename map_type::iterator         iterator;

    static size_type apply(map_type& object, const element_type& value_pair)
    {
        if(value_pair.second == codomain_combine::neutron())
            return 0; // neutrons are never contained 'substantially' 
                      // only 'virtually'.

        iterator it_ = seqs<map_type>::find(object, value_pair.first);
        if(it_ != seqs<map_type>::end(object) && value_pair.second == it_->second)
        {
            seqs<map_type>::erase(object, it_);
            return 1;
        }

        return 0;
    }
};

template<class MapT, class Combiner>
struct map_erase<MapT, Combiner, false>
{
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;
    typedef typename map_type::size_type    size_type;
    typedef typename map_type::iterator     iterator;

    static size_type apply(map_type& object, const element_type& value_pair)
    {
        iterator it_ = seqs<map_type>::find(object, value_pair.first);
        if(it_ != seqs<map_type>::end(object) && value_pair.second == it_->second)
        {
            seqs<map_type>::erase(object, it_);
            return 1;
        }

        return 0;
    }
};

//==============================================================================
//=P Intersection
//==============================================================================

template<class MapT, bool is_total>
struct map_add_intersection
{
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;

    //JODO test add_intersection(c, a, b) == c += (a & b)
    static map_type& apply(map_type&, const map_type&, const domain_type&);
    static map_type& apply(map_type&, const map_type&, const element_type&);
};


template<class MapT>
struct map_add_intersection<MapT, true> 
{                          // is_total     
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;


    static map_type& apply(map_type& section, const map_type& object, const domain_type& key_value)
    {
        typename map_type::const_iterator it_ = seqs<map_type>::find(object, key_value);
        if(it_ != seqs<map_type>::end(object))
            itl::add(section, *it_);
        return section;
    }

    static map_type& apply(map_type& section, const map_type& object, const element_type& value_pair)
    {
        itl::add(section, value_pair); //JODO test this
        return section;
    }
};

template<class MapT>
struct map_add_intersection<MapT, false>
{                          // !is_total
    typedef          MapT                         map_type;
    typedef typename map_type::domain_type        domain_type;
    typedef typename map_type::element_type       element_type;
    typedef typename map_type::codomain_intersect codomain_intersect;
    typedef typename map_type::const_iterator     const_iterator;

    BOOST_STATIC_CONSTANT(bool, absorbs = (absorbs_neutrons<MapT>::value));

    static map_type& apply(map_type& section, const map_type& object, const domain_type& key_value)
    {
        const_iterator it_ = seqs<map_type>::find(object, key_value);
        if(it_ != seqs<map_type>::end(object))
            map_add<map_type,codomain_intersect,absorbs>::apply(section, *it_);

        return section;
    }

    static map_type& apply(map_type& section, const map_type& object, const element_type& value_pair)
    {
        const_iterator it_ = seqs<map_type>::find(object, value_pair.first);
        if(it_ != seqs<map_type>::end(object))
        {
            map_add<map_type,codomain_intersect,absorbs>::apply(section, *it_);
            map_add<map_type,codomain_intersect,absorbs>::apply(section, value_pair);
        }
        return section;
    }
};

//------------------------------------------------------------------------------
//-P inplace_intersect
//------------------------------------------------------------------------------

template<class MapT, bool is_total>
struct map_inplace_intersect
{
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;

    //JODO test add_intersection(c, a, b) == c += (a & b)
    static map_type& apply(map_type&, const element_type&);
    static map_type& apply(map_type&, const map_type&);
};


template<class MapT>
struct map_inplace_intersect<MapT, true> 
{                       // is_total     
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;

    static map_type& apply(map_type& object, const element_type& operand){return object += operand;}
    static map_type& apply(map_type& object, const map_type& operand)    {return object += operand;}
};

template<class MapT>
struct map_inplace_intersect<MapT, false>
{                           // !is_total
    typedef          MapT                         map_type;
    typedef typename map_type::domain_type        domain_type;
    typedef typename map_type::element_type       element_type;
    typedef typename map_type::codomain_intersect codomain_intersect;
    typedef typename map_type::const_iterator     const_iterator;

    static MapT& apply(MapT& object, const element_type& operand)
    {
        MapT section;
		map_add_intersection<MapT,false>::apply(section, object, operand);
		seqs<MapT>::swap(object, section);
        return object;
    }

    static MapT& apply(MapT& object, const MapT& operand)
    {
        MapT section;
        //CL map_add_intersection<MapT,false>::apply(section, object, operand);

		const_iterator common_lwb_;
		const_iterator common_upb_;
		if(Set::common_range(common_lwb_, common_upb_, operand, object))
		{
			const_iterator sec_ = common_lwb_;
			while(sec_ != common_upb_)
			{
				const_iterator it_ = seqs<MapT>::find(object, sec_->first);
				if(it_ != seqs<MapT>::end(object))
				{
					itl::add(section, *it_);
					map_add<MapT, codomain_intersect, absorbs_neutrons<MapT>::value >::apply(section, *sec_);
				}
				++sec_;
			}
		}

		seqs<MapT>::swap(object, section);
        return object;
    }

};

//------------------------------------------------------------------------------
//-P intersects
//------------------------------------------------------------------------------
template<class MapT, bool is_total>
struct map_intersects
{
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;
    typedef typename map_type::set_type     set_type;

    static bool apply(map_type&, const domain_type&);
    static bool apply(map_type&, const element_type&);
    static bool apply(map_type&, const set_type&);
    static bool apply(map_type&, const map_type&);
};


template<class MapT>
struct map_intersects<MapT, true> 
{                    // is_total     
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;
    typedef typename map_type::set_type     set_type;

    static bool apply(map_type&, const domain_type&) {return true;}
    static bool apply(map_type&, const element_type&){return true;}
    static bool apply(map_type&, const set_type&)    {return true;}
    static bool apply(map_type&, const map_type&)    {return true;}
};

template<class MapT>
struct map_intersects<MapT, false>
{                    // !is_total
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;
    typedef typename map_type::set_type     set_type;

    static bool apply(const MapT& object, const domain_type& key)
    {
        return object.contains(key);
    }

    static bool apply(const MapT& object, const element_type& key_value_pair)
    {
        MapT intersection;
		map_add_intersection<MapT,false>::apply(intersection, object, key_value_pair);
        return !intersection.empty();
    }

    static bool apply(const MapT& object, const set_type& key_set)
    {
        if(object.iterative_size() < key_set.iterative_size())
            return Map::intersects(object, key_set);
        else
            return Map::intersects(key_set, object);
    }

    static bool apply(const MapT& object, const MapT& operand)
    {
        if(object.iterative_size() < operand.iterative_size())
            return Map::intersects(object, operand);
        else
            return Map::intersects(operand, object);
    }
};


//==============================================================================
//=P Symmetric difference
//==============================================================================

template<class MapT, bool codomain_is_set>
struct map_flip_ 
{         
    typedef          MapT                   map_type;
    typedef typename map_type::element_type element_type;

    static void apply(MapT&, const element_type&);
    static void apply(MapT&, const MapT&);
};

template<class MapT, bool is_total, bool absorbs_neutrons>
struct map_flip
{
    typedef          MapT                   map_type;
    typedef typename map_type::domain_type  domain_type;
    typedef typename map_type::element_type element_type;

    static map_type& apply(map_type&, const element_type&);
    static map_type& apply(map_type&, const map_type&);
};

template<class MapT>
struct map_flip<MapT, true, true> 
{              // is_total, absorbs_neutrons     
    typedef          MapT                   map_type;
    typedef typename map_type::element_type element_type;

    static map_type& apply(map_type& object, const element_type&)
    {
        itl::clear(object);
        return object;
    }

    static map_type& apply(map_type& object, const map_type&)
    {
        itl::clear(object);
        return object;
    }
};

template<class MapT>
struct map_flip<MapT, true, false> 
{              // is_total, !absorbs_neutrons     
    typedef MapT map_type;
    typedef typename map_type::codomain_type codomain_type;
    BOOST_STATIC_CONSTANT(bool, codomain_is_set = (is_set<codomain_type>::value));

    static map_type& apply(map_type& object, const map_type& operand)
    {
        map_flip_<MapT,codomain_is_set>::apply(object, operand);

        ITL_FORALL(typename MapT, it_, object)
            it_->second = neutron<codomain_type>::value();

        return object;
    }
};

template<class MapT, bool absorbs_neutrons>
struct map_flip<MapT, false, absorbs_neutrons> 
{              // !is_total
    typedef          MapT                    map_type;
    typedef typename map_type::codomain_type codomain_type;
    BOOST_STATIC_CONSTANT(bool, codomain_is_set = (is_set<codomain_type>::value));

    static map_type& apply(map_type& object, const map_type& operand)
    {
        map_flip_<MapT,codomain_is_set>::apply(object, operand);
        return object;
    }
};

//------------------------------------------------------------------------------
template<class MapT>
struct map_flip_<MapT, false> 
{        // !codomain_is_set
    typedef          MapT                     map_type;
    typedef typename map_type::element_type   element_type;
    typedef typename map_type::iterator       iterator;
    typedef typename map_type::const_iterator const_iterator;

    static void apply(MapT& result, const element_type& value_pair)
    {
        std::pair<iterator,bool> insertion = itl::insert(result, value_pair);

        if(!insertion.second)
            MapT::subtract_::apply(result, *insertion.first);
    }

    static void apply(MapT& result, const MapT& x2)
    {
		const_iterator x2_ = seqs<MapT>::begin(x2);
        while(x2_ != seqs<MapT>::end(x2)) 
            apply(result, *x2_++);
    }
};


template<class MapT>
struct map_flip_<MapT, true> 
{   // codomain_is_set
    typedef MapT map_type;
    typedef typename map_type::domain_type    domain_type;
    typedef typename map_type::codomain_type  codomain_type;
    typedef typename map_type::element_type   element_type;
    typedef typename map_type::iterator       iterator;
    typedef typename map_type::const_iterator const_iterator;
    typedef typename map_type::inverse_codomain_intersect 
                                              inverse_codomain_intersect;

    static void apply(MapT& result, const element_type& value_pair)
    {
		std::pair<iterator,bool> insertion = itl::insert(result, value_pair);

        if(!insertion.second)
        {
            iterator      res_         = insertion.first;
            codomain_type common_value = res_->second;
            domain_type   key_value    = res_->first;
            inverse_codomain_intersect()(common_value, value_pair.second);
            MapT::subtract_::apply(result, *res_);
            itl::add(result, element_type(key_value, common_value));
        }
    }

    static void apply(MapT& result, const MapT& x2)
    {
		const_iterator x2_ = seqs<MapT>::begin(x2);
        while(x2_ != seqs<MapT>::end(x2)) 
			apply(result, *x2_++);
    }
};




//=============================================================================
//=P Neutron absorbtion
//=============================================================================

template<class MapT, bool absorbs_neutrons>
struct map_absorb_neutrons
{
    typedef MapT map_type;
    static void apply(map_type&);
};

template<class MapT>
struct map_absorb_neutrons<MapT, false> 
{                 // !absorbs_neutrons     
    typedef MapT map_type;
    static void absorb_neutrons(map_type&){}
};


template<class MapT>
struct map_absorb_neutrons<MapT, true>
{                 // absorbs_neutrons
    typedef MapT map_type;
	typedef typename MapT::element_type element_type;

    static void absorb_neutrons(map_type& object)
    {
		itl::erase_if(content_is_neutron<element_type>(), object);
    }
};


}} // namespace itl boost

#endif

