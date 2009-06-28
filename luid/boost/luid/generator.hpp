//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/luid for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_LUID_GENERATOR_HPP_
#define BOOST_LUID_GENERATOR_HPP_

#include "boost/luid/dsl.hpp"
#include "boost/luid/iccl.hpp"

//#include "boost/luid/generator/dsl_parser.hpp"
//#include "boost/luid/generator/dsl_assign_defaults.hpp"
#include "boost/luid/generator/assemble_components.hpp"
#include "boost/mpl/if.hpp"

namespace boost { namespace luid {
//enum to_do{
//    do_all,
//    default_and_assemble,
//    assemble_the_components        
//};

/*
InputDsl is based on a Boost.Parameter 

*/
   
template< typename InputDsl = dsl::luidg<>
//, to_do WhatToDo = do_all 
>
struct generator {

//    typedef typename mpl::if_c<(WhatToDo==do_all)
//            , InputDsl
//            , dsl::luidg<>
//         >::type dsl_description;
    
//    typedef typename detail::dsl_parser<InputDsl>::type parsed_dsl__;
    
//    typedef typename mpl::if_c<WhatToDo==default_and_assemble
//            , InputDsl
//            , parsed_dsl__
//         >::type parsed_dsl_;
//    
//    typedef typename dsl_assign_defaults<parsed_dsl_>::type complete_dsl__;

//    typedef typename mpl::if_c<WhatToDo==assemble_the_components
//            , InputDsl
//            , parsed_dsl__
//         >::type complete_dsl_;

    typedef typename detail::assemble_components<InputDsl>::type  result;
    public:
    typedef result type;
    
};

template <
 typename Arg1=parameter::void_,
 typename Arg2=parameter::void_,
 typename Arg3=parameter::void_,
 typename Arg4=parameter::void_,
 typename Arg5=parameter::void_,
 typename Arg6=parameter::void_,
 typename Arg7=parameter::void_,
 typename Arg8=parameter::void_
     > 
 struct make_luidg {
     typedef dsl::luidg<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8> luidg;
     typedef typename generator<luidg>::type type;  
 };
 
 template <
  typename Arg1=parameter::void_,
  typename Arg2=parameter::void_,
  typename Arg3=parameter::void_,
  typename Arg4=parameter::void_,
  typename Arg5=parameter::void_,
  typename Arg6=parameter::void_,
  typename Arg7=parameter::void_,
  typename Arg8=parameter::void_
      > 
  struct luidg : make_luidg<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8>::type 
  {
          typedef typename make_luidg<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8>::type super_type;
        template <typename ArgsParam> 
        luidg(const ArgsParam& args) 
        : super_type(args)
        {
        }
      
//        //! constructor with name parameters
//        BOOST_PARAMETER_CONSTRUCTOR(
//                luidg, (super_type), tag
//          , (optional (low,*)) (optional (high,*)) 
//              (optional (delay,*)) 
//              (optional (shm_name,*)) (optional (shm_size,*))
//              (optional (ext_mtx,*)) 
//        ) // no semicolon
  };
}
}

#endif // BOOST_LUID_GENERATOR_HPP_
