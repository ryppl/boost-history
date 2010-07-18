//
//ModificationHistor:
//  2010/07/09
//    cp'ed from 
//      http://svn.boost.org/svn/boost/sandbox/variadic_templates/boost/mpl/if_recur.hpp
//  2010/07/10-?
//    adapted to fusion's needs, i.e. operates on values instead of types.
//
#ifndef BOOST_FUSION_IF_RECUR_HPP_20100709_1405UTC
#define BOOST_FUSION_IF_RECUR_HPP_20100709_1405UTC
// Copyright Larry Evans 2010
//

// $Id$
// $Date: 2010/07/18 09:35:28 $
// $Revision: 1.30 $

#include <boost/utility/result_of.hpp>
#include <boost/mpl/eval_if.hpp>

namespace boost
{
namespace fusion
{
namespace aux
{
namespace if_recur
{
      enum
    recur_kind
    { recur_type  //Recur arg to if_recur tests types.
    , recur_value //Recur arg to if_recur tests values.
    };
    
      template
      < class State
      , bool Result=false
      , recur_kind RecurKind=recur_value
      >
    struct always
    {
            static
          recur_kind const
        our_recur=RecurKind
        ;
            typedef 
          bool 
        result_type
        ;
            static
          result_type
        call
          ( State const& state
          )
        {
            return Result;
        }
    };
      template
      < class State
      >
    struct identity
    {
            typedef 
          State
        result_type
        ;
            static
          State
        call
          ( State const& state
          )
        {
            return state;
        }
    };
      template
      < unsigned Index
      , typename... Args
      >
    struct at_c
    ;
      template
      < unsigned Index
      , typename Head
      , typename... Tail
      >
    struct at_c
      < Index
      , Head
      , Tail...
      >
    {
            typedef
          at_c
          < Index-1
          , Tail...
          >
        tail_at_c
        ;
            typedef
          typename tail_at_c::result_type
        result_type
        ;
            static
          result_type
        call
          ( Head const& head
          , Tail const&... tail
          )
        {
            return tail_at_c::call(tail...);
        }
    };
      template
      < typename Head
      , typename... Tail
      >
    struct at_c
      < 0
      , Head
      , Tail...
      >
    {
            typedef
          Head
        result_type
        ;
            static  
          Head
        call
          ( Head const& head
          , Tail const&...
          )
        {
            return head;
        }
        
    };
    
    namespace detail//just to emphasize it's not for user
    {
          template
          < class StateNow
          , class Recur
          , class ThenDown
          , class NowNow
          , class NowUp
          , class ElseBtm
          , recur_kind RecurKind
          >
        struct impl //implements the if_result template
        ;
          template
          < class StateNow
          , class Recur
          , class ThenDown
          , class NowNow
          , class NowUp
          , class ElseBtm
          >
        struct impl
          < StateNow
          , Recur
          , ThenDown
          , NowNow
          , NowUp
          , ElseBtm
          , recur_value
          >
        {
                typedef        
              typename boost::result_of
              < ElseBtm(StateNow)
              >::type
            result_type
            ;
              
                static
              result_type
            call
              ( StateNow const& state_now
              )
            {
              #ifdef IF_RECUR_TRACE
                indent_undent_cout iuc;
              #endif
                if(Recur::call(state_now))
                {
                    return NowUp::call
                    ( NowNow::call(state_now)
                    , call
                      ( ThenDown::call(state_now)
                      )
                    );
                }
                else
                {
                    return ElseBtm::call(state_now);
                }
            }
        };
        
