/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// void_cast.cpp: implementation of run-time casting of void pointers

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// <gennadiy.rozental@tfn.com>

//  See http://www.boost.org for updates, documentation, and revision history.

#if (defined _MSC_VER) && (_MSC_VER == 1200)
# pragma warning (disable : 4786) // too long name, harmless warning
#endif

#include <cassert>

// STL
#include <set>
#include <functional>
#include <algorithm>
#include <cassert>

// BOOST
#define BOOST_SERIALIZATION_SOURCE
#include <boost/serialization/singleton.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/void_cast.hpp>

namespace boost { 
namespace serialization {
namespace void_cast_detail {

// registry of casting objects
struct void_caster_compare
{
    bool
    operator()(
        const void_caster * lhs, 
        const void_caster * rhs 
    ) const {
        if( lhs->m_derived < rhs->m_derived )
            return true;
        if( lhs->m_base < rhs->m_base )
            return true;
        return false;
    }
};

typedef std::set<const void_caster *, void_caster_compare> set_type;
typedef boost::serialization::singleton<set_type> void_caster_registry;

// implementation of void caster base class
BOOST_SERIALIZATION_DECL(BOOST_PP_EMPTY())
void_caster::void_caster(
    extended_type_info const & derived,
    extended_type_info const & base 
) :
    m_derived(derived),
    m_base(base)
{}

BOOST_SERIALIZATION_DECL(void)
void_caster::static_register() const {
    void_caster_registry::get_mutable_instance().insert(this);
    // to do - add new void_caster_derived entries
    // which can be generated by adding this new primitive
    /*
    while(){
        if(!extend_down(this)
        && !extend_up(this))
            break;
    }
    */
}

BOOST_SERIALIZATION_DECL(void)
void_caster::static_unregister() const {
    void_cast_detail::set_type & st 
        = void_caster_registry::get_mutable_instance();
    void_cast_detail::set_type::iterator it;
    it = st.find(this);
    assert(st.end() != it);
    st.erase(it);
    // to do - remove all void_caster_derived entries
    // which depend upon this primitive
    /*
    while(){
        if(!truncate_down(this)
        && !truncate_up(this))
            break;
    }
    */
}

// implementation of shortcut void caster
class void_caster_derived : public void_caster
{
    std::ptrdiff_t difference;
    virtual void const *
    upcast(void const * const t) const{
        return static_cast<const char *> ( t ) + difference;
    }
    virtual void const *
    downcast(void const * const t) const{
        return static_cast<const char *> ( t ) - difference;
    }
public:
    void_caster_derived(
        extended_type_info const & derived,
        extended_type_info const & base,
        std::ptrdiff_t difference
    ) :
        void_caster(derived, base),
        difference( difference )
    {
        this->static_register();
    }
    ~void_caster_derived(){
        this->static_unregister();
    }
};

// just used as a search key
class void_caster_argument : public void_caster
{
    virtual void const *
    upcast(void const * const t) const {
        assert(false);
        return NULL;
    }
    virtual void const *
    downcast( void const * const t) const {
        assert(false);
        return NULL;
    }
public:
    void_caster_argument(
        extended_type_info const & derived,
        extended_type_info const & base
    ) :
        void_caster(derived, base)
    {}
    ~void_caster_argument(){};
};

} // namespace void_cast_detail

// Given a void *, assume that it really points to an instance of one type
// and alter it so that it would point to an instance of a related type.
// Return the altered pointer. If there exists no sequence of casts that
// can transform from_type to to_type, return a NULL.  
BOOST_SERIALIZATION_DECL(void const *)  
void_upcast(
    extended_type_info const & derived,
    extended_type_info const & base,
    void const * const t
){
    // same types - trivial case
    if (derived == base)
        return t;

    // check to see if base/derived pair is found in the registry
    const void_cast_detail::set_type & s
        = void_cast_detail::void_caster_registry::get_const_instance();
    void_cast_detail::set_type::const_iterator it;
    void_cast_detail::void_caster_argument ca(derived, base);

    it = s.find(& ca);
    
    // if so
    if (s.end() != it)
        // we're done
        return (*it)->upcast(t);

    // try to find a chain that gives us what we want
    for(it = s.begin(); it != s.end(); ++it){
        // if the current candidate doesn't cast to the desired target type
        if((*it)->m_base == base){
            // if the current candidate casts from the desired source type
            if ((*it)->m_derived == derived){
                // we have a base/derived match - we're done
                // cast to the intermediate type
                return (*it)->upcast(t);
            }
            const void * t_new;
            t_new = void_upcast(derived, (*it)->m_derived, t);
            if(NULL != t_new)
                return (*it)->upcast(t_new);
        }
    }
    return NULL;
}

BOOST_SERIALIZATION_DECL(void const *)  
void_downcast(
    extended_type_info const & derived,
    extended_type_info const & base,
    void const * const t
){
    // same types - trivial case
    if (derived == base)
        return t;

    // check to see if base/derived pair is found in the registry
    const void_cast_detail::set_type & s
        = void_cast_detail::void_caster_registry::get_const_instance();
    void_cast_detail::set_type::const_iterator it;
    void_cast_detail::void_caster_argument ca(derived, base);

    it = s.find(& ca);
    
    // if so
    if (s.end() != it)
        // we're done
        return(*it)->downcast(t);

    // try to find a chain that gives us what we want
    for(it = s.begin(); it != s.end(); ++it){
        // if the current candidate doesn't cast to the desired target type
        if ((*it)->m_derived == derived){
            // if the current candidate casts from the desired source type
            if ((*it)->m_base == base){
                // we have a base/derived match - we're done
                // cast to the intermediate type
                return (*it)->downcast(t);
            }
            const void * t_new;
            t_new = void_downcast((*it)->m_base, base, t);
            if(NULL != t_new)
                return (*it)->downcast(t_new);
        }
    }
    return NULL;
}

} // namespace serialization
} // namespace boost

// EOF
