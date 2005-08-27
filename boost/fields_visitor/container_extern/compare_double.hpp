#ifndef BOOST_FIELDS_VISITOR_CONTAINER_EXTERN_COMPARE_DOUBLE_HPP_LJE20050815
#define BOOST_FIELDS_VISITOR_CONTAINER_EXTERN_COMPARE_DOUBLE_HPP_LJE20050815
//  (C) Copyright Larry Evans 2005.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#ifdef CONTAINER_SINGLE_OBJ_ID
#include "boost/utility/obj_id.hpp"
#endif
#include "boost/fields_visitor/registrar_heirarchy.hpp"
#include "boost/fields_visitor/iter_range.hpp"
#include <memory>
namespace boost
{
namespace fields_visitor
{
namespace container_extern
{
/**@brief
 *  Enable visiting a record's fields where
 *  record is a templated container with
 *  a compare_double template parameter describing it's
 *  contained fields.
 */
template
  < template
      < typename //Key
      , typename //Definition
      , typename //Compare with default value
      , typename //Allocator with default value
      >
    class Container
  , typename Key
  , typename Definition
  , typename VisitorSequence
  , typename Compare=std::less<Key>
  , typename Allocator=std::allocator<std::pair<Key const,Definition> >
  >
  class
compare_double
  : public Container<Key,Definition,Compare,Allocator>
#ifdef CONTAINER_SINGLE_OBJ_ID
  , public utility::obj_id
#endif
  , public registrar_heirarchy<VisitorSequence>
{
 public:
        typedef
      Key
    key_type
    ;
        typedef
      Definition
    def_type
    ;
        typedef
      compare_double
        < Container
        , key_type
        , def_type
        , VisitorSequence
        , Compare
        , Allocator
        >
    my_type
    ;
        typedef
      Container<key_type, def_type, Compare, Allocator>
    container_type
    ;
        typedef
      registrar_heirarchy<VisitorSequence>
    registrar_type
    ;
    ~compare_double(void)
    {
    }
    compare_double
      ( void
      )
      : registrar_type(this)
    {
    }
};//end compare_double<FieldsVisitor, Container, Key> general template
  
}//exit container_extern namespace

template
  < typename VisitorSequence
  , typename FieldsVisitor
  , template
      < typename //Key
      , typename //Definition
      , typename //Compare
      , typename //Allocator
      >
      class Container
  , typename Key
  , typename Definition
  , typename Compare
  , typename Allocator
  >
struct field_iterator_of
  < FieldsVisitor
  , container_extern::compare_double
    < Container
    , Key
    , Definition
    , VisitorSequence
    , Compare
    , Allocator
    >
  >
{
 public:
        typedef
      FieldsVisitor
    fields_visitor_type
    ;
        typedef
      Container
      < Key
      , Definition
      , Compare
      , Allocator
      >
    container_type
    ;
        typedef
      iter_range<container_type>
    iter_type
    ;
        typedef    
      container_extern::compare_double
      < Container
      , Key
      , Definition
      , VisitorSequence
      , Compare
      , Allocator
      >
    record_type
    ;
    field_iterator_of
      ( record_type* a_record
      )
      : m_iter(*a_record)
    {
    }
      void
    accept(fields_visitor_type& a_visitor)
    { 
        typedef typename container_type::key_type key_type;
        typedef typename container_type::value_type pair_type;
        typedef typename pair_type::second_type def_type;
        pair_type& a_pair = *m_iter;
        key_type& a_key=const_cast<key_type&>(a_pair.first);
        def_type& a_def=a_pair.second;
        a_visitor.visit_field(a_key);
        a_visitor.visit_field(a_def);
    }
      void
    increment(void)
    { 
        ++m_iter;
    } 
      std::size_t
    size(void)const
    { 
        field_iterator_of l_cpy(*this);
        std::size_t l_siz=0;
        for(; !l_cpy.empty(); ++l_siz, l_cpy.increment())
        return l_siz;
    } 
      bool
    empty(void)const
    { 
        return m_iter.empty();
    }
    
private:
      iter_type
    m_iter
    ;
};//end field_iterator_of<FieldsVisitor,container_extern::compare_double<...> > specialization

}//exit fields_visitor namespace
}//exit boost namespace
//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2005-08-17: Larry Evans
//     WHAT:
//       In accept, had to const_cast key.
//     WHY:
//       Avoid compile-time error during compile of libs/policy_ptr/test/
//       wartens_sparse_matrix_test.cpp.~1.2~ (my local version).
//   2005-08-15: Larry Evans
//     WHAT:
//       copied and and modified from single.hpp.
//     WHY:
//       allow field visitaion of std::map and other associative containers.
//////////////////////////////////////////////////////////////////////////////
#endif
