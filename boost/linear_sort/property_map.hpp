/* (C) Copyright Hervé Brönnimann, Polytechnic University, 2002.
 * 
 * Permission to copy, use, modify, sell and distribute this software
 * and its documentation is granted provided this copyright notice
 * appears in all copies. This software and its documentation is
 * provided "as is" without express or implied warranty, and with no
 * claim as to its suitability for any purpose.
 */

#ifndef BOOST_LINEAR_SORT_PROPERTY_MAP_HPP
#define BOOST_LINEAR_SORT_PROPERTY_MAP_HPP

#include <boost/limits.hpp>
#include <boost/static_assert.hpp>
#include <boost/property_map.hpp>

namespace boost {

  // this is what the boost::identity_property_map should look like!
  template <class T>
  struct linear_sort_identity_property_map 
    : public boost::put_get_helper< T const&,
                                    linear_sort_identity_property_map<T> > 
  {
    typedef T                                   key_type;
    typedef T                                   value_type;
    typedef T const&                            const_reference;
    typedef T &                                 reference;
    typedef boost::lvalue_property_map_tag      category;
    inline const_reference operator[](T const& x) const { return x; }
    inline reference operator[](T& x) const { return x; }
  };

  template <class T>
  linear_sort_identity_property_map<T>
  make_linear_sort_identity_property_map(T const&) {
    return linear_sort_identity_property_map<T>();
  }

  // this property map gives access to the nth radix of a builtin type
  template < typename T, typename Radix,
             class PropertyMap = linear_sort_identity_property_map<T> >
  struct nth_radix_property_map
    : public boost::put_get_helper<Radix,
                                   nth_radix_property_map<T,Radix,PropertyMap> >
  {
    typedef std::size_t                                     size_type;
    typedef typename property_traits<PropertyMap>::key_type key_type;
    typedef Radix                                           value_type;
    typedef Radix&                                          reference;
    typedef Radix const&                                    const_reference;
    typedef boost::lvalue_property_map_tag                  category;
    inline reference operator[](key_type& x) const
      { return *((Radix*)(&m_pmap[x])+m_n); }
    inline const_reference operator[](key_type const& x) const
      { return *((Radix const*)(&m_pmap[x])+m_n); }
  
    explicit
    nth_radix_property_map(size_type n = -1, PropertyMap p = PropertyMap())
      : m_n(convert_endian(n)), m_pmap(p), m_float_hack(false) {}
    nth_radix_property_map(nth_radix_property_map const& p)
      : m_n(p.m_n), m_pmap(p.m_pmap), m_float_hack(p.m_float_hack) {}
    nth_radix_property_map& operator=(nth_radix_property_map const& p)
      { m_n = p.m_n; m_pmap = p.m_pmap; m_float_hack = p.m_float_hack; }

    void set_radix(size_type n) { m_n = convert_endian(n); }
    void set_float_hack(bool hack) { m_float_hack = hack; }
    bool get_float_hack() const    { return m_float_hack; }

    // assume small endian, but really that should be decided by a BOOST
    // macro
    size_type convert_endian(size_type n) { return sizeof(T)-n-1; /* n */ }
  protected:
    size_type m_n;
    PropertyMap m_pmap;
    bool m_float_hack;
  };

  // this property map gives access to the nth element of a container
  // such as boost::array<T,N> (unfortunately, T[N] does not seem to work)
  template < typename Container, class T,
             class PropertyMap = linear_sort_identity_property_map<Container> >
  struct nth_element_property_map
    : public boost::put_get_helper<T&,
        nth_element_property_map<Container,T,PropertyMap> >
  {
    typedef std::size_t size_type;
    typedef typename property_traits<PropertyMap>::key_type key_type;
    typedef T                                               value_type;
    typedef T&                                              reference;
    typedef T const&                                        const_reference;
    typedef typename property_traits<PropertyMap>::category category;
    inline value_type& operator[](key_type& x) const
      { return m_pmap[x][m_n]; }
    inline value_type const& operator[](key_type const& x) const
      { return m_pmap[x][m_n]; }
  
    explicit
    nth_element_property_map(size_type n = -1, PropertyMap p = PropertyMap())
      : m_n(n) , m_pmap(p) {}
    nth_element_property_map(nth_element_property_map const& p)
      : m_n(p.m_n), m_pmap(p.m_pmap) {}
    nth_element_property_map& operator=(nth_element_property_map const& p)
      { m_n = p.m_n; m_pmap = p.m_pmap; }

    void set_radix(size_type n) { m_n = n; }
  protected:
    size_type m_n;
    PropertyMap m_pmap;
  };

  // this property map adaptor applies a dereference operator prior to
  // computing the property
  template <class Iterator,
            class PropertyMap
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
  = linear_sort_identity_property_map<
                         typename std::iterator_traits<Iterator>::value_type > >
#endif
  struct dereference_property_map
    : PropertyMap
    , public boost::put_get_helper<Iterator,
                               dereference_property_map<Iterator, PropertyMap> >
  {
    typedef Iterator                                          key_type;
    typedef typename property_traits<PropertyMap>::value_type value_type;
    typedef typename property_traits<PropertyMap>::reference  reference;
    typedef typename property_traits<PropertyMap>::category   category;
    inline value_type& operator[](key_type& x) const
      { return PropertyMap::operator[](*x); }
    inline value_type operator[](key_type const& x) const
      { return PropertyMap::operator[](*x); }
  
    dereference_property_map() : PropertyMap() {}
    explicit dereference_property_map(PropertyMap pmap) : PropertyMap(pmap) {}
    dereference_property_map(dereference_property_map const& pmap)
      : PropertyMap(pmap) {}
  };

  template <class Iterator, class PropertyMap>
  dereference_property_map<Iterator,PropertyMap>
  make_dereference_property_map(Iterator const&, PropertyMap pmap) {
    return dereference_property_map<Iterator,PropertyMap>(pmap);
  }
  
  // this comparison object adaptor compares the properties of two objects
  template <class PropertyMap, class Compare
	  = std::less<typename property_traits<PropertyMap>::value_type> >
  struct linear_sort_property_map_cmp {
    typedef typename property_traits<PropertyMap>::key_type key_type;
    typedef key_type first_argument_type;
    typedef key_type second_argument_type;
    typedef bool result_type;
    explicit linear_sort_property_map_cmp(PropertyMap pmap, Compare cmp = Compare())
      : m_pmap(pmap), m_cmp(cmp) {}
    bool operator()(key_type& x, key_type& y) const
      { return m_cmp(m_pmap[x], m_pmap[y]); }
    bool operator()(key_type const& x, key_type const& y) const
      { return m_cmp(m_pmap[x], m_pmap[y]); }
  protected:
    PropertyMap m_pmap;
    Compare     m_cmp;
  };
  
} // namespace boost

#endif
