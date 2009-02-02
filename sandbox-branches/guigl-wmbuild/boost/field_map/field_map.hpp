/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__FIELD_MAP__FIELD_MAP_HPP
#define BOOST__FIELD_MAP__FIELD_MAP_HPP


#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/algorithm/transformation/join.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/sequence/intrinsic/has_key.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/utility/enable_if.hpp>

#include <string>


namespace boost { namespace field_map {

namespace detail {

    template<typename Field>
    struct pair
    {
        typedef fusion::pair<Field, typename Field::type> type;
    };

    template<>
    struct pair<fusion::void_>
    {
        typedef fusion::void_ type;
    };

    template<typename SourceMap>
    struct copy
    {
        copy(const SourceMap &source)
            : source(source)
        {}
        
        template<typename DestPair>
        typename disable_if<
            fusion::result_of::has_key<SourceMap, typename DestPair::first_type>
        >::type operator()(DestPair &dest) const
        {
            typedef typename DestPair::first_type field;
            dest = fusion::make_pair<field>(field::default_value());
        }

        template<typename DestPair>
        typename enable_if<
            fusion::result_of::has_key<SourceMap, typename DestPair::first_type>
        >::type operator()(DestPair &dest) const
        {
            typedef typename DestPair::first_type key;
            dest = fusion::make_pair<key>(fusion::at_key<key>(source));
        }
        
        const SourceMap &source;
    };

}

template<typename Map>
class field_map
{
public:
    typedef Map map_type;
    
    // all default values - todo
    field_map()
    {
    }
    
    // initialize from a fusion map
    field_map(const Map &map)
        : m_map(map)
    {}
    
    // get values from a different FieldMap
    template<typename OtherMap>
    field_map(const field_map<OtherMap> &other_field_map)
    {
        fusion::for_each(m_map, detail::copy<OtherMap>(other_field_map.map()));
    }
    
    template<typename Field>
    const typename Field::type &at() const
    {   return fusion::at_key<Field>(m_map); }

    template<typename FieldObject>
    const typename FieldObject::field_type::type &operator[](const FieldObject &) const
    {   return fusion::at_key<typename FieldObject::field_type>(m_map); }

    const Map &map() const
    {   return m_map; }
private:
    Map m_map;
};

namespace detail {

    template<typename LeftFieldMap, typename RightFieldMap>
    struct join_map
    {
        typedef field_map<
            typename 
                fusion::result_of::as_map<
                    typename fusion::result_of::join<
                        typename LeftFieldMap::map_type,
                        typename RightFieldMap::map_type
                    >::type
                >::type
            > type;
    };
}

template<typename LeftFieldMap, typename RightFieldMap>
typename detail::join_map<LeftFieldMap, RightFieldMap>::type
    operator, (const LeftFieldMap &lhs, const RightFieldMap &rhs)
{
    return typename detail::join_map<LeftFieldMap, RightFieldMap>::type(fusion::join(lhs.map(), rhs.map()));
}

template<typename LeftFieldMap, typename RightFieldMap>
typename detail::join_map<LeftFieldMap, RightFieldMap>::type
    operator& (const LeftFieldMap &lhs, const RightFieldMap &rhs)
{
    return (lhs,rhs);
}

#ifndef BOOST_FIELDMAP_MAX_ARGS
#define BOOST_FIELDMAP_MAX_ARGS 9
#endif

#define BOOST_FIELDMAP_PACK_TYPENAME(z,n,text) typename T##n=fusion::void_
#define BOOST_FIELDMAP_PACK_FIELD_TYPE(z,n,text) typename detail::pair<T##n>::type

template<BOOST_PP_ENUM(BOOST_FIELDMAP_MAX_ARGS, BOOST_FIELDMAP_PACK_TYPENAME, _)>
struct map
{
    typedef field_map<fusion::map<BOOST_PP_ENUM(BOOST_FIELDMAP_MAX_ARGS, BOOST_FIELDMAP_PACK_FIELD_TYPE, _)> > type;
};

typedef map<>::type empty_map_type;


#define BOOST_FIELD_MAP_FIELD_FUNCTION(field,function) \
map<field>::type function(const field::type &value) \
{ \
    return map<field>::type(value); \
}

#define BOOST_FIELD_MAP_FIELD_ASSIGNABLE(field,assigner) \
namespace { \
struct BOOST_PP_CAT(assigner,_type) \
{ \
    map<field>::type operator=(const field::type &value) const \
    { \
        return map<field>::type(value); \
    } \
}; } \
static const BOOST_PP_CAT(assigner,_type) assigner = BOOST_PP_CAT(assigner,_type)();

#define BOOST_FIELD_MAP_PARAMETER(field_name,param_type) \
namespace field { \
    struct field_name { \
        typedef param_type type; \
    }; \
} \
namespace { \
struct BOOST_PP_CAT(field_name,_type) \
{ \
    typedef field::field_name field_type; \
    map<field::field_name>::type operator=(const field::field_name::type &value) const \
    { \
        return map<field::field_name>::type(value); \
    } \
}; } \
static const BOOST_PP_CAT(field_name,_type) field_name = BOOST_PP_CAT(field_name,_type)();

}}

#endif // BOOST__FIELD_MAP__FIELD_MAP_HPP