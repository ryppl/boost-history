//nodes with 2 children of same type
#ifndef LIBS_POLICY_PTR_TEST_BINARY_NODE_HPP_LJE20050513
#define LIBS_POLICY_PTR_TEST_BINARY_NODE_HPP_LJE20040513
#include "boost/utility/chk_dbl_die.hpp"
namespace binary_node
{

    enum
  field_numerals
  { left_index
  , right_index
  };
  
    enum
  end_numeral
  { num_fields=right_index+1
  };
  
    struct
  field_index
  {
   public:
      field_index(void)
        : my_index(left_index)
      {}
      field_index(end_numeral)
        : my_index(num_fields)
      {}
        void
      increment(void)
      {
          ++my_index;
      }
        bool
      empty(void)const
      {
          return num_fields<=my_index;
      }
        field_numerals
      deref(void)const
      /**@requires
       *  this->!empty()
       */
      {
          return field_numerals(my_index);
      }
   private:
        unsigned
      my_index
      ;
  };

    enum
  member_multiplicity
  /**@brief
   *  Indicates how many elements of containing class, a member variable
   *  contains, indirectly (via smart ptr or the like).
   */
  { contains_one
  , contains_two
  };
  
    enum
  constancy
  { constant_not
  , constant_yes
  };
  
    template
    < class T
    , constancy IsConstant
    >
    struct
  meta_constancy
  {
      typedef T type;
  };
  
    template
    < class T
    >
    struct
  meta_constancy
    < T
    , constant_yes
    >
  {
      typedef T const type;
  };
  
    template
    < class SpTypes
    , member_multiplicity
    >
    class 
  binary_container
  ;
    template
    < class SpTypes
    >
    class 
  binary_container
    < SpTypes
    , contains_one
    >
  : public utility::chk_dbl_die
  {
   public:
          typedef
        binary_container<SpTypes,contains_one>
      my_type
      ;
          typedef 
        typename SpTypes::template source<my_type>::type
      src_type
      ;
          typedef 
        typename SpTypes::template scalar<my_type>::type
      sp_type
      ;
        sp_type
      my_left_sp_child
      ;
        sp_type
      my_right_sp_child
      ;
          typedef
        sp_type my_type::*
      ptr_field
      ;
          static
        ptr_field
      field_at( field_numerals field_number )
      {
        static ptr_field const field_vec[ num_fields ]=
        { &my_type::my_left_sp_child
        , &my_type::my_right_sp_child
        };
        return field_vec[field_number];
      }
      
        sp_type&
      left(void)
      {
          return my_left_sp_child;
      }
        sp_type&
      right(void)
      {
          return my_right_sp_child;
      }
        sp_type const&
      left(void)const
      {
          return my_left_sp_child;
      }
        sp_type const&
      right(void)const
      {
          return my_right_sp_child;
      }
        template
        < constancy IsConst
        >
        struct
      iterators
        : public field_index
      {
       public:
              typedef
            typename meta_constancy<my_type,IsConst>::type
          container_type
          ;
              typedef
            typename meta_constancy<sp_type,IsConst>::type
          value_type
          ;
          iterators(container_type& a_container)
            : my_container(a_container)
          {}
          iterators(container_type& a_container, field_index const& a_index)
            : field_index(a_index)
            , my_container(a_container)
          {}
            void
          operator++(void)
          {
              field_index::increment();
          }
            value_type&
          operator*(void)const
          {
              return my_container.*my_type::field_at(this->deref());
          }
            bool
          operator==(iterators const& a_iter)const
          {
              if(this->deref() != a_iter.deref()) return false;
              if(&my_container != &a_iter.my_container) return false;
              return true;
          }
            bool
          operator!=(iterators const& a_iter)const
          {
              return !(this->operator==(a_iter));
          }
          
       private:
            container_type&
          my_container
          ;
      };
      
        iterators<constant_not>
      begin(void)
      {
          return iterators<constant_not>(*this);
      }
        iterators<constant_not>
      end(void)
      {
          return iterators<constant_not>(*this,field_index(num_fields));
      }
      
          typedef
        iterators<constant_not>
      iterator
      ;
      
        iterators<constant_yes>
      begin(void)const
      {
          return iterators<constant_yes>(*this,field_index());
      }
        iterators<constant_yes>
      end(void)const
      {
          return iterators<constant_yes>(*this,field_index(num_fields));
      }
      
          typedef
        iterators<constant_yes>
      const_iterator
      ;
      
  };//end binary_container<SpTypes,contains_one> template specialization
  
    template
    < class SpTypes
    >
  class binary_container
    < SpTypes
    , contains_two
    >
  : public utility::chk_dbl_die
  {
   public:
          typedef
        binary_container<SpTypes,contains_two>
      my_type
      ;
          typedef 
        typename SpTypes::template source<my_type>::type
      src_type
      ;
          typedef 
        typename SpTypes::template scalar<my_type>::type
      sp_type
      ;
          typedef
        typename SpTypes::template container<sp_type>::type
      container_type
      ;
        container_type
      my_sp_children
      ;
      
      ~binary_container(void)
      {
        #ifdef BOOST_MANAGED_PTR_TRACE_MODE
          mout()<<"binary_container-"<<":this="<<this<<":obj_id="<<id_get()<<"\n";
        #endif
      }
      
      binary_container(void)
      : my_sp_children(2)
      {
        #ifdef BOOST_MANAGED_PTR_TRACE_MODE
          mout()<<"binary_container+"<<":this="<<this<<":obj_id="<<id_get()<<"\n";
        #endif
      }
        sp_type&
      left(void)
      {
          return my_sp_children[0];
      }
        sp_type&
      right(void)
      {
          return my_sp_children[1];
      }
        sp_type const&
      left(void)const
      {
          return my_sp_children[0];
      }
        sp_type const&
      right(void)const
      {
          return my_sp_children[1];
      }
      
          typedef
        typename container_type::iterator
      iterator
      ;
      
        iterator
      begin(void)
      {
          return my_sp_children.begin();
      }
      
        iterator
      end(void)
      {
          return my_sp_children.end();
      }
      
          typedef
        typename container_type::const_iterator
      const_iterator
      ;
      
        const_iterator
      begin(void)const
      {
          return my_sp_children.begin();
      }
      
        const_iterator
      end(void)const
      {
          return my_sp_children.end();
      }
      
  };//end binary_container

}//exit binary_node namespace   

#endif //include guard
