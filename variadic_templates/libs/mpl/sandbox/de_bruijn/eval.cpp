//Date:
//  2010-09-04
//Purpose:
//  Provide alternative implementation of:
//
//    http://bitbucket.org/camior/de-bruijn-bind/src/tip/include/DeBruijnBind.hpp
//
//  for *types* instead of *values*.
//
//Status(2010-09-20):
//  There a problem with wiki and fusion_composed_op.
//  I suspect the problem is applying eval to 2nd arg
//  of appl.  Instead, I think maybe somehow the
//  arguments should just be substituted in the 1st arg.
//  However, that leaves other problems.
//  Still working on it.
//
#include <boost/mpl/int.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/package.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/print.hpp>
#include <boost/type_traits/is_same.hpp>

using namespace boost;
using namespace mpl;

namespace dbr
/**@brief
 *  Provides all the template classes used
 *  to implement the De Bruijn Index:
 *    http://en.wikipedia.org/wiki/De_Bruijn_index
 *  for *types*.  The code in the DeBruijnBind.hpp 
 *  mentioned above (after //Purpose:) is for *values*.
 */
{
  template
  < typename Repl=package<> //Replacements for varb's
  , unsigned DepthAbstraction=0 //which are bound this high up.
  >
struct args
  /**@brief
   *  Replacement terms for variables bound DepthAbstraction
   *  levels up in the AST (Abstract Syntax Tree).
   */
{};  

  template
  < typename Term
  , typename Args=args<>
  >
struct eval
  /**@brief
   *  metafunction which "evaluates" term, Term, with args, Args.
   *  Defaults to identity metafunction.
   *
   *  Where:
   *    Term = lamb<typename Body>
   *         | varb<unsigned ArgIndex,unsigned DepthBinding>
   *         | Prim<typename... Args>
   *         | appl<typename Fun,typename... Args>
   *         | //anything else
   */
{
    typedef Term type;
};

  template
  < typename Body
  >
struct lamb
  /**@brief
   *  A function abstraction
   *  with body, Body.
   */
{
    typedef lamb type;
};

  template
  < unsigned ArgIndex=0
  , unsigned DepthBinding=0
  >
struct varb
  /**@brief
   *  The ArgIndex-th formal argument (bound variable) to the
   *  lamb (lambda expression) located 
   *  DepthBinding levels up the 'expression tree'.
   *  For example, if lambda is represented
   *  by \ , and the expression tree is:
   *    \(x0,x1,x2)
   *    . \(y0,y1)
   *      . x0(x1,x2,y0,y1)
   *  then this would be expressed as:
   *    lamb//lambda for 3 x's
   *       < lamb //lambda for 2 y's
   *            < appl //apply x0 to (x1,x2,y0,y1)
   *              < varb<0,2> //x0
   *              , varb<1,2> //x1
   *              . varb<2,2> //x2
   *              . varb<0,1> //y0
   *              . varb<1,1> //y1
   *              >
   *            >
   *       >
   *                        
   */
{};

  template
  < unsigned ArgIndex
  , typename Repl
  , unsigned DepthAbstraction
  >
struct eval
  < varb<ArgIndex,DepthAbstraction>
  , args<Repl,DepthAbstraction>
  >
  /**@brief
   *  Return ArgIndex-th arg from
   *  DepthAbstraction levels up in
   *  the Abstraction tree.
   */
  : at_c<Repl,ArgIndex>
{
};

  template
  < unsigned ArgIndex
  >
struct arg_at
  /**@brief
   *  eval
   *  < appl
   *    < arg_at<I>
   *    , A_0,A_1,...,A_n
   *    >
   *  >::type == A_I
   */
{
};
  template
  < unsigned ArgIndex
  , typename Args
  , unsigned DepthAbstraction
  >
struct eval
  < arg_at<ArgIndex>
  , args<Args,DepthAbstraction>
  >
  /**@brief
   *  Return ArgIndex-th arg from
   *  DepthAbstraction levels up in
   *  the Abstraction tree.
   */
  : at_c<Args,ArgIndex>
{
};
  template
  < template<typename...> class Prim //meta function (has typename Prim<Args...>::type ).
  , typename... PArgs//Arguments to Prim.
  , typename ArgsDepth//instance of args
  >
struct eval
  < Prim<PArgs...>
  , ArgsDepth
  >
{
    typedef Prim<typename eval<PArgs,ArgsDepth>::type...> type;
};

  template
  < template<typename...> class Prim
  , typename... PArgs
  , typename Repl
  >
struct eval
  < Prim<PArgs...>
  , args<Repl,0>
  >
  : Prim<typename eval<PArgs,args<Repl,0> >::type...>
{
};

  template
  < typename Body
  , unsigned DepthAbstraction
  , typename Args
  >
struct eval
  < lamb
    < Body
    >
  , args<Args,DepthAbstraction>
  >
  : lamb
    < typename eval
      < Body
      , args<Args,DepthAbstraction+1>
      >::type
    >
{
};

  template
  < typename Body
  , typename Args
  >
struct eval
  < lamb
    < Body
    >
  , args<Args,0>
  >
  : eval
    < Body
    , args<Args,1>
    >
{
};

  template
  < typename Fun
  , typename... Args
  >
struct appl
  /**@brief
   *  Apply function, Fun to args, Args...
   */
{
    typedef appl type;
};
  template
  < typename Fun
  , typename... FArgs
  , typename ArgsD
  >
struct eval
  < appl<Fun,FArgs...>
  , ArgsD
  >
  : appl
    < typename eval<Fun,ArgsD>::type
    , typename eval<FArgs,ArgsD>::type...
    >
{};

  template
  < typename Fun
  , typename... FArgs
  , typename ArgsD
  >
struct eval
  < appl<Fun,FArgs...>
  , args<ArgsD,0>
  >
  : eval
    < Fun
    , args
      < package
      #if 1
        < typename eval<FArgs,args<ArgsD,0> >::type...
      #else
        < FArgs...
      #endif
        >
      , 0
      > 
    >
{};

  template
  < typename Term
  , unsigned Del=1
  >
struct del_depth
  /**@brief
   *  Change all varb<I,Depth> in Term
   *  to varb<I,Depth+Del>.
   *  Used when wrapping Term within a lamb.
   */
{
    typedef Term type;
};

  template
  < template<typename...>class Term
  , typename... Args
  , unsigned Del
  >
struct del_depth
  < Term<Args...>
  , Del
  >
{
        typedef
      Term
      < typename del_depth<Args,Del>::type...
      >
    type
    ;
};
  template
  < unsigned ArgIndex
  , unsigned DepthBinding
  , unsigned Del
  >
struct del_depth
  < varb<ArgIndex,DepthBinding>
  , Del
  >
{
        typedef 
      varb<ArgIndex,DepthBinding+Del>
    type
    ;
};

  template
  < typename Term
  >
struct wrap_lamb
{
        typedef
      lamb
      < typename del_depth
        < Term
        >::type
      >
    type
    ;
};    

}//exit namespace dbr
namespace shorthand
/**@brief
 *  Abbreviations for some templates.
 */
{
      template
      < int I
      >
    struct i
      /**@brief
       *  Shorthand for integral_c<int,I>
       */
    {
        typedef integral_c<int,I> _;
    };
    
}//exit shorthand namespace
#if 1
#include <boost/mpl/next.hpp>
namespace test_basic
{
   using namespace dbr;
   using namespace shorthand;
   
