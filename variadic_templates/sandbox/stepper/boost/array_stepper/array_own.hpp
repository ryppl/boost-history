#ifndef BOOST_ARRAY_STEPPER_ARRAY_OWN_HPP_INCLUDED
#define BOOST_ARRAY_STEPPER_ARRAY_OWN_HPP_INCLUDED
#include <boost/array_stepper/stepper_offset.hpp>
namespace boost
{
namespace array_stepper
{

  enum
array_type
{ arr_own //array that owns its data.
, arr_ref //array that references data belonging to an arr_own.
, arr_cref//same as arr_ref but data is const.
};

  template
  < typename Value
  , typename Index=int
  , array_type ArrType=arr_own
  >
struct array
;
  template
  < typename Value
  , typename Index
  >
struct array_data
  : stepper_offset<Index>
{
        typedef
      stepper_offset<Index>
    domain_t
      ;
        typedef
      typename domain_t::index_t
    index_t
      ;
        typedef
      typename domain_t::axis_t
    axis_t
      ;
        typedef
      typename domain_t::lengths_t
    lengths_t
      ;
        typedef
      typename domain_t::offset_t
    offset_t
      ;
        typedef
      Value
    value_t
      ;
        typedef
      std::vector<Value>
    data_t
      ;
    array_data
      ( lengths_t const& a_lengths
      , permut_predefined a_permut_pred=permut_default
      )
      : domain_t(a_lengths.size())
      , my_data
        ( this->domain_t::init_strides
          ( a_lengths
          , permutation0_last<axis_t>::permute_select(a_permut_pred)(a_lengths.size())
          )
        )
      {
      }
      
      void
    reshape
      ( lengths_t const& a_lengths
      , permut_predefined a_permut_pred=permut_default
      )
      {
          this->stepper_ones().resize(a_lengths.size());
          offset_t new_size=
            this->domain_t::init_strides
            ( a_lengths
            , permutation0_last<axis_t>::permute_select(a_permut_pred)(a_lengths.size())
            );
          my_data.resize(new_size);
      }
      
      data_t&
    data()
      {
          return my_data;
      }
      data_t const&
    data()const
      {
          return my_data;
      }
      
 private:    
      data_t
    my_data
      ;
};

  template
  < typename Value
  , typename Index
  >
struct array
  < Value
  , Index
  , arr_cref
  >
{
        typedef
      stepper_offset<Index>
    domain_t
      ;
        typedef
      typename domain_t::axis_t
    axis_t
      ;
        typedef
      typename domain_t::index_t
    index_t
      ;
        typedef
      typename domain_t::lengths_t
    lengths_t
      ;
        typedef
      typename domain_t::offset_t
    offset_t
      ;
        typedef
      Value
    value_t
      ;
        typedef
      std::vector<Value>
    data_t
      ;
        typedef
      typename data_t::const_iterator
    data_iter_t
      ;
        typedef
      typename domain_t::const_iterator
    domain_iter_t
      ;
    array
      ( array_data< Value, Index>const& a_val
      , index_t a_index
      )
      : my_data_iter
        ( a_val.data().begin()
        + a_val.offset()
        + a_val.stepper_ones().front().offset(a_index)
        )
      , my_domain_iter(a_val.domain_t::begin()+1)
      , my_rank(a_val.rank()-1)
      {
      }
    array
      ( array< Value, Index, arr_cref>const& a_val
      , index_t a_index
      )
      : my_data_iter
        ( a_val.my_data_iter
        + a_val.my_domain_iter->offset(a_index)
        )
      , my_domain_iter(a_val.my_domain_iter+1)
      , my_rank(a_val.my_rank-1)
      {
      }
      
      axis_t
    rank()const
    {
        return my_rank;
    }
      
      index_t
    size()const
    {
        typename domain_t::stepper_one_t const&s1=*my_domain_iter;
        return s1[stepr_length];
    }
      value_t const&
    operator()()const
      {
          return *my_data_iter;
      }
      
      value_t const&
    operator()(index_t a_index)const
      {
          auto l_offset=my_domain_iter->offset(a_index);
          return *(my_data_iter+l_offset);
      }
      
      array< value_t, Index, arr_cref>
    operator[](index_t a_index)const
      {
          return array< value_t, Index, arr_cref>( *this, a_index);
      }
      
 private:
      data_iter_t
    my_data_iter
      ;
      domain_iter_t
    my_domain_iter
      ;
      axis_t
    my_rank
      ;    
};
  
