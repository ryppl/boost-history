//package template "operators"
#ifndef BOOST_MPL_PACKAGE_PACKAGE0_HPP_INCLUDED
#define BOOST_MPL_PACKAGE_PACKAGE0_HPP_INCLUDED
//  (C) Copyright Larry Evans 2007.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/clear.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/iterator_category.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/package/aux_/tag.hpp>
#include <boost/mpl/package_fwd.hpp>

namespace boost
{
namespace mpl
{
    namespace variadic
    //!
    //!@brief
    //!  Super templae classes, impl_OP, for
    //!  mpl::OP_impl::apply classes
    //!  for various values of OP 
    //!  (e.g. size, front, etc.).
    //!
    //!  These helper classes are needed because, for some reason, 
    //!  specializations of mpl::OP_impl::apply, as done for
    //!  these helper classes, does not work.
    //!
    {
          template
          < typename Package 
          >
          struct
        impl_size
        ;
          template
          < typename... Args
          >
          struct
        impl_size
          < package
            < Args...
            >
          >
        : integral_c
          < long
          , sizeof...(Args)
          >
        {
        };
        
          template
          < typename Package
          , long N
          >
          struct
        impl_at_c
        ;
          template
          < typename Head
          , typename... Tail
          , long N
          >
          struct
        impl_at_c
          < package<Head, Tail...>
          , N
          >
          : impl_at_c<package<Tail...>, N-1>
        {  
        };
          template
          < typename Head
          , typename... Tail
          >
          struct
        impl_at_c
          < package<Head, Tail...>
          , 0
          >
        {
                typedef
              Head
            type
            ;
        };
        
          template
          < typename Package
          >
          struct
        impl_front
        ;
          template
          < typename Head
          , typename... Tail
          >
          struct
        impl_front
          < package
            < Head
            , Tail...
            >
          >
        {  
                typedef
              Head
            type
            ;
        };
        
          template
          < typename Package
          , typename Head
          >
          struct
        impl_push_front
        ;
          template
          < typename... Tail
          , typename Head
          , template
            < typename...
            >class Package
          >
          struct
        impl_push_front
          < Package
            < Tail...
            >
          , Head
          >
        {  
                typedef
              Package
              < Head
              , Tail...
              >
            type
            ;
        };
        
          template
          < typename Package
          >
          struct
        impl_pop_front
        ;
          template
          < typename... Tail
          , typename Head
          , template
            < typename...
            >class Package
          >
          struct
        impl_pop_front
          < Package
            < Head
            , Tail...
            >
          >
        {  
                typedef
              Package
              < Tail...
              >
            type
            ;
        };
        
          template
          < typename Package
          , typename Tail
          >
          struct
        impl_push_back
        ;
          template
          < typename... Head
          , typename Tail
          , template
            < typename...
            >class Package
          >
          struct
        impl_push_back
          < Package
            < Head...
            >
          , Tail
          >
        {  
                typedef
              Package
              < Head...
              , Tail
              >
            type
            ;
        };
        
          template
          < typename Package
          >
          struct
        impl_clear
        ;
          template
          < typename... Args
          >
          struct
        impl_clear
          < package
            < Args...
            >
          >
        {
                typedef
              package<>
            type;
            
        };
        
          template
          < typename... Args
          >
          struct
        iterator_fwd
        {
                typedef
              forward_iterator_tag
            category
            ;
        };
          template
          < typename Package
          >
          struct
        impl_begin
        ;
          template
          < typename... Args
          >
          struct
        impl_begin
          < package
            < Args...
            >
          >
        {  
                typedef
              iterator_fwd
              < Args...
              >
            type
            ;
        };
          template
          < typename Package
          >
          struct
        impl_end
        ;
          template
          < typename... Args
          >
          struct
        impl_end
          < package
            < Args...
            >
          >
        {  
                typedef
              iterator_fwd
              <
              >
            type
            ;
        };
    }//exit variadic namespace
    
      template
      <
      >
      struct
    size_impl
      < aux::package_tag
      >
    {
          template
          < class Package
          >
          struct
        apply
          : variadic::impl_size
            < typename Package::pkg_type
            >
        {
        };
    };
    
      template
      <
      >
      struct
    at_impl
      < aux::package_tag
      >
    {
          template
          < class Package
          , class N
          >
          struct
        apply
          : variadic::impl_at_c
            < typename Package::pkg_type
            , N::value
            >
        {
        };
    };
    
      template
      <
      >
      struct
    front_impl
      < aux::package_tag
      >
    {
          template
          < class Package
          >
          struct
        apply
          : variadic::impl_front
            < typename Package::pkg_type
            >
        {
            
        };
    };
    
      template
      <
      >
      struct
    push_front_impl
      < aux::package_tag
      >
    {
          template
          < class Package
          , class Head
          >
          struct
        apply
          : variadic::impl_push_front
            < Package
            , Head
            >
        {
        };
    };
    
      template
      <
      >
      struct
    pop_front_impl
      < aux::package_tag
      >
    {
          template
          < class Package
          >
          struct
        apply
          : variadic::impl_pop_front
            < Package
            >
        {
        };
    };
    
      template
      <
      >
      struct
    push_back_impl
      < aux::package_tag
      >
    {
          template
          < class Package
          , class Tail
          >
          struct
        apply
          : variadic::impl_push_back
            < Package
            , Tail
            >
        {
        };
    };
    
      template
      <
      >
      struct
    clear_impl
      < aux::package_tag
      >
    {
          template
          < class Package
          >
          struct
        apply
          : variadic::impl_clear
            < Package
            >
        {
        };
    };
    
      template
      <
      >
      struct
    begin_impl
      < aux::package_tag
      >
    {
          template
          < class Package
          >
          struct
        apply
          : variadic::impl_begin
            < typename Package::pkg_type
            >
        {
        };
    };
    
      template
      <
      >
      struct
    end_impl
      < aux::package_tag
      >
    {
          template
          < class Package
          >
          struct
        apply
          : variadic::impl_end
            < typename Package::pkg_type
            >
        {
        };
    };
    
      template
      < typename Head
      , typename... Tail
      >
      struct
    deref
      < variadic::iterator_fwd
        < Head
        , Tail...
        >
      >
    {
            typedef
          Head
        type
        ;
    };
      template
      < typename Head
      , typename... Tail
      >
      struct
    next
      < variadic::iterator_fwd
        < Head
        , Tail...
        >
      >
    {
            typedef
          variadic::iterator_fwd
          < Tail...
          >
        type
        ;
    };

}}//exit boost::mpl
#endif
