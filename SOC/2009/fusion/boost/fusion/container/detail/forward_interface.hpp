/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
 
#if !defined(BOOST_FUSION_SEQ_NAME) && !defined(BOOST_FUSION_CONTAINER_DETAIL_FORWARD_INTERFACE_HPP)
#   define BOOST_FUSION_CONTAINER_DETAIL_FORWARD_INTERFACE_HPP
#   include <boost/config.hpp>
#endif

#ifndef BOOST_FUSION_SEQ_NAME
#   ifdef BOOST_NO_VARIADIC_TEMPLATES
#       include <boost/fusion/container/detail/pp/forward_interface.hpp>
#   else
#       include <boost/fusion/container/detail/variadic_templates/forward_interface.hpp>
#   endif
#else

#   ifdef BOOST_FUSION_USE_BASE_TYPE
#       define BOOST_FUSION_SEQ_CTOR(MODIFIER,_)\
        BOOST_FUSION_SEQ_NAME(BOOST_FUSION_SEQ_NAME MODIFIER seq)\
          : base_type(static_cast<base_type MODIFIER>(seq))\
        {}
#   else
#       define BOOST_FUSION_SEQ_CTOR(MODIFIER,_)\
        BOOST_FUSION_SEQ_NAME(BOOST_FUSION_SEQ_NAME MODIFIER seq)\
          : data(static_cast<BOOST_FUSION_SEQ_NAME MODIFIER>(seq).data)\
        {}
#   endif

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_SEQ_CTOR,_)

#   undef BOOST_FUSION_SEQ_CTOR

#   ifdef BOOST_FUSION_USE_BASE_TYPE
#       define BOOST_FUSION_INIT_BASE base_type
#   else
#       define BOOST_FUSION_INIT_BASE data
#   endif

#   ifdef BOOST_NO_VARIADIC_TEMPLATES
#       include <boost/fusion/container/detail/pp/forward_interface.hpp>
#   else
#       include <boost/fusion/container/detail/variadic_templates/forward_interface.hpp>
#   endif

#ifdef BOOST_NO_RVALUE_REFERENCES
    template<typename Arg>
    BOOST_FUSION_SEQ_NAME(BOOST_FUSION_R_ELSE_CLREF(Arg) arg)
      : BOOST_FUSION_INIT_BASE(BOOST_FUSION_FORWARD(Arg, arg))
    {}
#endif

#   undef BOOST_FUSION_INIT_BASE

        template<typename Seq>
        BOOST_FUSION_SEQ_NAME&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
#   ifdef BOOST_FUSION_USE_BASE_TYPE
            *static_cast<base_type*>(this) = BOOST_FUSION_FORWARD(Seq, seq);
#   else
            data = BOOST_FUSION_FORWARD(Seq, seq);
#   endif
            return *this;
        }
#endif

