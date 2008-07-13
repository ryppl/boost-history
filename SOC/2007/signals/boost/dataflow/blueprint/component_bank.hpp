// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_COMPONENT_BANK_HPP
#define BOOST_DATAFLOW_BLUEPRINT_COMPONENT_BANK_HPP

#include <boost/dataflow/blueprint/component.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/functional/factory.hpp>

#include <map>
#include <memory>

namespace boost { namespace dataflow { namespace blueprint {

template<typename Map>
class key_iterator
    : public boost::iterator_adaptor<
        key_iterator<Map>                    // Derived
      , typename Map::const_iterator
      , const typename Map::key_type              // Value
      , boost::use_default
    >
{
public:
    key_iterator()
      : key_iterator::iterator_adaptor_() {}

    explicit key_iterator(typename Map::const_iterator it)
      : key_iterator::iterator_adaptor_(it) {}

    typename key_iterator::reference dereference() const
    {
        return key_iterator::base_reference()->first;
    }

 private:
    friend class boost::iterator_core_access;
};

class component_bank
{
    typedef std::map<std::string, boost::function<component * ()> > map_type;
public:
    typedef key_iterator<map_type> iterator;
    
    std::auto_ptr<component> make(const std::string &key)
    {
        std::auto_ptr<component> ret(m_components[key]());
        return ret;
    }
    key_iterator<map_type> begin()
    {   return key_iterator<map_type>(m_components.begin()); }
    key_iterator<map_type> end()
    {   return key_iterator<map_type>(m_components.end()); }
    boost::function<component * ()> operator[](const std::string& k)
    {   return m_components[k]; }
protected:
    map_type m_components;
};

template<typename Tag>
class tag_component_bank : public component_bank
{
public:
    template<typename T>
    void add_component(const std::string &s)
    {
        m_components[s] = boost::factory<component_t<T, Tag> *>();
    }
    template<typename T, typename T0>
    void add_component(const std::string &s, const T0 &t0)
    {
        m_components[s] = boost::bind(boost::factory<component_t<T, Tag> *>(), t0);
    }
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_COMPONENT_BANK_HPP

