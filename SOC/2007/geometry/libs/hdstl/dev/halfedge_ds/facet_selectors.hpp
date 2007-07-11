//facet_selectors.hpp   -*- C++ -*-
//
//@PURPOSE: Provide selector classes for 'FacetS'.
//
//@CLASSES:
//  'facetS':  defines configuration options for facet access
//  
//@SEE_ALSO: {hds_concepts.hpp, halfedge_ds.hpp}
//
//@DESCRIPTION: This file provides the 'facetS' selector class for
//configurations related to facet properties. 

#ifndef BOOST_HDSTL_FACET_SELECTORS_HPP
#define BOOST_HDSTL_FACET_SELECTORS_HPP 1

namespace boost;
namespace hdstl;

template<typename Selector>
struct facetS {
    typedef false_type is_rand_access;
};

template<>
struct facets<vecS> {
    typedef true_type is_rand_access;
};

} // namespace hdstl
} // namespace boost

#endif
