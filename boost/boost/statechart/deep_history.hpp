#ifndef BOOST_FSM_DEEP_HISTORY_HPP_INCLUDED
#define BOOST_FSM_DEEP_HISTORY_HPP_INCLUDED
//////////////////////////////////////////////////////////////////////////////
// (c) Copyright Andreas Huber Doenni 2002-2004
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



#include <boost/mpl/bool.hpp>
#include <boost/static_assert.hpp>



namespace boost
{
namespace fsm
{

  
  
template< class DefaultState >
class deep_history
{
  public:
    // If you receive a 
    // "use of undefined type 'boost::STATIC_ASSERTION_FAILURE<x>'" or similar
    // compiler error here then you forgot to pass either
    // fsm::has_deep_history or fsm::has_full_history as the last parameter of
    // DefaultState's context.
    BOOST_STATIC_ASSERT( DefaultState::context_type::deep_history::value );

    //////////////////////////////////////////////////////////////////////////
    // The following declarations should be private.
    // They are only public because many compilers lack template friends.
    //////////////////////////////////////////////////////////////////////////
    typedef typename DefaultState::outermost_context_type
      outermost_context_type;
    typedef typename DefaultState::context_type context_type;
    typedef typename DefaultState::context_ptr_type context_ptr_type;
    typedef typename DefaultState::context_type_list context_type_list;
    typedef typename DefaultState::orthogonal_position orthogonal_position;

    typedef mpl::bool_< true > history_destination;

    static void deep_construct(
      const context_ptr_type & pContext,
      outermost_context_type & outermostContext )
    {
      outermostContext.template construct_with_deep_history< DefaultState >(
        pContext );
    }
};



} // namespace fsm
} // namespace boost



#endif
