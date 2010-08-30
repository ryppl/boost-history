/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_DETAIL_MAP_FUNCTORS_HPP_JOFA_100823
#define BOOST_ITL_DETAIL_MAP_FUNCTORS_HPP_JOFA_100823

#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/detail/associated_value.hpp>
#include <boost/itl/type_traits/adds_inversely.hpp>
#include <boost/itl/functors.hpp>
#include <boost/itl/map_functions.hpp>

namespace boost{namespace itl
{

template<class MapT, class Predicate>
inline typename enable_if<is_element_map<MapT>, MapT>::type&
erase_if(const Predicate& pred, MapT& object);//JODO gcc-3.4.4 does not recognize some functions from map_functions.hpp
                                              //JODO e.g. erase_if and subtract. Forward decl should not be necessary
                                              //JODO but may be it is better to use forward decls only instead of inlclues here.

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
//JODO should be made usable for interval maps too and must go to it's own file
// in itl/detail

template<class MapT, class Combiner, bool absorbs_neutrons>
struct absorbs_neutron
{
	static bool apply(const typename MapT::codomain_type& value);
};

template<class MapT, class Combiner>
struct absorbs_neutron<MapT,Combiner,false>
{
	static bool apply(const typename MapT::codomain_type&){ return false; }
};

template<class MapT, class Combiner>
struct absorbs_neutron<MapT,Combiner,true>
{
	static bool apply(const typename MapT::codomain_type& value)
	{ return value == Combiner::neutron(); }
};

//------------------------------------------------------------------------------
template<class MapT, class Combiner, bool absorbs_neutrons>
struct handle_neutron
{
	typedef typename MapT::iterator iterator;
	static iterator apply(MapT& object, iterator it);
};

template<class MapT, class Combiner>
struct handle_neutron<MapT,Combiner,false>
{
	typedef typename MapT::iterator iterator;
	static iterator apply(MapT&, iterator it)
	{ return it; }
};

template<class MapT, class Combiner>
struct handle_neutron<MapT,Combiner,true>
{
	typedef typename MapT::iterator iterator;
	static iterator apply(MapT& object, iterator it)
	{ 
		if(it->second == Combiner::neutron())
		{                                           
			object.erase(it);                
			return object.end();                    
		}
		return it;
	}
};

//==============================================================================


//==============================================================================
//=P Addition
//==============================================================================

template <class MapT, class Combiner, bool absorbs_neutrons>
struct map_add 
{             
    typedef MapT map_type;
    typedef typename map_type::value_type    value_type;
    typedef typename map_type::element_type  element_type;
    typedef typename map_type::codomain_type codomain_type;
    typedef typename map_type::iterator      iterator;

    BOOST_STATIC_CONSTANT(bool, 
		co_version = (adds_inversely<codomain_type,Combiner>::value));

    static map_type& apply(map_type& object, const value_type& value_pair)
    {
		if(absorbs_neutron<MapT,Combiner,absorbs_neutrons>::apply(value_pair.second))
            return object;

		// if Combiner is negative e.g. subtract and the codomain has an inverse, 
		// we have to insert the inverse element of value_pair.second
        std::pair<iterator, bool> insertion =  
            object.insert(element_version<MapT,Combiner,co_version>::apply(value_pair));

        if( insertion.second )
            return object;
        else
        {
            iterator it = insertion.first;
            Combiner()((*it).second, value_pair.second);  
                                                   
			handle_neutron<MapT,Combiner,absorbs_neutrons>::apply(object, it);
            return object;
        }
    }

    static iterator apply(map_type& object, iterator prior, const element_type& value_pair)
    {
		if(absorbs_neutron<MapT,Combiner,absorbs_neutrons>::apply(value_pair.second))
            return object.end();

        iterator inserted_ 
            = object.insert(prior, value_type(value_pair.first, Combiner::neutron()));
        Combiner()(inserted_->second, value_pair.second); // May generate an inverse via inv(x)= 0-x

		return handle_neutron<MapT,Combiner,absorbs_neutrons>::apply(object, inserted_);
    }
};



//==============================================================================
//=P Subtraction < is_total && is_invertible >
//==============================================================================

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
    typedef          MapT                       map_type;
    typedef typename map_type::element_type     element_type;
    typedef typename map_type::codomain_combine codomain_combine;
    typedef typename map_type::inverse_codomain_combine 
		                                        inverse_codomain_combine;
    typedef typename map_type::iterator         iterator;
    BOOST_STATIC_CONSTANT(bool, absorbs = (absorbs_neutrons<MapT>::value));

