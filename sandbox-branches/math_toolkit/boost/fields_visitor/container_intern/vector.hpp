#ifndef BOOST_FIELDS_VISITOR_CONTAINER_INTERN_VECTOR_HPP_INCLUDED
#define BOOST_FIELDS_VISITOR_CONTAINER_INTERN_VECTOR_HPP_INCLUDED
/**@file
 *  Prototype of how to modify std::vector to use 
 *  fields_visitor to enable enumeration of contained
 *  fields (which in this case, are just the elements
 *  of the vector).
 */

#include "boost/fields_visitor/container_intern/prox_indirect.hpp"
#ifdef CONTAINER_SINGLE_OBJ_ID
#include "boost/utility/obj_id.hpp"
#include "boost/utility/trace_scope.hpp"
#endif
namespace boost
{
namespace fields_visitor
{
namespace container_intern
{
    template
      < typename Value
      , typename Allocator=allocator<Value> //from prox_indirect
      >
    struct vector
    /**@brief
     *  A prototype of proposed modification to the way 
     *  This prototype represents the current vector.
     *  Specialization on 2nd template parameter to allocator, as is
     *  possible in:
     *    field_iterator_of_container_intern_single.hpp
     *  will allow enumeration of elements with fields_visitor 
     *  library.
     */
    #ifdef CONTAINER_SINGLE_OBJ_ID
      : public utility::obj_id
    #endif
      {
            typedef
          Value
        value_type
          ;
            typedef
          Allocator
        allocator_type
          ;
            typedef
          value_type&
        reference
          ;
            typedef
          value_type const&
        const_reference
          ;
            typedef
          vector<value_type,allocator_type>
        my_type
          ;
          unsigned
        m_size
          ;
          typename allocator_type::root_pointer
        m_values
          ;
            typedef
          value_type*
        iterator
          ;
          iterator
        begin(void)
          { return m_values.subj_mut()
          ;}
          iterator
        end(void)
          { return m_values.subj_mut()+m_size
          ;}
            typedef
          value_type const*
        const_iterator
          ;
          const_iterator
        begin(void)const
          { return m_values.subj_con()
          ;}
          const_iterator
        end(void)const
          { return m_values.subj_con()+m_size
          ;}
        ~vector(void)
          {
            #ifdef CONTAINER_SINGLE_OBJ_ID
            utility::trace_scope ts("-vector(void)");
            mout()<<"vector::TOR-:id="<<id_get()<<":"<<"\n";
            #endif
            delete[] m_values.subj_mut();
          }
    
        vector(unsigned a_size=1)
          : m_size(a_size)
          , m_values(new value_type[m_size], this)
          {
            #ifdef CONTAINER_SINGLE_OBJ_ID
            utility::trace_scope ts("+vector(unsigned)");
            mout()<<"vector::TOR+:id="<<id_get()<<":size="<<m_size<<"\n";
            #endif
          }
          unsigned
        size(void)const
          { return m_size
          ;}
          value_type&
        operator[](unsigned i)
          { return *(begin()+i)
          ;}
      };//end vector<E,A> struct general template
      
}//exit container_intern namespace
}//exit fields_visitor namespace
}//exit boost namespace

#endif