    typedef
  eval
  < varb<0>
  , args<package<int_<0> > >
  >::type
varb_0_int_0_e0
;
BOOST_MPL_ASSERT((is_same<varb_0_int_0_e0,int_<0> >));
    typedef
  eval
  < varb<0>
  , args<package<int_<0> >,1>
  >::type
varb_0_int_0_e1
;
BOOST_MPL_ASSERT((is_same<varb_0_int_0_e1,varb<0> >));
    typedef
  eval
  < lamb<varb<0,1> >
  , args<package<int_<0> > >
  >::type
lamb_varb_01_int_0_e0
;
BOOST_MPL_ASSERT((is_same<lamb_varb_01_int_0_e0,int_<0> >));
    typedef
  eval
  < lamb<lamb<varb<0,2> > >
  , args<package<int_<0> > >
  >::type
lamb_lamb_varb_02_int_0_e0
;
BOOST_MPL_ASSERT((is_same<lamb_lamb_varb_02_int_0_e0,lamb<int_<0> > >));
    typedef
  eval
  < lamb<int_<0> >
  , args<>
  >::type
lamb_int0_edefault
;
BOOST_MPL_ASSERT((is_same<lamb_int0_edefault,int_<0> >));
    typedef
  eval
  < appl
    < lamb<int_<0> >
    >
  , args<>
  >::type
appl_lamb_int0_edefault
;
BOOST_MPL_ASSERT((is_same<appl_lamb_int0_edefault,int_<0> >));
    typedef
  eval
  < appl
    < varb<0>
    , varb<1>
    >
  , args
    < package
      < next<arg_at<0> >
      , int_<9>
      >
    , 0
    >
  >
appl_v0_v1_next_9
;
#if 1
print<appl_v0_v1_next_9> print_appl_v0_v1_next_9;
#else 
BOOST_MPL_ASSERT((equal_to<appl_v0_v1_next_9,int_<10> >));
#endif
  template
  < typename... Args
  >
struct any_tmpl
{
    typedef any_tmpl type;
};
    typedef
  eval
  < any_tmpl
    < varb<0>
    , int_<2>
    , varb<1>
    >
  , args   
    < package
      < int_<10>
      , int_<30>
      >
    >
  >::type
eval_10_2_30
;
BOOST_MPL_ASSERT((is_same<eval_10_2_30,any_tmpl<int_<10>,int_<2>,int_<30> > >));
    typedef
  eval
  < lamb
    < any_tmpl
      < varb<0,1>
      , int_<2>
      , varb<1,1>
      >
    >
  , args   
    < package
      < int_<10>
      , int_<30>
      >
    >
  >::type
abs_eval_10_2_30
;
BOOST_MPL_ASSERT((is_same<abs_eval_10_2_30,any_tmpl<int_<10>,int_<2>,int_<30> > >));