  template
  < typename Value
  , typename Index
  >
struct array
  < Value
  , Index
  , arr_ref
  >
{
        typedef
      stepper_offset<Index>
    domain_t
      ;
        typedef
      typename domain_t::axis_t
    axis_t
      ;
        typedef
      typename domain_t::index_t
    index_t
      ;
        typedef
      typename domain_t::lengths_t
    lengths_t
      ;
        typedef
      typename domain_t::offset_t
    offset_t
      ;
        typedef
      Value
    value_t
      ;
        typedef
      std::vector<Value>
    data_t
      ;
        typedef
      typename data_t::iterator
    data_iter_t
      ;
        typedef
      typename domain_t::const_iterator
    domain_iter_t
      ;
    array
      ( array_data< Value, Index>& a_val
      , index_t a_index
      )
      : my_data_iter
        ( a_val.data().begin()
        + a_val.offset()
        + a_val.stepper_ones().front()[stepr_stride]*a_index
        )
      , my_domain_iter(a_val.domain_t::begin()+1)
      , my_rank(a_val.rank()-1)
      {
      }
    array
      ( array< Value, Index, arr_ref>& a_val
      , index_t a_index
      )
      : my_data_iter
        ( a_val.my_data_iter
        + a_val.my_domain_iter->offset( a_index)
        )
      , my_domain_iter(a_val.my_domain_iter+1)
      , my_rank(a_val.my_rank-1)
      {
      }
      
      axis_t
    rank()const
    {
        return my_rank;
    }
      
      index_t
    size()const
    {
        return (*my_domain_iter)[stepr_length];
    }
       value_t&
    operator()()
      {
          return *my_data_iter;
      }
      
      value_t const&
    operator()()const
      {
          return *my_data_iter;
      }
      
     value_t&
    operator()(index_t a_index)
      {
          return *(my_data_iter+a_index);
      }
      
      value_t const&
    operator()(index_t a_index)const
      {
          return *(my_data_iter+a_index);
      }
      
      array< value_t, Index, arr_ref>
    operator[](index_t a_index)
      {
          return array< value_t, Index, arr_ref>( *this, a_index);
      }
      
      array< value_t, Index, arr_cref>
    operator[](index_t a_index)const
      {
          return array< value_t, Index, arr_cref>( *this, a_index);
      }
      
 private:
      data_iter_t
    my_data_iter
      ;
      domain_iter_t
    my_domain_iter
      ;
      axis_t
    my_rank
      ;    
};
  
  template
  < typename Value
  , typename Index
  >
struct array
  < Value
  , Index
  , arr_own
  >
  : array_data< Value, Index>
{
        typedef
      array_data< Value, Index>
    super_t
      ;
        typedef
      typename super_t::domain_t
    domain_t
      ;
        typedef
      typename domain_t::index_t
    index_t
      ;
        typedef
      typename domain_t::lengths_t
    lengths_t
      ;
        typedef
      typename domain_t::offset_t
    offset_t
      ;
        typedef
      typename super_t::value_t
    value_t
      ;
    array
      ( lengths_t const& a_lengths
      , permut_predefined a_permut_pred=permut_default
      )
      : super_t
        ( a_lengths
        , a_permut_pred
        )
      {
      }
      
      value_t&
    operator()()
      {
          return this->data().front();
      }
      
      value_t const&
    operator()()const
      {
          return this->data().front();
      }
      
      value_t&
    operator()(index_t a_index)
      {
          return this->data()[a_index];
      }
      
      value_t const&
    operator()(index_t a_index)const
      {
          return this->data()[a_index];
      }
      
      array< value_t, Index, arr_ref>
    operator[](index_t a_index)
      {
          return array< value_t, Index, arr_ref>( *this, a_index);
      }
      
      array< value_t, Index, arr_cref>
    operator[](index_t a_index)const
      {
          return array< value_t, Index, arr_cref>( *this, a_index);
      }
      
};

}//exit array_stepper namespace
}//exit boost namespace
#endif
