#ifndef BOOST_FSM_DETAIL_NODE_STATE_HPP_INCLUDED
#define BOOST_FSM_DETAIL_NODE_STATE_HPP_INCLUDED
//////////////////////////////////////////////////////////////////////////////
// Copyright (c) Andreas Huber Doenni 2002-2004.
// Use, modification and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



#include <boost/fsm/detail/state_base.hpp>

#include <boost/assert.hpp>  // BOOST_ASSERT



namespace boost
{
namespace fsm
{
namespace detail
{



//////////////////////////////////////////////////////////////////////////////
template< class NoOfOrthogonalRegions, class Allocator, class RttiPolicy >
class node_state : public state_base< Allocator, RttiPolicy >
{
  typedef state_base< Allocator, RttiPolicy > base_type;
  protected:
    //////////////////////////////////////////////////////////////////////////
    node_state(
      typename RttiPolicy::id_provider_type idProvider
    ) :
      base_type( idProvider )
    {
      for ( orthogonal_position_type pos = 0; 
            pos < NoOfOrthogonalRegions::value; ++pos )
      {
        pInnerStates[ pos ] = 0;
      }
    }

    virtual ~node_state() {}

  public:
    //////////////////////////////////////////////////////////////////////////
    // The following declarations should be private.
    // They are only public because many compilers lack template friends.
    //////////////////////////////////////////////////////////////////////////
    void add_inner_state( orthogonal_position_type position,
                          base_type * pInnerState )
    {
      BOOST_ASSERT( ( position < NoOfOrthogonalRegions::value ) &&
                    ( pInnerStates[ position ] == 0 ) );
      pInnerStates[ position ] = pInnerState;
    }

    void remove_inner_state( orthogonal_position_type position )
    {
      BOOST_ASSERT( position < NoOfOrthogonalRegions::value );
      pInnerStates[ position ] = 0;
    }

    typedef typename base_type::state_list_type state_list_type;

    virtual void remove_from_state_list(
      state_list_type & states,
      typename state_list_type::iterator & pUnstableState )
    {
      if ( ( pUnstableState != states.end() ) &&
           ( get_pointer( *pUnstableState ) == this ) )
      {
        for ( base_type ** pState = &pInnerStates[ 0 ]; 
              pState != &pInnerStates[ NoOfOrthogonalRegions::value ]; ++pState )
        {
          BOOST_ASSERT( *pState == 0 );
        }

        states.erase( pUnstableState );
        pUnstableState = states.end();
      }
      else
      {
        // Destroy inner states in the reverse order of construction
        for ( base_type ** pState = &pInnerStates[ NoOfOrthogonalRegions::value ]; 
              pState != &pInnerStates[ 0 ]; )
        {
          --pState;
          // An inner orthogonal state might have been terminated long before,
          // that's why we have to check for 0 pointers
          if ( *pState != 0 )
          {
            ( *pState )->remove_from_state_list( states, pUnstableState );
          }
        }
      }
    }

  private:
    //////////////////////////////////////////////////////////////////////////
    base_type * pInnerStates[ NoOfOrthogonalRegions::value ];
};



} // namespace detail
} // namespace fsm
} // namespace boost



#endif