    typedef
  eval
  < lamb
    < any_tmpl
      < varb<0,1>
      , int_<2>
      , varb<0,0>
      >
    >
  , args   
    < package
      < int_<10>
      >
    >
  >::type
cur_eval_10_2_30
;
BOOST_MPL_ASSERT((is_same<cur_eval_10_2_30,any_tmpl<int_<10>,int_<2>,varb<0> > >));
   
    typedef
  eval
  < cur_eval_10_2_30
  , args   
    < package
      < int_<30>
      >
    >
  >::type
eval_cur_eval_10_2_30
;
BOOST_MPL_ASSERT((is_same<eval_cur_eval_10_2_30,any_tmpl<int_<10>,int_<2>,int_<30> > >));
    typedef
  del_depth
  < varb<0,0>
  >::type
del_v00_1
;
BOOST_MPL_ASSERT((is_same<del_v00_1,varb<0,1> >));

    typedef
  del_depth
  < any_tmpl<varb<0,0> >
  >::type
del_any_v00_1
;
BOOST_MPL_ASSERT((is_same<del_any_v00_1,any_tmpl<varb<0,1> > >));
}
#endif //test_basic

#if 0
namespace wiki
/**@brief
 *  Problem found in [wiki]:
 *    http://en.wikipedia.org/wiki/De_Bruijn_index
 *  just after:
 *    "following term written in usual notation"
 *  that term being:
 *    (\x. \y. z x (\u. u x)) (\x. w x)
 *  which should beta-reduce to:
 *    \y. z (\x. w x) (\u. u (\x. w x))
 */
{
   using namespace dbr;
   
    template
    < typename //arg0
    >
  struct w
  {
      typedef w type;
  };
    template
    < typename //arg0
    , typename //arg1
    >
  struct z
  {
      typedef z type;
  };

    typedef
  lamb //lam[1]:\x
  < lamb //lam[2]:\y
    < z
      < varb<0,2> //x, bound by lam[1].
      , lamb //lam[3]:\u
        < appl
          < varb<0,1> //u, bound by lam[3].
          , varb<0,3> //x, bound by lam[1].
          >
        >//\u.u x
      >//z x (\u.u x)
    >//\y.z x (\u.u x)
  >//(\x.\y.z x (\u.u x))
lamb_x_lamb_y_zx_lamb_u_ux
;
//print<lamb_x_lamb_y_zx_lamb_u_ux> print_lamb_x_lamb_y_zx_lamb_u_u;
    typedef
  lamb //lam[1]:\x
  < w
    < varb<0,1> //x, bound by lam[1].
    >
  > //(\x. w x)
x_arg
;
    typedef
  eval
  < lamb_x_lamb_y_zx_lamb_u_ux
  , args<package<x_arg> >
  >::type
eval_lamb_x_lamb_y_zx_lamb_u_ux_x_arg
;
//print<eval_lamb_x_lamb_y_zx_lamb_u_ux_x_arg> print_eval_lamb_x_lamb_y_zx_lamb_u_ux_x_arg;
    typedef
  lamb //lamb[1]:\y
  < z
    < lamb //lamb[2]:\x
      < w
        < varb<0u, 1u> //x, bound by lam[2]
        > 
      >
    , lamb //lamb[3]:\u
      < appl
        < varb<0u, 1u> //u, bound by lam[3]
        , lamb //lamb[4]:\x
          < w
            < varb<0u, 1u> //x, bound by lam[4]
            > 
          > 
        > 
      > 
    > 
  >// \y. z (\x. w x) (\u. u (\x. w x))
expected_eval_lamb_x_lamb_y_zx_lamb_u_ux_x_arg
;
BOOST_MPL_ASSERT
((is_same
  <          eval_lamb_x_lamb_y_zx_lamb_u_ux_x_arg
  , expected_eval_lamb_x_lamb_y_zx_lamb_u_ux_x_arg
  >
));
    typedef
  appl
  < lamb_x_lamb_y_zx_lamb_u_ux
  , x_arg
  >
appl_lamb_x_lamb_y_zx_lamb_u_ux_x_arg
;
//print<appl_lamb_x_lamb_y_zx_lamb_u_ux_x_arg> print_appl_lamb_x_lamb_y_zx_lamb_u_ux_x_arg;
    typedef
  eval
  < appl_lamb_x_lamb_y_zx_lamb_u_ux_x_arg
  >::type
eval_appl_lamb_x_lamb_y_zx_lamb_u_ux_x_arg
;
//print<eval_appl_lamb_x_lamb_y_zx_lamb_u_ux_x_arg> print_eval_appl_lamb_x_lamb_y_zx_lamb_u_ux_x_arg;
BOOST_MPL_ASSERT
((is_same
  <     eval_appl_lamb_x_lamb_y_zx_lamb_u_ux_x_arg
  , expected_eval_lamb_x_lamb_y_zx_lamb_u_ux_x_arg
  >
));
      
}
#endif //#if namespace wiki