    static map_type& apply(map_type& object, const element_type& value_pair)
    {
        iterator it_ = object.find(value_pair.first);
        if(it_ != object.end())
        {
            inverse_codomain_combine()((*it_).second, value_pair.second);
			handle_neutron<MapT,codomain_combine,absorbs>::apply(object,it_);
        }
        return object;
    }
};


//==============================================================================
//=P Insertion
//==============================================================================

template<class MapT, class Combiner, bool absorbs_neutrons>
struct map_insert
{
    typedef          MapT                       map_type;
    typedef typename map_type::element_type     element_type;
    typedef typename map_type::codomain_combine codomain_combine;
    typedef typename map_type::iterator         iterator;

    static std::pair<iterator,bool> apply(map_type& object, const element_type& value_pair)
    {
		if(absorbs_neutron<MapT,Combiner,absorbs_neutrons>::apply(value_pair.second)) 
            return std::pair<iterator,bool>(object.end(),true);
        else
            return object.insert(value_pair);
    }

    static iterator apply(map_type& object, iterator prior, const element_type& value_pair)
    {
		if(absorbs_neutron<MapT,Combiner,absorbs_neutrons>::apply(value_pair.second)) 
            return object.end();
        else
            return object.insert(prior, value_pair);
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

    static size_type apply(map_type& object, const domain_type& key)
    {
        iterator it_ = object.find(key);
        if(it_ != object.end())
        {
            object.erase(it_);
            return 1;
        }

        return 0;
    }

    static size_type apply(map_type& object, const element_type& value_pair)
    {
		if(absorbs_neutron<MapT,Combiner,absorbs_neutrons>::apply(value_pair.second)) 
            return 0; // neutrons are never contained 'substantially' only 'virtually'.

        iterator it_ = object.find(value_pair.first);
        if(it_ != object.end() && value_pair.second == it_->second)
        {
            object.erase(it_);
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
    typedef          MapT                       map_type;
    typedef typename map_type::domain_type      domain_type;
    typedef typename map_type::element_type     element_type;
    typedef typename map_type::codomain_combine codomain_combine;
    typedef typename map_type::const_iterator   const_iterator;

    BOOST_STATIC_CONSTANT(bool, absorbs = (absorbs_neutrons<MapT>::value));

    static map_type& apply(map_type& section, const map_type& object, const domain_type& key_value)
    {
        const_iterator it_ = object.find(key_value);
        if(it_ != object.end())
            //itl::add(section, *it_);
			map_add<map_type,codomain_combine,absorbs>::apply(section, *it_);
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
        const_iterator it_ = object.find(key_value);
        if(it_ != object.end())
            map_add<map_type,codomain_intersect,absorbs>::apply(section, *it_);

        return section;
    }

    static map_type& apply(map_type& section, const map_type& object, const element_type& value_pair)
    {
        const_iterator it_ = object.find(value_pair.first);
        if(it_ != object.end())
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
		object.swap(section);
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
				const_iterator it_ = object.find(sec_->first);
				if(it_ != object.end())
				{
					itl::add(section, *it_);
					map_add<MapT, codomain_intersect, absorbs_neutrons<MapT>::value >::apply(section, *sec_);
				}
				++sec_;
			}
		}

		object.swap(section);
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
    BOOST_STATIC_CONSTANT(bool, codomain_is_set = (has_set_semantics<codomain_type>::value));

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
    BOOST_STATIC_CONSTANT(bool, codomain_is_set = (has_set_semantics<codomain_type>::value));

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
		const_iterator x2_ = x2.begin();
        while(x2_ != x2.end()) 
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
		const_iterator x2_ = x2.begin();
        while(x2_ != x2.end()) 
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

