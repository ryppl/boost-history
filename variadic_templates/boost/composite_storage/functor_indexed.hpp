//
#ifndef BOOST_COMPOSITE_STORAGE_FUNCTOR_INDEXED_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_FUNCTOR_INDEXED_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//====================================================================
//[switch #includes
//  These are from:
//
//    http://svn.boost.org/svn/boost/sandbox/switch/
//
  #include <boost/control/switch.hpp>
  #include <boost/control/case.hpp>
//]switch #includes
#include <boost/assert.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost
{
namespace composite_storage
{
namespace functor_indexed
//  This namespace defines support templates
//  for use of "Index Functors" taking only index arguments.
//  A "Index Functor" is a functor with the 
//  "Index Functor Interface":
//
//    IndexFunctor::result_type IndexFunctor::operator()
//    ( mpl::integral_c<IndexType,IndexValu> case_c )
//
//  where:
//    
//    IndexFunctor is an Index Functor.
//    typedef IndexFunctor::cases::value_type IndexType;
//    IndexType IndexValu;
//
//  the case_c argument above is called 
//    "case argument"
//  below.
//
{

//The following modified from:
//  http://svn.boost.org/svn/boost/sandbox/switch/libs/switch/example/apply_visitor.cpp
//As it was on 2009-12-03.1238 CST
//
//The comments here starting with //[functor_indexed
//correspond to the comments in apply_visitor.cpp starting with //[apply_visitor
//
//[functor_indexed_implementation
//
// bring switch_ and case_ into scope.
using namespace boost::control;

// One of the cases should always be selected.  If the
// the default is executed it must be an error.
template<class R>
struct never_called 
{
    template<class Int>
    R operator()(Int) 
    {
        BOOST_ASSERT(!"this function should never be called.");
    }
};

  template
  < class Visitor 
    //functor with signature (case_c,Component) -> Visitor::result_type
    //where:
    //  case_c (see  below).
    //  component is some component of Variant.
    //
  , class Variant
    //  Abstractly, a "dependent index pair", IOW, like std::pair<Key,Value>,
    //  except the type of Value depends on the value of Key part of the pair.
    //  Also Key is of type like case_c below.
    //
    //  The possible values of Value are called the Components of Variant.
  >
struct visitor_variant
/**@brief
 *  Adapts the Visitor functor to the 
 *  Index Functor Interface.
 */
{
 public:
        typedef
      typename Visitor::cases
    cases
    ;
        typedef
      typename cases::value_type
    case_type
    ;
        typedef
      typename Visitor::result_type
    result_type
    ;
    visitor_variant( Visitor& visitor, Variant& variant)
        : visitor_(visitor)
        , variant_(variant)
    {}
      template<case_type CaseValu>
      result_type
    operator()
      ( mpl::integral_c<case_type,CaseValu> case_c
      )
    {
            typedef
          typename Variant::index_type
        index_type
        ;
          return
        visitor_
        ( case_c //pass index to visitor
        , *(variant_.template project<index_type(CaseValu)>())//pass component to visitor
        );
    }
 private:
    Visitor& visitor_;
    Variant& variant_;
};

  template
  < class IndexFunctor //an Index Functor (see above).
  >
  struct
apply_static
{
        typedef
      typename remove_cv<IndexFunctor>::type
    index_functor
    ;
        typedef
      typename index_functor::result_type
    result_type
    ;
        typedef
      typename index_functor::cases
    cases
    ;
        static
      result_type
    _    
      ( IndexFunctor& index_functor
      , typename index_functor::cases::value_type a_case
      )
    {
          never_called<result_type>
        default_
        ;
          return 
        switch_<result_type>(a_case, case_<cases>(index_functor), default_);
    }
};

  template
  < class IndexFunctor //an Index Functor (see above).
  >
  typename IndexFunctor::result_type
apply
  ( IndexFunctor& index_functor
  , typename IndexFunctor::cases::value_type a_case
  )
{
    return apply_static<IndexFunctor>::_( index_functor, a_case);
};

//]functor_indexed_implementation

  template
  < typename Layout
  , typename Case0=typename Layout::index_undefined
  >
struct layout_visitor
{
        typedef
      typename Case0::value_type
    case_type
    ;  
        typedef
      mpl::range_c
      < case_type
      , Case0::value
      , Layout::index_end::value
      >
    cases
    ;
};

}//exit namespace functor_indexed

}//exit composite_storage namespace
}//exit boost namespace
#endif
