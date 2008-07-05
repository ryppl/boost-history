
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_EDGE_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_EDGE_HPP

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The edge template is the base class of different edge types for adjacency
 * lists. It essentially provides common functionality for associating
 * user-defined properties with edge structures.
 */
template <typename EdgeProps>
class edge
{
public:
    typedef EdgeProps properties_type;

    edge();
    edge(properties_type const& ep);

    // Property accessors...
    properties_type* operator->();
    properties_type& operator*();
    properties_type const& operator*() const;

private:
    properties_type _props;
};

// Functions

template <typename EP>
edge<EP>::edge()
    : _props()
{ }

template <typename EP>
edge<EP>::edge(properties_type const& ep)
    : _props(ep)
{ }

template <typename VP>
typename edge<VP>::properties_type*
edge<VP>::operator->()
{ return &_props; }

template <typename VP>
typename edge<VP>::properties_type&
edge<VP>::operator*()
{ return _props; }

template <typename VP>
typename edge<VP>::properties_type const&
edge<VP>::operator*() const
{ return _props; }

} /* namespace adjacency_lists */
} /* namespace graphs */
} /* namespace boost */

#endif