#if 0
#include <boost/mpl/plus.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>
namespace fold_composed_op
{
/**@brief
 *  The is the real problem which prompted creation of
 *  the simplified problem in [apply_apply].
 */
   using namespace dbr;
   using namespace shorthand;
   
      template
      < typename Op
      , typename StateNow
      , typename Package
      >
    struct fold_pack
    ;
      template
      < typename Op
      , typename StateNow
      >
    struct fold_pack
      < Op
      , StateNow
      , package<>
      >
    {
        typedef StateNow type;
    };
      template
      < typename Op
      , typename StateNow
      , typename ArgHead
      , typename... ArgsTail
      >
    struct fold_pack
      < Op
      , StateNow
      , package<ArgHead,ArgsTail...>
      >
    : fold_pack
      < Op
      , typename eval<appl<Op,ArgHead,StateNow> >::type
      , package<ArgsTail...>
      >
    {
    };
    
        typedef
      plus<varb<0>,varb<1> >
    op0
    ;
        typedef
      i<100>::_
    state_now0
    ;
        typedef
      package
      < i<1>::_
      , i<2>::_
      , i<3>::_
      >
    ints
    ;
        typedef
      fold_pack
      < op0
      , state_now0
      , ints
      >::type
    plus_reduce_123
    ;
     BOOST_MPL_ASSERT
     ((equal_to
       < plus_reduce_123
       , i<106>::_
       >
     ));
     
        typedef
      wrap_lamb<op0>::type
    lamb_op0
    ;
        typedef
      push_front
      < varb<1>
      , appl
        < lamb_op0
        , varb<0>
        , front<varb<1> >
        >
      >
    op1
    ;
        typedef
      package<state_now0>
    state_now1
    ;
      
        typedef
      eval
      < appl
        < front<varb<0> >
        , state_now1
        >
      >::type
    appl_front_now1
    ;
    BOOST_MPL_ASSERT
      ((equal_to
        < appl_front_now1
        , state_now0
        >
      ));
      
        typedef
      eval
      < appl
        < lamb_op0
        , i<9>::_
        , i<1>::_
        >
      >::type
    appl_lamb_op0_9_1
    ;
    BOOST_MPL_ASSERT
      ((equal_to
        < appl_lamb_op0_9_1
        , i<10>::_
        >
      ));
      
        typedef
      eval
      < appl
        < op1
        , i<1>::_
        , state_now1
        >
      >::type
    op1_state1
    ;
    BOOST_MPL_ASSERT
      ((equal
        < op1_state1
        , package<i<101>::_,i<100>::_>
        >
      ));
      
        typedef
      eval
      < appl
        < op1
        , i<2>::_
        , op1_state1
        >
      >::type
    op1_state2
    ;
    BOOST_MPL_ASSERT
      ((equal
        < op1_state2
        , package<i<103>::_,i<101>::_,i<100>::_>
        >
      ));
      
        typedef
      fold_pack
      < op1
      , state_now1
      , ints
      >::type
    plus_scan_123
    ;
    BOOST_MPL_ASSERT
      ((equal
        < plus_scan_123
        , package<i<106>::_,i<103>::_,i<101>::_,i<100>::_>
        >
      ));

}
#endif //#if namespace fold_composed_op