          template
          < class StateNow
          , class Recur
          , class ThenDown
          , class NowNow
          , class NowUp
          , class ElseBtm
          >
        struct impl
          < StateNow
          , Recur
          , ThenDown
          , NowNow
          , NowUp
          , ElseBtm
          , recur_type
          >
        {
                       
          //#define SHOW_NOW_UP_LAZY_NEED
          #ifndef SHOW_NOW_UP_LAZY_NEED
              template
              < typename StateLazy
              >
            struct result_of
            ;
            
              template
              < typename StateLazy
              >
            struct now_up_lazy
            {
                    typedef
                  typename NowUp::template result_of
                  < typename NowNow::template result_of
                    < typename StateLazy::type
                    >::type
                  , typename impl::template result_of
                    < typename ThenDown::template result_of
                      < typename StateLazy::type
                      >
                    >::type
                  >::type
                type
                ;
            };
          #endif
              template
              < typename StateLazy
              >
            struct result_of
            : mpl::eval_if
              < typename Recur::template result_of
                < typename StateLazy::type
                >
            #ifndef SHOW_NOW_UP_LAZY_NEED
              , now_up_lazy<StateLazy>
            #else
              ,   typename NowUp::template result_of
                  < typename NowNow::template result_of
                    < typename StateLazy::type
                    >::type
                  , typename impl::template result_of
                    < typename ThenDown::template result_of
                      < typename StateLazy::type
                      >
                    >::type
                  >
            #endif
              , typename ElseBtm::template result_of
                < typename StateLazy::type
                >
              >
            {};
            
                typedef 
            #if 1
                typename
              result_of
              < mpl::identity<StateNow>
              >::type
            #else
              StateNow
            #endif
            result_type
            ;
                static
              result_type
            call
              ( StateNow const& state_now
              )
            ;
         private:
                static
              result_type
            call_recur
              ( StateNow const& state_now
              , mpl::true_
              )
            {
                typedef typename ThenDown::template result_of<StateNow>::type state_next;
                return NowUp::call
                ( NowNow::call(state_now)
                , impl
                  < state_next
                  , Recur
                  , ThenDown
                  , NowNow
                  , NowUp
                  , ElseBtm
                  , recur_type
                  >
                  ::call
                  ( ThenDown::call(state_now)
                  )
                );
            }
                static
              result_type
            call_recur
              ( StateNow const& state_now
              , mpl::false_
              )
            {
                return ElseBtm::call(state_now);
            }
        };
        
          template
          < class StateNow
          , class Recur
          , class ThenDown
          , class NowNow
          , class NowUp
          , class ElseBtm
          >
        typename impl
          < StateNow
          , Recur
          , ThenDown
          , NowNow
          , NowUp
          , ElseBtm
          , recur_type
          >::
              result_type
        impl
          < StateNow
          , Recur
          , ThenDown
          , NowNow
          , NowUp
          , ElseBtm
          , recur_type
          >::
            call
              ( StateNow const& state_now
              )
            {
              #ifdef IF_RECUR_TRACE
                indent_undent_cout iuc;
              #endif
                typename Recur::template result_of<StateNow>::type recur;
                return call_recur
                ( state_now
                , recur
                );
            }
          
    }//exit detail namespace
    
}//exit if_recur namespace    
}//exit aux namespace    
    
    //Notation:
    //
    //  X -> Y = function from "type" X to "type" Y.
    //
    //  State = some Kind of "state"
    //Purpose:
    //  Almost the same as section 12.5 equation 1) of:
    //    http://www.thocp.net/biographies/papers/backus_turingaward_lecture.pdf
    //  The correspondences between this and that equaton is:
    //
    //    this              equation 1)
    //    ----              -----------
    //    if_recur          f
    //    !Recur            p
    //    ThenDown          j
    //    NowNow            i
    //    NowUp             h
    //    ElseBtm           g
    //
    
      template
      < class StateNow//current state
      , class Recur=aux::if_recur::always<StateNow>//::call: StateNow -> bool.
      , class ThenDown=aux::if_recur::identity<StateNow>//::call: StateNow -> StateNow.
      , class NowNow=aux::if_recur::identity<StateNow>//::call: StateNow -> StateNow.
      , class NowUp=aux::if_recur::at_c<1,StateNow,StateNow>//::call: (StateNow,StateNow) -> StateNow.
      , class ElseBtm=aux::if_recur::identity<StateNow>//::call: StateNow -> StateNow.
      >
    struct if_recur
    : aux::if_recur::detail::impl
      < StateNow
      , Recur
      , ThenDown
      , NowNow
      , NowUp
      , ElseBtm
      , Recur::our_recur
      >
    {
    };
    
}//exit fusion namespace
}//exit boost namespace
#endif
